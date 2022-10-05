#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <color/color.hpp>

using namespace std;

class Image {
public:

    unsigned int width, height;

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
    Image(unsigned int w, unsigned int h, vector<vector<RGB>> data) 
        :  width(w), height(h), imageData(data) {}

    void writeToPPM(string path, float res = 255.0) const;
    static Image readPPM(string path);

private:

    vector<vector<RGB>> imageData;
};