#include "color.hpp"

ostream& operator<<(ostream& os, const Color& c) {
    os << c.toString();
    return os;
}
