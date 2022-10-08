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
    
    cout << "reading file '" << argv[1] << "'..." << flush;
    Image im = Image::readPPM(string(argv[1]));
    cout << " done." << endl;

    cout << "tonemapping image..." << flush;
    Image imclamp = tonemapping::clamp(im);
    //Image imclamp = clamping(im);
    cout << " done." << endl;
    
    cout << "saving image to '" << argv[1] << "reduced.ppm" << "'..." << endl;
    imclamp.writeToPPM(string(argv[1]) + "reduced.ppm", imclamp.maxNumber, 10000);
}