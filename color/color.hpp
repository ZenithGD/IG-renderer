#pragma once 

#include <iostream>
#include <sstream>
using namespace std;

struct Color {

    /**
     * @brief Get the Luminance value
     * 
     * @return float 
     */
    virtual float getLuminance() const = 0;

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
    float red;
    float green;
    float blue;
    
    /**
     * @brief Construct a new RGB object
     * 
     * @param _red 
     * @param _green 
     * @param _blue 
     */
    inline RGB(float _red, float _green, float _blue) : red(_red), green(_green), blue(_blue) {}

    /**
     * @brief Construct a new RGB object
     * 
     */
    inline RGB() : red(0), green(0), blue(0) {}

    inline float getLuminance() const override { return 0.27 * red + 0.67 * green + 0.06 * blue; } 

    string toString() const override {
        ostringstream ss;

        ss << "R: " << red << ", G: " << green << ", B: " << blue << endl;

        return ss.str();
    }
};