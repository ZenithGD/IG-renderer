#include <iostream>
#include <iomanip>
#include <cmath>
#include <photonmapper/photonmapping.hpp>
#include <photonmapper/photon.hpp>
#include <acceleration/kdtree.hpp>
#include <acceleration/threadpool.hpp>
#include <color/color.hpp>
#include <pathtracer/pathtracing.hpp>

using PhotonMap = nn::KDTree<Photon, 3, PhotonAxisPosition>;

struct LightPhotonInfo {
    shared_ptr<Light> light;
    list<Photon> photons, causticPhotons;
};

struct PixelResult
{
    unsigned int x, y;
    RGB contribution;
};

void photonTraceRay(const Scene& sc, const Ray& r, LightPhotonInfo& t, const RGB& energy, 
    const unsigned int bounces, bool isCaustic, const PhotonConfig& pc) {

    if ( bounces >= sc.getProps().bounces ) return;

    Intersection closest {
        .intersects = false,
    };

    double closestT = INFINITY;
    for ( auto p : sc.primitives ) {

        Intersection inter = p->intersection(r, 0);
        if ( inter.intersects && inter.closest() < closestT ){
            closest = inter;
            closestT = inter.closest();
        }
    }
    
    // trace direct light ray
    if( closest.intersects ) {

        if ( !closest.brdf->emitter ) {
            auto result = closest.brdf->sample(normalize(r.direction), r(closest.closest()), closest);
            
            // Check if ray was absorbed
            if ( result.has_value() ) {

                auto [ omega, li, isDelta ] = result.value();
                Ray out(r(closest.closest()), omega);

                RGB outEnergy = energy;

                if ( !isDelta ) {
                    Photon p(r(closest.closest()), omega, 
                    energy, closest.closestNormal());

                    outEnergy = energy * li;

                    // bounce > 0 if we take next event estimation into account
                    if ( pc.nee || bounces > 0 ) {
                        // Add photons to light source
                        if ( isCaustic ) {
                            t.causticPhotons.push_back(p);
                        } else {
                            t.photons.push_back(p);
                        }
                        t.light->count++;
                    }
                }
                photonTraceRay(sc, out, t, outEnergy, bounces + 1, isDelta, pc);
            }
        }
    }
}

void photonTrace(const Scene& sc, vector<LightPhotonInfo>& lightPhotons, const PhotonConfig& pc) {

    auto lights = sc.lights.size();
    double luminanceTotal = 0.0;
    for ( auto& l : sc.lights ) {
        luminanceTotal += l->power.getLuminance();
    }

    for ( auto& t : lightPhotons ) {

        // Photon initial energy
        RGB energy = t.light->power * 4 * M_PI;

        int times = pc.maxPhotons * (t.light->power.getLuminance() / luminanceTotal); 
    
        if ( t.light->count < times ) {
            Ray r = t.light->sample();

            photonTraceRay(sc, r, t, energy, 0, false, pc);
        }
    }
}

RGB densityEstimation(const Vector3& x, const Vector3& inDir, const Intersection& it, 
    const vector<const Photon*>& closestPhotons) {
    RGB color = RGB();
    double rk = -INFINITY;
    for (auto photon : closestPhotons) {
        auto a = x - photon->position_;
        rk = max(a.modulus(), rk);
    }

    for (auto photon : closestPhotons){
        
        auto result = it.brdf->eval(x, inDir, photon->inDirection, it);
            
        color = color + result * (photon->flux / (M_PI * rk * rk));
    }

    return color;
}

RGB traceRay(const Ray& r, const Scene& sc, const PhotonMap& pmap, const PhotonMap& cmap, const PhotonConfig& pc) {
    Intersection closest {
        .intersects = false,
    };

    double closestT = INFINITY;
    for ( auto p : sc.primitives ) {

        Intersection inter = p->intersection(r, 0);
        if ( inter.intersects && inter.closest() < closestT ){
            closest = inter;
            closestT = inter.closest();
        }
    }
    
    // trace direct light ray
    if( closest.intersects ) {
        
        auto interaction = closest.brdf->sample(normalize(r.direction), r(closest.closest()), closest);
        
        if ( interaction.has_value() ) {

            auto [ outDirection, radiance, isDelta ] = interaction.value();
            if ( isDelta ) {
                Ray outRay(r(closest.closest()), outDirection);

                // Recursively trace ray
                return traceRay(outRay, sc, pmap, cmap, pc);
            }

            // Non-delta interaction, density estimation on the intersection point
            auto nearestPhotons = pmap.nearest_neighbors(r(closest.closest()), 50);
            // Non-delta interaction, density estimation on the intersection point
            auto causticPhotons = cmap.nearest_neighbors(r(closest.closest()), 50);

            RGB contrib = pc.nee 
                ? nextEventEstimation(sc, r(closest.closest()), r.direction, closest)
                : RGB();
        
            if ( !nearestPhotons.empty() ) {
                contrib = contrib + densityEstimation(r(closest.closest()), r.direction, closest, nearestPhotons) * pc.photonBoost;
            }

            if ( !causticPhotons.empty() ) {
                contrib = contrib + densityEstimation(r(closest.closest()), r.direction, closest, causticPhotons) * pc.causticBoost;
            }

            return contrib;
        } 
    }

    return RGB();
}


RGB tracePhotonMapRay(const Ray& r, const Scene& sc, const PhotonMap& pmap) {
    Intersection closest {
        .intersects = false,
    };

    double closestT = INFINITY;
    for ( auto p : sc.primitives ) {
        Intersection inter = p->intersection(r, 0);
        if ( inter.intersects && inter.closest() < closestT ){
            auto interaction = inter.brdf->sample(r.direction, r(inter.closest()), inter);
            
            if ( interaction.has_value() ) {
                auto [ outDirection, radiance, isDelta ] = interaction.value();
                if ( !isDelta ) {
                    closest = inter;
                    closestT = inter.closest();
                }
            }
        }
    }
    
    if( closest.intersects ) {

        auto nearestPhotons = pmap.nearest_neighbors(r(closest.closest()), 1, 0.01);
        
        auto interaction = closest.brdf->sample(r.direction, r(closest.closest()), closest);
        
        if ( interaction.has_value() ) {

            auto [ outDirection, radiance, isDelta ] = interaction.value();
            if ( isDelta ) {
                throw logic_error("Shouldn't be delta interaction");
            }
        
            if ( !nearestPhotons.empty() ) {
                //+ nextEventEstimation(sc, r(closest.closest()), r.direction, closest)
                return densityEstimation(r(closest.closest()), r.direction, closest, nearestPhotons);
            }
        } 
    }

    return RGB();
}

Image render(const Scene& sc, const PhotonMap& pmap, const PhotonMap& cmap, const PhotonConfig& pc) {

    // task function should have pixel position and rays as arguments
    using TaskFn = std::function<PixelResult(void)>;

    Threadpool<TaskFn, PixelResult> tp(sc.getProps().threads);

    Image img(sc.getProps().viewportWidth, sc.getProps().viewportHeight);

    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            tp.enqueueTask([&, i, j]() -> PixelResult
            {
                RGB contrib = RGB();

                auto rays = sc.cam.perPixel(j, i, sc.getProps().antialiasingFactor);

                for ( const Ray& ray : rays ) {

                    contrib = contrib + traceRay(ray, sc, pmap, cmap, pc);
                }
                return PixelResult{ .x = j, .y = i, .contribution = contrib / (double)rays.size() }; 
            });
        }
    }

    double maxValue = 0;
    double nextval = 0;

    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            double progress = double( i * img.width + j ) / double(img.height * img.width);
            if ( progress >= nextval ) {
                nextval += 0.005;
                std::cout << setprecision(3) << progress << " % .. " << flush;
            }

            PixelResult res = tp.getResult();
            img.imageData[res.y][res.x] = res.contribution;
            //std::cout << "i = " << i << ", j = " << j << std::endl;

            img.maxNumber = max({img.maxNumber, res.contribution.red, res.contribution.green, res.contribution.blue});
        }
    }

    return img;
}

Image photonMapping(const Scene& sc, const PhotonConfig& pc) {
    
    auto lights = sc.lights.size();
    vector<LightPhotonInfo> lightPhotons(lights);
    transform(sc.lights.begin(), sc.lights.end(), lightPhotons.begin(), [](shared_ptr<Light> l) -> LightPhotonInfo {
        return {l, list<Photon>(), list<Photon>()};
    });
    
    int nextPhotonNum = 0;

    while ( true ){
        photonTrace(sc, lightPhotons, pc);

        
        int totalPhotons = 0;
        for ( auto& t : lightPhotons ) {
            totalPhotons += t.light->count;
        }

        if ( totalPhotons > nextPhotonNum ) {
            cout << "Current photons : " << totalPhotons << endl;
            nextPhotonNum += 2000;
        }

        if ( totalPhotons >= pc.maxPhotons ) {
            cout << "Photons : " << totalPhotons << endl;
            break;
        }
    }
    

    // Normalize every photon by the number of photons emitted by the light
    for ( auto& t : lightPhotons ) {
        for ( auto& p : t.photons ) {
            if ( t.light->count > 0 ) {
                p.flux = p.flux / (double)t.light->count;
            }
        }

        for ( auto& p : t.causticPhotons ) {
            if ( t.light->count > 0 ) {
                p.flux = p.flux / (double)t.light->count;
            }
        }
    }


    list<Photon> photons;
    for ( auto& t : lightPhotons ) {
        copy(t.photons.begin(), t.photons.end(), back_insert_iterator<list<Photon>>(photons));
    }

    list<Photon> causticPhotons;
    for ( auto& t : lightPhotons ) {
        copy(t.causticPhotons.begin(), t.causticPhotons.end(), back_insert_iterator<list<Photon>>(causticPhotons));
    }

    std::cout << "Generating photon map...";
    // Photon Map
    PhotonMap pmap(photons, PhotonAxisPosition());
    
    std::cout << "done." << std::endl;
    std::cout << "Generating caustic photon map...";
    PhotonMap cmap(causticPhotons, PhotonAxisPosition());
    std::cout << "done." << std::endl;

    // Density estimation
    auto L = render(sc, pmap, cmap, pc);
    // render photon map directly
    // return renderPhotonMap(sc, pmap);

    return L;

}

