#include "triangleMesh.hpp"

Intersection TriangleMesh::intersection(const Ray &r, double minT, double maxT)
{
    Intersection inter;
    Intersection result = _triangles[0] -> intersection(r,minT,maxT);

    for(int i = 1; i < _triangles.size(); i++){
        inter = _triangles[i] -> intersection(r,minT,maxT);

        if(inter.intersects){
           if(result.closest() > inter.closest()){
                result = inter;
           }
        }
    }

    return result;
}
