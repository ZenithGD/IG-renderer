#include <geometry/intersection.hpp>

ostream& operator<<(ostream& os, const Intersection& it) {

    for ( auto& [t, normal] : it.intersections ) {
        cout << "{ " << t << ": " << normal << " }" << endl;
    }

    os << "Emission : " << it.emission;
    
    return os;
}