#include "color.hpp"

ostream& operator<<(ostream& os, const Color& c) {
    os << c.toString();
    return os;
}

RGB equalizeClamp(const RGB& c, float v) {

    float r = c.red > v ? 1 : c.red / v; 
    float g = c.green > v ? 1 : c.green / v; 
    float b = c.blue > v ? 1 : c.blue / v; 

    return RGB(r, g, b);
}


RGB gammaCorrection(const RGB& c, float k, float gamma){
    float r = k * pow(c.red, gamma);
    float g = k * pow(c.green, gamma);
    float b = k * pow(c.blue, gamma);

    return RGB(r, g, b);
}