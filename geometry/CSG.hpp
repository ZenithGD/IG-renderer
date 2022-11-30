#pragma once

#include <memory>

#include <geometry/primitive.hpp>
#include <geometry/ray.hpp>

using namespace std;

enum class CSGOperation {
    Union,
    Intersection,
    Difference
};

/**
 * @brief Class for representing a constructive solid geometry object.
 */
class CSG : public Primitive {
public:

    /**
     * @brief Construct a new CSG object
     * 
     * @param left The first object
     * @param right The second object
     * @param operation The operation applied to both solids
     * @param e The emission value.
     */
    CSG(const shared_ptr<Primitive>& l, const shared_ptr<Primitive>& r, CSGOperation op, const shared_ptr<BRDF>& b)
        : Primitive(brdf), operation(op), left(l), right(r) {}

    Intersection intersection(const Ray& r, double minT, double maxT) override;

private:
    
    CSGOperation operation;
    shared_ptr<Primitive> left, right;
};