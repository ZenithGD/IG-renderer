#include <texture/texture.hpp>

class SolidColour : public Texture {
public:

    SolidColour(const RGB& r) : colour(r) {}

    RGB sample(const double u, const double v, const Vector3& p) const override {
        return colour;
    }
private:
    RGB colour;
};
