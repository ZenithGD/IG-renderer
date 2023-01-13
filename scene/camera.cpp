#include <scene/camera.hpp>
#include <math/coordinate.hpp>

list<Ray> Camera::perPixel(unsigned int x, unsigned int y, unsigned int n) const {
    
    list<Ray> listRay;

    Coordinate camBasis(left, up, forward, origin, 1);

    std::default_random_engine generator;
    uniform_real_distribution<double> rnd(0, 1);
    uniform_real_distribution<double> rndDepth(0, 0.02);

    for(int i = 0; i < n; i++){

        Vector3 rayFocus(0, 0, 1);
        Vector3 depthDisplace(rndDepth(generator), rndDepth(generator), 0);
 
        rayFocus.x = ((double)x + rnd(generator)) / (double)width * -2 + 1;
        rayFocus.y = ((double)y + rnd(generator)) / (double)height * -2 + 1;

        Coordinate raydirection(Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), rayFocus - depthDisplace, 0);
        Coordinate origin(Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), depthDisplace, 1);

        Coordinate newRayDirection = camBasis(raydirection);
        Coordinate newRayOrigin = camBasis(origin);

        Ray r(newRayOrigin.getPosition(), newRayDirection.getPosition().normalized());
        
        listRay.push_back(r);
        
        //listRay.emplace_back(origin, forwardNew);
    }

    return listRay;
}