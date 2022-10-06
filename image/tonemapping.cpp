#include "tonemapping.hpp"

Image clamping(const Image& image){

    Image result = image;
    result.maxNumber = 1;
    for ( unsigned int i = 0; i < image.height; i++ ) {
        for ( unsigned int j = 0; j < image.width; j++ ) {
            RGB color = image.imageData[i][j];
            result.imageData[i][j] = clamp(color, 1);
        }
    }
    return result;
}

Image equalization(const Image& image){
    Image result = image;

    result.maxNumber = 1;

    for ( unsigned int i = 0; i < image.height; i++ ) {
        for ( unsigned int j = 0; j < image.width; j++ ) {
            RGB color = image.imageData[i][j];
            result.imageData[i][j] = equalize(color,image.maxNumber);
        }
    }
    return result;
    
}

Image gamma(const Image& image, float k, float gamma){

    Image result = image;
    result.maxNumber = 1;
    for ( unsigned int i = 0; i < image.height; i++ ) {
        for ( unsigned int j = 0; j < image.width; j++ ) {
            RGB color = image.imageData[i][j];
            result.imageData[i][j] = gammaCorrection(color, k, gamma);
        }
    }
    return result;
}

Image equalizeClamp(const Image& image, float v) {
    Image result = image;
    result.maxNumber = 1;
    for ( unsigned int i = 0; i < image.height; i++ ) {
        for ( unsigned int j = 0; j < image.width; j++ ) {
            RGB color = image.imageData[i][j];
            result.imageData[i][j] = equalizeClamp(color, v);
        }
    }
    return result;

}

Image gammaClamp(const Image& image, float v) {

    Image result = image;
    result.maxNumber = 1;
    for ( unsigned int i = 0; i < image.height; i++ ) {
        for ( unsigned int j = 0; j < image.width; j++ ) {
            RGB color = image.imageData[i][j];
        }
    }
}