#include <texture/texture.hpp>
#include <image/image.hpp>
#include <memory>

class ImageTexture : public Texture<RGB> {
public:

    enum WrapMode {
        NONE = 0,
        REPEAT,
        CLAMP,
        //MIRROR  // TODO
    };

    ImageTexture(const Image& image, double x = 1, double y = 1, WrapMode mode = NONE) 
        : img(image),
          xsize(x),
          ysize(y),
          wrapmode(mode)
    {}

    RGB sample(const double u, const double v, const Vector3& p) const override {
        int px = double(u * img.width) * xsize;
        int py = double(v * img.height) * ysize;

        unsigned int pxc;
        unsigned int pyc;

        if ( px < 0 || px >= img.width || py < 0 || py >= img.height ) {
            switch ( wrapmode ) {
                case NONE:
                    return RGB();
                break;
                case REPEAT:
                    return img.imageData[py % img.height][px % img.width];
                break;
                case CLAMP:
                    pxc = (unsigned int)clampInterval(px, 0, img.width);
                    pyc = (unsigned int)clampInterval(py, 0, img.height);
                    return img.imageData[pyc][pxc];
                break;
                default:
                    throw runtime_error("Invalid wrap mode type");
            }
        } else {
            return img.imageData[py][px];
        }
    }
private:
    // TODO: how can i store a texture here? 
    Image img;
    double xsize, ysize;
    WrapMode wrapmode;
};
