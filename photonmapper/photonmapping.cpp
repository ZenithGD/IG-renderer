#include <iomanip>

#include <photonmapper/photonmapping.hpp>
#include <photonmapper/photon.hpp>
#include <acceleration/kdtree.hpp>
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
    
    // trace direct light ray
    if( closest.intersects ) {

        auto [ omega, li ] = closest.brdf->sample(normalize(r.direction), r(closest.closest()), normalize(closest.closestNormal()));
        
        Ray out(r(closest.closest()), omega);

        Photon p(r(closest.closest()), omega, 
            4 * M_PI * get<0>(t)->power.getLuminance() * abs(dot(normalize(closest.closestNormal()), normalize(r.direction))), closest.closestNormal());

        // Add photons to light source
        get<1>(t).push_back(p);
        get<0>(t)->count++;
        
        photonTraceRay(sc, out, t, photons, total, bounces + 1);
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

        auto nearestPhotons = pmap.nearest_neighbors(r(closest.closest()), 1, 0.01);

        if ( !nearestPhotons.empty() ) {
            return RGB(nearestPhotons[0]->flux, nearestPhotons[0]->flux, nearestPhotons[0]->flux);
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
                contrib = contrib + nearbyPhotons(r, sc, pmap);
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
            if ( get<0>(t)->count > 0 )
                p.flux /= get<0>(t)->count;
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
    
    // render photon map directly
    return renderPhotonMap(sc, pmap);

}

