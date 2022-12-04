#pragma once 

#include <color/color.hpp>
#include <math/vector3.hpp>
#include <geometry/intersection.hpp>
#include <geometry/ray.hpp>

#include <memory>

/**
 * @brief Base class for representing geometric primitives
 */
class Primitive {
public:
    // TODO : maybe separate brdf from actual primitive, useful for BVH to avoid
    // new box class (might be a bit cumbersome)
    shared_ptr<BRDF> brdf;

    /**
     * @brief Construct a new Primitive object
     * 
     * @param _e The emission value
     */
    Primitive(const shared_ptr<BRDF>& _b) : brdf(_b) {}; 

    /**
     * @brief Find an intersection with a ray.
     * 
     * @param r The ray
     * @return Intersection 
     */
    virtual Intersection intersection(const Ray& r, double minT, double maxT = INFINITY) = 0; 

    virtual ~Primitive() { }
};
