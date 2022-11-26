#include <photonmapper/photonmapping.hpp>
#include <photonmapper/photon.hpp>
#include <acceleration/kdtree.hpp>

void photonTrace(const Scene& sc, const unsigned int photons, const unsigned int total) {

    using LightPhotonPair = tuple<shared_ptr<Light>, list<Photon>>;
    
    auto lights = sc._lights.size();
    double luminanceTotal = 0.0;
    for ( auto& l : sc._lights ) {
        luminanceTotal += l->power.getLuminance();
    }

    vector<LightPhotonPair> lightPhotons(lights);
    transform(sc._lights.begin(), sc._lights.end(), lightPhotons.begin(), [](shared_ptr<Light> l) -> LightPhotonPair {
        return make_tuple(l, list<Photon>());
    });



    for ( auto& t : lightPhotons ) {
    
       int times = photons * (get<0>(t)->power.getLuminance() / luminanceTotal);

       for ( int i = 0; i < times; i++) {
           
           Ray r = get<0>(t)->sample();
    
           Intersection closest {
               .intersects = false,
           };
    
           double closestT = INFINITY;
           for ( auto p : sc._primitives ) {
    
               Intersection inter = p->intersection(r, 0);
               if ( inter.intersects && inter.closest() < closestT ){
                   closest = inter;
                   closestT = inter.closest();
               }
           }
           
           // trace direct light ray
           if( closest.intersects ) {
    
               auto [ omega, li ] = closest.bsdf.sample(normalize(r.direction), r(closest.closest()), normalize(closest.closestNormal()));
               
               Ray out(r(closest.closest()), omega);
    
               //Photon p(r(closest.closest()), omega, );  
           }
    
                 
        }
    }
}

Image photonMapping(const Scene& sc, const unsigned int photons) {
    
}

