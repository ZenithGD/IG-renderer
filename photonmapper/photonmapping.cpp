#include <iomanip>
#include <cmath>
#include <photonmapper/photonmapping.hpp>
#include <photonmapper/photon.hpp>
#include <acceleration/kdtree.hpp>
#include <acceleration/threadpool.hpp>
#include <color/color.hpp>

using PhotonMap = nn::KDTree<Photon, 3, PhotonAxisPosition>;
using LightPhotonPair = tuple<shared_ptr<Light>, list<Photon>>;

void photonTraceRay(const Scene& sc, const Ray& r, LightPhotonPair& t,
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

    // Photon initial energy
    RGB energy = get<0>(t)->power * 4 * M_PI;
    
    // trace direct light ray
    if( closest.intersects ) {

        if ( !closest.brdf->emitter ) {
            auto result = closest.brdf->sample(normalize(r.direction), r(closest.closest()), closest);
            
            // Check if ray was absorbed
            if ( result.has_value() ) {

                auto [ omega, li ] = result.value();
                Ray out(r(closest.closest()), omega);

                Photon p(r(closest.closest()), omega, 
                    energy, closest.closestNormal());

                energy = energy * li * 2 * M_PI;

                // Add photons to light source
                get<1>(t).push_back(p);
                get<0>(t)->count++;
                
                photonTraceRay(sc, out, t, photons, total, bounces + 1);
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

        int times = photons * (get<0>(t)->power.getLuminance() / luminanceTotal); 
    
        if ( get<0>(t)->count < times ) {
            Ray r = get<0>(t)->sample();

            photonTraceRay(sc, r, t, photons, total, 0);
        }
    }
}

RGB closestPhoton(const Ray& r, const Scene& sc, const PhotonMap& pmap){
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

        auto nearestPhotons = pmap.nearest_neighbors(r(closest.closest()), 1, 0.01);

        if ( !nearestPhotons.empty() ) {
            return nearestPhotons[0]->flux;
        }
    }

    return RGB();
}

Image renderPhotonMap(const Scene& sc, const PhotonMap& pmap) {

    Image img(sc.getProps().viewportWidth, sc.getProps().viewportHeight);

    double nextval = 0.0, progress;
    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            RGB contrib = RGB();

            auto rays = sc.cam.perPixel(j, i, sc.getProps().antialiasingFactor);

            for ( const Ray& r : rays ) {
                contrib = contrib + closestPhoton(r, sc, pmap);
            }
            img.imageData[i][j] = contrib / (double)rays.size() * 4000; 
            progress = double( i * img.width + j ) / double(img.height * img.width);
            if ( progress >= nextval ) {
                nextval += 0.01;
                cout << setprecision(3) << progress << " % .. " << flush;
            }
        }
    }

    return img;
}

RGB densityEstimation(const Vector3& x, const Intersection& it, const vector<const Photon*>& closestPhotons) {
    RGB color = RGB();
    double rk = -INFINITY;
    for (auto photon : closestPhotons) {
        auto a = x - photon->position_;
        rk = max(a.modulus(), rk);
    }

    for (auto photon : closestPhotons){
        
        auto result = it.brdf->sample(photon->inDirection, x, it);
            
        // Check if ray was absorbed
        if ( result.has_value() ) {

            auto [ omega, li ] = result.value();
            
            color = color + li * (photon->flux / (M_PI * rk * rk));
        }
    }

    return color;
}

struct PixelResult
{
    unsigned int x, y;
    RGB contribution;
};

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

                for ( const Ray& r : rays ) {

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

                        auto nearestPhotons = pmap.nearest_neighbors(r(closest.closest()), 500);

                        if ( !nearestPhotons.empty() ) {
                            contrib = contrib + densityEstimation(r(closest.closest()), closest, nearestPhotons);
                        }
                    }
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
                cout << setprecision(3) << progress << " % .. " << flush;
            }

            PixelResult res = tp.getResult();
            img.imageData[res.y][res.x] = res.contribution;
            //cout << "i = " << i << ", j = " << j << endl;

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
    
    while ( true ){
        photonTrace(sc, lightPhotons, total, maxPhotons);
        
        int totalPhotons = 0;
        for ( auto& t : lightPhotons ) {
            totalPhotons += get<0>(t)->count;
        }

        if ( totalPhotons >= maxPhotons ) 
            break;
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

    cout << "Generating photon map...";
    // Photon Map
    PhotonMap pmap(photons, PhotonAxisPosition());
    cout << "done." << endl;

    // Density estimation
    auto L = render(sc, photons);
    // render photon map directly
    // return renderPhotonMap(sc, pmap);

    return L;

}


Image getPhotonMap(const Scene& sc, const unsigned int total, const unsigned int maxPhotons) {
    
    auto lights = sc.lights.size();
    vector<LightPhotonPair> lightPhotons(lights);
    transform(sc.lights.begin(), sc.lights.end(), lightPhotons.begin(), [](shared_ptr<Light> l) -> LightPhotonPair {
        return make_tuple(l, list<Photon>());
    });
    
    while ( true ){
        photonTrace(sc, lightPhotons, total, maxPhotons);
        
        int totalPhotons = 0;
        for ( auto& t : lightPhotons ) {
            totalPhotons += get<0>(t)->count;
        }

        if ( totalPhotons >= maxPhotons ) 
            break;
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

    cout << "Generating photon map...";
    // Photon Map
    PhotonMap pmap(photons, PhotonAxisPosition());
    cout << "done." << endl;

    // render photon map directly
    return renderPhotonMap(sc, pmap);
}

