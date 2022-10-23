#include <iostream>
#include <fstream>
#include <cstring>

#include <image/image.hpp>
#include <color/color.hpp>
#include <image/tonemapping.hpp>

using namespace std;

const unsigned int N = 100;

void ppm(const string path) {
    cout << "reading file '" << path << "'..." << flush;
    Image im = Image::readPPM(path);
    cout << " done." << endl;

    cout << "tonemapping image..." << flush;
    Image imclamp = tonemapping::localReinhard(im, 0.18, 8, 0.35, 0.35 * 1.6, 0.05);
    cout << " done." << endl;
    
    cout << "saving image to '" << path << "reduced.ppm" << "'... ";
    im.writeToPPM(path + "raw.ppm", im.maxNumber, 65535);
    imclamp.writeToPPM(path + "reduced.ppm", imclamp.maxNumber, 65535);
    cout << "done." << endl;
}

void bmp(const string path) {
    cout << "reading file '" << path << "'..." << flush;
    Image im = Image::readPPM(path);
    cout << " done." << endl;

    cout << im << endl;

    cout << "tonemapping image..." << flush;
    Image imclamp = tonemapping::filmicUncharted(im);
    cout << " done." << endl;
    
    cout << "saving image to '" << path << "reduced.ppm" << "'... ";
    im.writeToBMP(path + "raw.bmp");
    imclamp.writeToBMP(path + "reduced.bmp");
    cout << "done." << endl;
}

int main(int argc, char** argv){

    if (argc < 2) {
        throw runtime_error("Please include a file to tonemap!");
    }

    if (argc < 3) {
        throw runtime_error("Please enter a file format to save {--bmp | --ppm}");
    }

    if ( strcmp(argv[1], "--bmp") == 0 ) {
        bmp(argv[2]);
    } else if ( strcmp(argv[1], "--ppm") == 0 ){
        ppm(argv[2]);
    }
}