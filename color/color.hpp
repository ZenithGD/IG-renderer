#pragma once 

#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;

#include <math/misc.hpp>

struct Color {

    /**
     * @brief Get the Luminance value
     * 
     * @return double 
     */
    virtual double getLuminance() const = 0;

    /**
     * @brief Convert Color to String
     * 
     * @return string 
     */
    virtual string toString() const = 0;

    friend ostream& operator<<(ostream& os, const Color& c);
};

ostream& operator<<(ostream& os, const Color& c);

struct RGB : public Color {
    double red;
    double green;
    double blue;
    
    /**
     * @brief Construct a new RGB object
     * 
     * @param _red 
     * @param _green 
     * @param _blue 
     */
    inline RGB(double _red, double _green, double _blue) : red(_red), green(_green), blue(_blue) {}

    /**
     * @brief Construct a new RGB object
     * 
     */
    inline RGB() : red(0), green(0), blue(0) {}

    inline double getLuminance() const override { return 0.27 * red + 0.67 * green + 0.06 * blue; } 

    string toString() const override {
        ostringstream ss;

        ss << "R: " << red << ", G: " << green << ", B: " << blue << endl;

        return ss.str();
    }
};

inline RGB clamp(const RGB& c, double v) { return RGB(clampHigh(c.red, v), clampHigh(c.green, v), clampHigh(c.blue, v)); };
inline RGB equalize(const RGB& c, double v) { return RGB(c.red / v, c.green / v, c.blue / v); };
RGB equalizeClamp(const RGB& c, double v);
RGB gammaCorrection(const RGB& c, double k, double gamma);
RGB gammaClamp(const RGB& c, double k, double gamma);