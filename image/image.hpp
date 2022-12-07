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
        :  width(w), height(h), maxNumber(1), imageData(h, vector<RGB>(w)) {}

    /**
     * @brief Construct a new Image object
     * 
     * @param w 
     * @param h 
     * @param data 
     */
    Image(unsigned int w, unsigned int h, vector<vector<RGB>> data, double max) 
        :  width(w), height(h), maxNumber(max), imageData(data) {}

    // PPM version P3 read-write

    /**
     * @brief Write image to the PPM file specified by path. 
     * The stored value for any value v will be computed as $v \cdot \dfrac{res}{max}$. 
     * @param path The image path
     * @param max The max color value to store.
     * @param res The color resolution
     */
    void writeToPPM(const string& path, double max, unsigned int res = 255) const;

    /**
     * @brief Read PPM image.
     * The memory value for any value v read from the file will be computed as
     * $v \cdot \dfrac{max}{res}$.
     * @param path 
     * @return Image 
     */
    static Image readPPM(const string& path);

    // BMP (LDR) read-write

    /**
     * @brief Write image to the PPM file specified by path.
     * Color depth is set to 24 bits, no compression applied, only one plane, 
     * 70 ppm on both axis by default.
     * 
     * @param path The image path 
     */
    void writeToBMP(const string& path) const;

    /**
     * @brief Read BMP image.
     * 
     * @param path The image path
     * @return Image 
     */
    static Image readBMP(const string& path);

    // EXR (HDR) read-write

    /**
     * @brief Write image to the EXR file specified by path. 
     */
    void writeToEXR(const string& path) const;

    /**
     * @brief Read EXR image.
     * @param path 
     * @return Image 
     */
    static Image readEXR(const string& path);

    friend ostream& operator<<(ostream& os, const Image& image);

    vector<vector<RGB>> imageData;
};

/**
 * @brief Print general image infomation to an output stream
 * 
 * @param os The output stream
 * @param image The image object
 * @return ostream& The same output stream
 */
ostream& operator<<(ostream& os, const Image& image);