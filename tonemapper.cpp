#include <iostream>
#include <fstream>

#include <image/image.hpp>
#include <color/color.hpp>
#include <image/tonemapping.hpp>

using namespace std;

const unsigned int N = 100;

int main(int argc, char** argv){

    if (argc < 2) {
        throw runtime_error("The path must be included!");
    }
    
    Image im = Image::readPPM(string(argv[1]));

    Image imclamp = gammaClamp(im, 300, 1 / 2.2);
    //Image imclamp = clamping(im);
    imclamp.writeToPPM(string(argv[1]) + "reduced.ppm", imclamp.maxNumber, 255);
}