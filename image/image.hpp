#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <color/color.hpp>

using namespace std;

struct Image {

    unsigned int width, height;
    float maxNumber;

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
    Image(unsigned int w, unsigned int h, vector<vector<RGB>> data, float max) 
        :  width(w), height(h), imageData(data), maxNumber(max) {}

    void writeToPPM(const string path, float max, unsigned int res = 255) const;
    static Image readPPM(string path);

    friend ostream& operator<<(ostream& os, const Image& image);

    vector<vector<RGB>> imageData;
};

ostream& operator<<(ostream& os, const Image& image);