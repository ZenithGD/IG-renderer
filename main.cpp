#include <iostream>

#include <cmath>

#include <math/vector3.hpp>
#include <math/mat4.hpp>
#include <math/coordinate.hpp>

using namespace std;

class Planet {

public:
    Planet(const Vector3 c, const Vector3 ax, const Vector3 ref)
        : center(c), yAxis(ax), refCity(ref)
    {
        Vector3 refVec = refCity - center;
        if ( abs(radius() - refVec.modulus()) > 10E-6 ) {
            throw logic_error("City is not on the surface of the planet");
        }

        // Compute planet axes
        xAxis = refVec - refVec.projection(yAxis);
        zAxis = cross(xAxis, yAxis);

        if ( abs(dot(xAxis, yAxis)) < 10E-6 ) {
            cout << "ok X,Y" << endl;
        }

        if ( abs(dot(xAxis, zAxis)) < 10E-6 ) {
            cout << "ok X,Z" << endl;
        }

        if ( abs(dot(zAxis, yAxis)) < 10E-6 ) {
            cout << "ok Z,Y" << endl;
        }
    }

    inline float radius() const { return yAxis.modulus() / 2; }

    Coordinate planetCoordinates() const {

        return Coordinate(xAxis, yAxis, zAxis, center);

    }

    Coordinate stationCoordinates(float inclination, float azimuth) const {
        Vector3 planetPos(
            radius()*sin(inclination)*cos(azimuth),
            radius()*sin(inclination)*sin(azimuth),
            radius()*cos(inclination));

        Coordinate c;
        Coordinate pc = planetCoordinates();
        Coordinate UCSPlanetPos = pc(c);
        
    }

    
    
private:
    // UCS coordinates of the planet
    Vector3 center;
    
    // Direction that conects the South Pole with the North Pole
    Vector3 xAxis, yAxis, zAxis;

    // Reference city position
    Vector3 refCity;
};


int main() {

    Planet p(
        Vector3(0,0,0),
        Vector3(0,1000,0),
        Vector3(-500, 0, 0),
        M_PI / 4,
        M_PI / 4
    );

    return 0;
}