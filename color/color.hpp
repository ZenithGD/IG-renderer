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

    /**
     * @brief Get the RGB colour's approximate luminance value
     * 
     * @return double The luminance value
     */
    inline double getLuminance() const override { return 0.27 * red + 0.67 * green + 0.06 * blue; } 

    /**
     * @brief Add two RGB colours together
     * The operation is done channel-wise
     * @param col The other colour
     * @return RGB 
     */
    inline RGB operator+(const RGB& col) const { return RGB(red + col.red, green + col.green, blue + col.blue); }

    /**
     * @brief Subtract two RGB colours together
     * The operation is done channel-wise
     * @param col The other colour
     * @return RGB 
     */
    inline RGB operator-(const RGB& col) const { return RGB(red - col.red, green - col.green, blue - col.blue); }

    /**
     * @brief Multiply two RGB colours together
     * The operation is done channel-wise
     * @param col The other colour
     * @return RGB 
     */
    inline RGB operator*(const RGB& col) const { return RGB(red * col.red, green * col.green, blue * col.blue); }

    /**
     * @brief Divide two RGB colours together
     * The operation is done channel-wise
     * @param col The other colour
     * @return RGB 
     */
    inline RGB operator/(const RGB& col) const { return RGB(red / col.red, green / col.green, blue / col.blue); }

    /**
     * @brief Add a scalar factor to an RGB color
     * The operation is done channel-wise
     * @param col The sum value
     * @return RGB 
     */
    inline RGB operator+(double f) const { return RGB(red + f, green + f, blue + f); }

    /**
     * @brief Subtract a scalar factor to an RGB color
     * The operation is done channel-wise
     * @param col The subtraction value
     * @return RGB 
     */
    inline RGB operator-(double f) const { return RGB(red - f, green - f, blue - f); }


     /**
     * @brief Scale up a colour by a scalar factor
     * 
     * @param col The scale-up factor
     * @return RGB 
     */
    inline RGB operator*(double f) const { return RGB(red * f, green * f, blue * f); }

    /**
     * @brief Scale down a colour by a scalar factor
     * 
     * @param col The scale-down factor
     * @return RGB 
     */
    inline RGB operator/(double f) const { return RGB(red / f, green / f, blue / f); }

    string toString() const override {
        ostringstream ss;

        ss << "R: " << red << ", G: " << green << ", B: " << blue;

        return ss.str();
    }
};

/**
 * @brief Get the maximum of the three colour channels.
 * 
 * @param r The colour object.
 * @return double The max channel value
 */
inline double maxChannel(const RGB& r) { return max(r.red, max(r.green, r.blue)); }

/**
 * @brief Return an equivalent colour with a new luminance value.
 * 
 * @param r The colour object.
 * @return double The scaled colour.
 */
inline RGB changeLuminance(const RGB& c, double outLuminance) { return c * ( outLuminance / c.getLuminance() ); }

inline RGB clamp(const RGB& c, double v) { return RGB(clampHigh(c.red, v), clampHigh(c.green, v), clampHigh(c.blue, v)); };
inline RGB equalize(const RGB& c, double v) { return RGB(c.red / v, c.green / v, c.blue / v); };
RGB equalizeClamp(const RGB& c, double v);
RGB gammaCorrection(const RGB& c, double k, double gamma);
RGB gammaClamp(const RGB& c, double k, double gamma);
inline RGB simpleReinhard(const RGB& col) { return col / ( col + 1 ); }
RGB extendedReinhard(const RGB& col, double maxWhite);
RGB filmicUncharted(const RGB& v);