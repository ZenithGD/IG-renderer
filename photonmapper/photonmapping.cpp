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
using LightPhotonPair = tuple<shared_ptr<Light>, list<Photon>>;

struct PixelResult
{
    unsigned int x, y;
    RGB contribution;
};

void photonTraceRay(const Scene& sc, const Ray& r, LightPhotonPair& t, const RGB& energy,
    const unsigned int photons, const unsigned int total, const unsigned int bounces) {

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

                    // bounce > 0
                    //if ( bounces > 0 ) {
                        // Add photons to light source
                        get<1>(t).push_back(p);
                        get<0>(t)->count++;
                    //}
                }
                photonTraceRay(sc, out, t, outEnergy, photons, total, bounces + 1);
            }
        }
    }
}

void photonTrace(const Scene& sc, vector<LightPhotonPair>& lightPhotons,
    const unsigned int photons, const unsigned int total) {

    auto lights = sc.lights.size();
    double luminanceTotal = 0.0;
    for ( auto& l : sc.lights ) {
        luminanceTotal += l->power.getLuminance();
    }

    for ( auto& t : lightPhotons ) {

        // Photon initial energy
        RGB energy = get<0>(t)->power * 4 * M_PI;

        int times = photons * (get<0>(t)->power.getLuminance() / luminanceTotal); 
    
        if ( get<0>(t)->count < times ) {
            Ray r = get<0>(t)->sample();

            photonTraceRay(sc, r, t, energy, photons, total, 0);
        }
    }
}

RGB densityEstimation(const Vector3& x, const Vector3& inDir,  const Intersection& it, const vector<const Photon*>& closestPhotons) {
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

RGB traceRay(const Ray& r, const Scene& sc, const PhotonMap& pmap) {
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

        auto nearestPhotons = pmap.nearest_neighbors(r(closest.closest()), 100);
        
        auto interaction = closest.brdf->sample(r.direction, r(closest.closest()), closest);
        
        if ( interaction.has_value() ) {

            auto [ outDirection, radiance, isDelta ] = interaction.value();
            if ( isDelta ) {
                Ray outRay(r(closest.closest()), outDirection);

                // Recursively trace ray
                return traceRay(outRay, sc, pmap);
            }
            
        
            if ( !nearestPhotons.empty() ) {
                //+ nextEventEstimation(sc, r(closest.closest()), r.direction, closest)
                return densityEstimation(r(closest.closest()), r.direction, closest, nearestPhotons);
            }
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

Image render(const Scene& sc, const PhotonMap& pmap) {

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

                    contrib = contrib + traceRay(ray, sc, pmap);
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

Image photonMapping(const Scene& sc, const unsigned int total, const unsigned int maxPhotons) {
    
    auto lights = sc.lights.size();
    vector<LightPhotonPair> lightPhotons(lights);
    transform(sc.lights.begin(), sc.lights.end(), lightPhotons.begin(), [](shared_ptr<Light> l) -> LightPhotonPair {
        return make_tuple(l, list<Photon>());
    });
    
    int nextPhotonNum = 0;

    while ( true ){
        photonTrace(sc, lightPhotons, total, maxPhotons);

        
        int totalPhotons = 0;
        for ( auto& t : lightPhotons ) {
            totalPhotons += get<0>(t)->count;
        }

        if ( totalPhotons > nextPhotonNum ) {
            cout << "Current photons : " << totalPhotons << endl;
            nextPhotonNum += 2000;
        }

        if ( totalPhotons >= maxPhotons ) {
            cout << "Photons : " << totalPhotons << endl;
            break;
        }
    }
    

    // Normalize every photon by the number of photons emitted by the light
    for ( auto& t : lightPhotons ) {
        for ( auto& p : get<1>(t) ) {
            if ( get<0>(t)->count > 0 ) {
                p.flux = p.flux / (double)get<0>(t)->count;
            }
        }
    }


    list<Photon> photons;
    for ( auto& t : lightPhotons ) {
        copy(get<1>(t).begin(), get<1>(t).end(), back_insert_iterator<list<Photon>>(photons));
    }

    std::cout << "Generating photon map...";
    // Photon Map
    PhotonMap pmap(photons, PhotonAxisPosition());
    std::cout << "done." << std::endl;

    // Density estimation
    auto L = render(sc, photons);
    // render photon map directly
    // return renderPhotonMap(sc, pmap);

    return L;

}

