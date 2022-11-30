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
