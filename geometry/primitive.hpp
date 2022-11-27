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
    BRDF brdf;

    /**
     * @brief Construct a new Primitive object
     * 
     * @param _e The emission value
     */
    Primitive(BRDF _b = BRDF()) : brdf(_b) {}; 

    /**
     * @brief Find an intersection with a ray.
     * 
     * @param r The ray
     * @return Intersection 
     */
    virtual Intersection intersection(const Ray& r, double minT, double maxT = INFINITY) = 0; 

    virtual ~Primitive() { }
};
