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
    RGB emission;

    /**
     * @brief Construct a new Primitive object
     * 
     * @param _e The emission value
     */
    Primitive(RGB _e) : emission(_e) {}; 

    /**
     * @brief Find an intersection with a ray.
     * 
     * @param r The ray
     * @return Intersection 
     */
    virtual Intersection intersection(const Ray& r) = 0; 
};
