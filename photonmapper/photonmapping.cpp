#include <photonmapper/photonmapping.hpp>
#include <photonmapper/photon.hpp>
#include <acceleration/kdtree.hpp>
#include <color/color.hpp>

using PhotonMap = nn::KDTree<Photon, 3, PhotonAxisPosition>;
using LightPhotonPair = tuple<shared_ptr<Light>, list<Photon>>;

void photonTraceRay(const Scene& sc, const Ray& r, LightPhotonPair& t,
    const unsigned int photons, const unsigned int total) {
    
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

            auto [ omega, li ] = closest.brdf->sample(normalize(r.direction), r(closest.closest()), normalize(closest.closestNormal()));
            
            Ray out(r(closest.closest()), omega);

            Photon p(r(closest.closest()), omega, 4 * M_PI * get<0>(t)->power.getLuminance(), closest.closestNormal());

            // Add photons to light source
            get<1>(t).push_back(p);

            photonTraceRay(sc, out, t, photons, total);
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
    
        Ray r = get<0>(t)->sample();
    }
}

RGB nearbyPhotons(const Ray& r, const Scene& sc, const PhotonMap& pmap){
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

            auto [ omega, li ] = closest.brdf->sample(normalize(r.direction), r(closest.closest()), normalize(closest.closestNormal()));
            
            Ray out(r(closest.closest()), omega);

            auto nearestPhotons = pmap.nearest_neighbors(r(closest.closest()), 100, 0.1);
        }
}

Image photonMapping(const Scene& sc, const unsigned int total, const unsigned int maxPhotons) {

    using PhotonMap = nn::KDTree<Photon, 3, PhotonAxisPosition>;
    
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
        if ( totalPhotons > maxPhotons ) 
            break;
    }

    // Normalize every photon by the number of photons emitted by the light
    for ( auto& t : lightPhotons ) {
        for ( auto& p : get<1>(t) ) {
            p.flux /= get<0>(t)->count;
        }
    }


    list<Photon> photons;
    for ( auto& t : lightPhotons ) {
        copy(get<1>(t).begin(), get<1>(t).end(), back_insert_iterator<list<Photon>>(photons));
    }

    // Photon Map
    PhotonMap pmap(photons, PhotonAxisPosition());
    
    // Density estimation
    
}

