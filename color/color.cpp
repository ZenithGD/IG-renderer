#include "color.hpp"

ostream& operator<<(ostream& os, const Color& c) {
    os << c.toString();
    return os;
}

RGB equalizeClamp(const RGB& c, double v) {

    double r = c.red > v ? 1 : c.red / v; 
    double g = c.green > v ? 1 : c.green / v; 
    double b = c.blue > v ? 1 : c.blue / v; 

    return RGB(r, g, b);
}


RGB gammaCorrection(const RGB& c, double k, double gamma){
    double r = r > k ? k : (pow(c.red, gamma) / pow(k, gamma));
    double g = g > k ? k : (pow(c.green, gamma) / pow(k, gamma));
    double b = b > k ? k : (pow(c.blue, gamma) / pow(k, gamma));

    return RGB(r, g, b);
}