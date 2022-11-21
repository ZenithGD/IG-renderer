#include "color.hpp"

ostream& operator<<(ostream& os, const Color& c) {
    os << c.toString();
    return os;
}

RGB equalizeClamp(const RGB& c, double v) {

    double r = c.red > v ? 1.0 : c.red / v; 
    double g = c.green > v ? 1.0 : c.green / v; 
    double b = c.blue > v ? 1.0 : c.blue / v; 

    return RGB(r, g, b);
}


RGB gammaCorrection(const RGB& c, double k, double gamma){
    double r = r > k ? k : (pow(c.red, gamma) / pow(k, gamma));
    double g = g > k ? k : (pow(c.green, gamma) / pow(k, gamma));
    double b = b > k ? k : (pow(c.blue, gamma) / pow(k, gamma));

    return RGB(r, g, b);
}

RGB extendedReinhard(const RGB& col, double maxWhite) {
    RGB num = col * ( col / (maxWhite * maxWhite) + 1 );
    RGB den = col + 1;

    return num / den;
}

RGB uncharted2_tonemap_partial(RGB x)
{
    double A = 0.15f;
    double B = 0.50f;
    double C = 0.10f;
    double D = 0.20f;
    double E = 0.02f;
    double F = 0.30f;
    return ((x*(x*A+C*B)+D*E)/(x*(x*A+B)+D*F)) - E/F;
}

RGB filmicUncharted(RGB v)
{
    double exposure_bias = 2.0;
    RGB curr = uncharted2_tonemap_partial(v * exposure_bias);

    RGB W = RGB(11.2, 11.2, 11.2);
    RGB white_scale = RGB(1.0, 1.0, 1.0) / uncharted2_tonemap_partial(W);
    return clamp(curr * white_scale, 1);
}