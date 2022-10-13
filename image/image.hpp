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
        :  width(w), height(h), imageData(h, vector<RGB>(w)) {}

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
    void writeToPPM(const string path, double max, unsigned int res = 255) const;
    static Image readPPM(const string path);

    // BMP (LDR) read-write
    void writeToBMP(const string path) const;
    static Image readBMP(const string path);

    friend ostream& operator<<(ostream& os, const Image& image);

    vector<vector<RGB>> imageData;
};

ostream& operator<<(ostream& os, const Image& image);