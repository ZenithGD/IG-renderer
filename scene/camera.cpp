#include <scene/camera.hpp>
#include <math/coordinate.hpp>

list<Ray> Camera::perPixel(unsigned int x, unsigned int y, unsigned int n){
    
    list<Ray> listRay;

    Coordinate camBasis(left, up, forward, origin, 1);

    std::default_random_engine generator;
    uniform_real_distribution<double> rnd(0, 1);

    for(int i = 0; i < n; i++){
        Vector3 forwardNew(0, 0, 1);
 
        forwardNew.x = ((double)x + rnd(generator)) / (double)width * -2 + 1;
        forwardNew.y = ((double)y + rnd(generator)) / (double)height * -2 + 1;

        Coordinate raydirection(Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1), forwardNew, 0);
        Coordinate origin(1);

        Coordinate newRayDirection = camBasis(raydirection);
        Coordinate newRayOrigin = camBasis(origin);

        Ray r(newRayOrigin.getPosition(), newRayDirection.getPosition());
        
        listRay.emplace_back(newRayOrigin.getPosition(), newRayDirection.getPosition());
        
        //listRay.emplace_back(origin, forwardNew);
    }

    return listRay;
}