#pragma once 
#include <math/vector3.hpp>
#include <color/color.hpp>
#include <geometry/ray.hpp>

using namespace std;

class Light {
public:
    RGB power;

    /**
     * @brief Construct a new Light object
     * 
     * @param p The light emission power
     */
    Light(RGB p) : power(p) {}

    /**
     * @brief Sample a random outbound ray from the light
     * 
     * @return Ray The outbound ray
     */
    virtual Ray sample() const = 0;
};

class PointLight : public Light {
public:
    Vector3 center;

    /**
     * @brief Construct a new Point Light object
     * 
     * @param c The center of the light
     * @param p The light emission power
     */
    PointLight(Vector3 c, RGB p) : Light(p), center(c) {}

    Ray sample() const override;
};

class AreaLight : public Light {
public:

    Ray sample() const override;
    
};