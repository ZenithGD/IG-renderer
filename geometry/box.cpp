#include "box.hpp"

Box::Box(const Vector3& p0, const Vector3& p1, const std::shared_ptr<BRDF>& brdf) 
    : Primitive(brdf) 
{
    _sides.push_back(std::make_shared<RectangleXY>(p0.x, p1.x, p0.y, p1.y, p1.z, brdf));
    _sides.push_back(std::make_shared<RectangleXY>(p0.x, p1.x, p0.y, p1.y, p0.z, brdf));

    _sides.push_back(std::make_shared<RectangleXZ>(p0.x, p1.x, p0.z, p1.z, p1.y, brdf));
    _sides.push_back(std::make_shared<RectangleXZ>(p0.x, p1.x, p0.z, p1.z, p0.y, brdf));

    _sides.push_back(std::make_shared<RectangleYZ>(p0.y, p1.y, p0.z, p1.z, p1.x, brdf));
    _sides.push_back(std::make_shared<RectangleYZ>(p0.y, p1.y, p0.z, p1.z, p0.x, brdf));
}

Intersection Box::intersection(const Ray& r, double minT, double maxT) {

    Intersection closest {
        .intersects = false
    };
    double closestT = INFINITY;

    for(int i = 0; i < _sides.size(); i++){
        Intersection inter = _sides[i] -> intersection(r,minT,maxT);

        if(inter.intersects && inter.closest() < closestT ){
            closest = inter;
            closestT = inter.closest();
        }
    }
    return closest;
}