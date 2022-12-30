#include "triangleMesh.hpp"

Intersection TriangleMesh::intersection(const Ray &r, double minT, double maxT)
{
    Intersection closest {
        .intersects = false
    };
    double closestT = INFINITY;

    for(int i = 0; i < _triangles.size(); i++){
        Intersection inter = _triangles[i] -> intersection(r,minT,maxT);

        if(inter.intersects && inter.closest() < closestT ){
            closest = inter;
            closestT = inter.closest();
        }
    }

    return closest;
}
