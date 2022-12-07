#pragma once

#include <color/color.hpp>
#include <math/vector3.hpp>

template <typename E>
class Texture {
public:
    /**
     * @brief Sample a texture by its UV coordinate
     * 
     * @param u The u coordinate 
     * @param v The v coordinate
     * @param p The point in global space
     * @return E The colour value 
     */
    virtual E sample(const double u, const double v, const Vector3& p) const = 0;
};

