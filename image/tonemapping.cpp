#include "tonemapping.hpp"

#include <tuple>
#include <algorithm>

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
                result.imageData[i][j] = gammaCorrection(color, image.maxNumber, 1 / gamma);
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

    Image gammaClamp(const Image& image, double gamma) {

        Image result = image;
        result.maxNumber = 1;
        for ( unsigned int i = 0; i < image.height; i++ ) {
            for ( unsigned int j = 0; j < image.width; j++ ) {
                RGB color = image.imageData[i][j];
                result.imageData[i][j] = gammaClamp(color, image.maxNumber, 1 / gamma);
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

    Image filmicUncharted(const Image& image) {
        Image result = image;
        result.maxNumber = 1;
        for ( unsigned int i = 0; i < image.height; i++ ) {
            for ( unsigned int j = 0; j < image.width; j++ ) {
                RGB color = image.imageData[i][j];
                result.imageData[i][j] = filmicUncharted(color);
            }
        }
        return result;
    }

    /*** Local operator ***/

    double gaussian(unsigned int x, unsigned int y, double alpha, double scale) {

        double as2 = alpha * alpha * scale * scale;
        double f1 = 1.0 / (M_PI * as2);
        double f2 = exp(-x * x + y * y / as2);

        return f1 * f2;
    }

    // Convolution of the image with a gaussian kernel
    // TODO: precalculate kernel
    double computeV(const Image& image, unsigned int x, unsigned int y, double alpha, double scale) {

        double conv = 0;

        // round up
        int kernelSize = ceil(scale * alpha);

        for ( unsigned int i = std::max(0.0, ceil(y - kernelSize)); i < std::min(y + kernelSize, image.height); i++ )  {
            for ( unsigned int j = std::max(0.0, ceil(x - kernelSize)); j < std::min(x + kernelSize, image.width); j++ ) {
                conv += image.imageData[i][j].getLuminance() * gaussian(j - x, i - y, alpha, scale);
            }
        }

        return conv;
    }

    tuple<double, double, double> computeCircleSurround(const Image& image,
        unsigned int x, unsigned int y,
        double key, double sharp, double center, double surround, double scale) {

        // center < surround

        // cout << "Circle surround -> x : " << x << ", y : " << y << endl;

        double v1 = computeV(image, x, y, center, scale);
        double v2 = computeV(image, x, y, surround, scale);
        double v  = (v1 - v2) / (key * pow(2, sharp) / ( scale * scale ) + v1);
        return { v1, v2, v };
    }

    Image localReinhard(const Image& image,
        double key, double sharp, double center, double surround, double threshold) {

        Image result = image;
        result.maxNumber = 1;

        //double*** kernelPyramid = generateKernels(8, 1, 43, 1.6);
        
        for ( unsigned int i = 0; i < image.height; i++ ) {
            for ( unsigned int j = 0; j < image.width; j++ ) {
                RGB color = image.imageData[i][j];
                double v, v1, scale;
                
                // try scales from 1 to 43 by a 1.6 step
                for ( double s = 1; s < 43; s += 1.6 ) {
                    auto [ v1s, v2s, vs ] = computeCircleSurround(image, j, i, key, sharp, center, surround, s);
                    
                    if ( abs(vs) < threshold ) {
                        v1 = v1s;
                        v = vs;
                        scale = s;
                        break;
                    }
                }

                //cout << "Circle surround -> x : " << j << ", y : " << i << endl;
                //cout << "v1 : " << v1 << ", v : " << v << ", scale : " << scale << endl;

                double prevLuminance = image.imageData[i][j].getLuminance();
                double newLuminance = clampHigh(prevLuminance / ( v1 + 1 ), 1);

                result.imageData[i][j] = changeLuminance(result.imageData[i][j], newLuminance);
            }

            cout << "row " << i << endl; 
        }

        return result;
    }
}