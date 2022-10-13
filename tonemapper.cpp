#include <iostream>
#include <fstream>

#include <image/image.hpp>
#include <color/color.hpp>
#include <image/tonemapping.hpp>

using namespace std;

const unsigned int N = 100;

int main(int argc, char** argv){

    if (argc < 2) {
        throw runtime_error("Please include a file to tonemap!");
    }

    if (argc < 3) {
        throw runtime_error("Please enter a file format to save {--bmp | --ppm}");
    }
    
    cout << "reading file '" << argv[1] << "'..." << flush;
    Image im = Image::readPPM(string(argv[1]));
    cout << " done." << endl;

    cout << "tonemapping image..." << flush;
    Image imclamp = tonemapping::extendedReinhard(im, 40000);
    cout << " done." << endl;
    
    cout << "saving image to '" << argv[1] << "reduced.ppm" << "'..." << endl;
    im.writeToPPM(string(argv[1]) + "raw.ppm", im.maxNumber, 65535);
    imclamp.writeToPPM(string(argv[1]) + "reduced.ppm", imclamp.maxNumber, 65535);
}