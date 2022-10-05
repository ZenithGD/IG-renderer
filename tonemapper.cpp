#include <iostream>
#include <fstream>

#include <image/image.hpp>
#include <color/color.hpp>

using namespace std;

const unsigned int N = 100;

int main(){

    std::vector<std::vector<RGB>> imageData(N, std::vector<RGB>(N));

    for(unsigned int i = 0; i < N; i++) {
        for(unsigned int j = 0; j < N; j++) {
            float r = (float)i / N;
            float g = (float)j / N;
            float b = (float)(i+j) / (float)(2 * N);
            imageData[i][j] = RGB(r,g,b);
        }
    }

    Image img(N, N, imageData);

    // real max 1, dynamic range 1E8
    img.writeToPPM("test.ppm", 1, 10000000);
}