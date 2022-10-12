#include "tonemapping.hpp"

namespace tonemapping {

    Image clamp(const Image& image){

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

    Image equalize(const Image& image){
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

    Image gamma(const Image& image, double gamma){

        Image result = image;
        result.maxNumber = 1;
        for ( unsigned int i = 0; i < image.height; i++ ) {
            for ( unsigned int j = 0; j < image.width; j++ ) {
                RGB color = image.imageData[i][j];
                result.imageData[i][j] = gammaCorrection(color, image.maxNumber, gamma);
            }
        }
        return result;
    }

    Image equalizeClamp(const Image& image, double v) {
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

    Image gammaClamp(const Image& image, double k, double gamma) {

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

    Image simpleReinhard(const Image& image) {
        Image result = image;
        result.maxNumber = 1;
        for ( unsigned int i = 0; i < image.height; i++ ) {
            for ( unsigned int j = 0; j < image.width; j++ ) {
                RGB color = image.imageData[i][j];
                result.imageData[i][j] = simpleReinhard(color);
            }
        }
        return result;
    }

    Image extendedReinhard(const Image& image, double maxWhite) {
        Image result = image;
        result.maxNumber = 1;
        for ( unsigned int i = 0; i < image.height; i++ ) {
            for ( unsigned int j = 0; j < image.width; j++ ) {
                RGB color = image.imageData[i][j];
                result.imageData[i][j] = extendedReinhard(color, maxWhite);
            }
        }
        return result;
    }

    Image gaussianKernel(const Image& image, unsigned int kernelSize, double sigma) {
        
    }
}