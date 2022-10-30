#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <color/color.hpp>

using namespace std;

struct Image {

    unsigned int width, height;
    double maxNumber;

    /**
     * @brief Construct a new Image object
     * 
     * @param w 
     * @param h 
     */
    Image(unsigned int w, unsigned int h) 
        :  width(w), height(h), imageData(h, vector<RGB>(w)), maxNumber(1) {}

    /**
     * @brief Construct a new Image object
     * 
     * @param w 
     * @param h 
     * @param data 
     */
    Image(unsigned int w, unsigned int h, vector<vector<RGB>> data, double max) 
        :  width(w), height(h), imageData(data), maxNumber(max) {}

    // PPM version P3 read-write

    /**
     * @brief Write image to the PPM file specified by path. 
     * The stored value for any value v will be computed as $v \cdot \dfrac{res}{max}$. 
     * @param path The image path
     * @param max The max color value to store.
     * @param res The color resolution
     */
    void writeToPPM(const string path, double max, unsigned int res = 255) const;

    /**
     * @brief Read PPM image.
     * The memory value for any value v read from the file will be computed as
     * $v \cdot \dfrac{max}{res}
     * @param path 
     * @return Image 
     */
    static Image readPPM(const string path);

    // BMP (LDR) read-write

    /**
     * @brief Write image to the PPM file specified by path.
     * Color depth is set to 24 bits, no compression applied, only one plane, 
     * 70 ppm on both axis by default.
     * 
     * @param path The image path 
     */
    void writeToBMP(const string path) const;

    /**
     * @brief 
     * 
     * @param path 
     * @return Image 
     */
    static Image readBMP(const string path);

    friend ostream& operator<<(ostream& os, const Image& image);

    vector<vector<RGB>> imageData;
};

ostream& operator<<(ostream& os, const Image& image);