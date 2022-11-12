#pragma once 

#include <color/color.hpp>
#include <math/vector3.hpp>
#include <geometry/intersection.hpp>
#include <geometry/ray.hpp>

/**
 * @brief Base class for representing geometric primitives
 */
class Primitive {
public:
    // TODO change to a material structure 
    BSDF bsdf;

    /**
     * @brief Construct a new Primitive object
     * 
     * @param _e The emission value
     */
    Primitive(BSDF _b = BSDF()) : bsdf(_b) {}; 

    /**
     * @brief Find an intersection with a ray.
     * 
     * @param r The ray
     * @return Intersection 
     */
    virtual Intersection intersection(const Ray& r, double minT, double maxT = INFINITY) = 0; 

    virtual ~Primitive() { }
};
