#pragma once

#include <geometry/primitive.hpp>
#include <geometry/rectangle.hpp>
#include <material/BRDF.hpp>
#include <vector>
#include <memory>

class Box : public Primitive {
public:
    Box(const Vector3& p0, const Vector3& p1, const std::shared_ptr<BRDF>& brdf);

    Intersection intersection(const Ray& r, double minT, double maxT) override;
private:
    std::vector<std::shared_ptr<Primitive>> _sides; 
};