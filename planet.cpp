#include <iostream>

#include <cmath>

#include <math/vector3.hpp>
#include <math/mat4.hpp>
#include <math/coordinate.hpp>

using namespace std;

class Planet
{

public:
    Planet(const Vector3 c, const Vector3 ax, const Vector3 ref)
        : center(c), yAxis(ax.normalized()), refCity(ref), radius(ax.modulus() / 2)
    {
        Vector3 refVec = refCity - center;
        if (abs(getRadius() - refVec.modulus()) > 10E-6)
        {
            throw logic_error("City is not on the surface of the planet");
        }

        // Compute planet axes
        xAxis = normalize(refVec - refVec.projection(yAxis));
        zAxis = normalize(cross(xAxis, yAxis));
    }

    inline float getRadius() const { return radius; }

    Coordinate planetCoordinates() const
    {

        return Coordinate(xAxis, yAxis, zAxis, center, 1);
    }

    Coordinate stationCoordinates(float inclination, float azimuth) const
    {
        Vector3 planetPos(
            getRadius() * sin(inclination) * cos(azimuth),
            getRadius() * sin(inclination) * sin(azimuth),
            getRadius() * cos(inclination));

        Coordinate c(Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), planetPos, 1);
        Coordinate pc = planetCoordinates();
        Coordinate UCSPlanetPos = pc(c);

        return UCSPlanetPos;
    }
    // UCS coordinates of the planet
    Vector3 center;

private:

    // Direction that conects the South Pole with the North Pole
    Vector3 xAxis, yAxis, zAxis;

    // Reference city position
    Vector3 refCity;

    float radius;
};

Vector3 getConnDirection(Planet p1, Planet p2) {
    return (p2.stationCoordinates(M_PI, M_PI).getPosition()) - (p1.stationCoordinates(M_PI, M_PI)).getPosition();
}

bool checkCollision(Planet p1, Planet p2) {
    Vector3 dir = getConnDirection(p1, p2);

    Vector3 p1StationPos = p1.stationCoordinates(M_PI, M_PI).getPosition();
    Vector3 normalP1 = p1StationPos - p1.center;

    Vector3 p2StationPos = p2.stationCoordinates(M_PI, M_PI).getPosition();
    Vector3 normalP2 = p2StationPos - p2.center;

    return ( dot(normalP1, dir) > 0 && dot(normalP2, dir) < 0 );
}

Planet getPlanet() {
    float x1, y1, z1;
    cout << "Input planet center: ";
    cin >> x1 >> y1 >> z1;

    float ax, ay, az;
    cout << "Input planet up axis: ";
    cin >> ax >> ay >> az;

    float rx, ry, rz;
    cout << "Input reference city coordinates: ";
    cin >> rx >> ry >> rz;

    return Planet(Vector3(x1, y1, z1), Vector3(ax, ay, az), Vector3(rx, ry, rz));
}

int main()
{
    
    Planet p1 = getPlanet();
    Planet p2 = getPlanet();
    
    Vector3 direction = (p2.stationCoordinates(M_PI, M_PI).getPosition()) - (p1.stationCoordinates(M_PI, M_PI)).getPosition();
    /*
    Planet p(
        Vector3(0, 1, 0),
        Vector3(0, 10, 0),
        Vector3(5, 1, 0));

    cout << "Planet coords: " << endl;
    cout << p.planetCoordinates() << endl;
    cout << "Station coords: " << endl;
    cout << p.stationCoordinates(M_PI / 2, M_PI / 2) << endl;
    */

    cout << direction << endl;
    cout << checkCollision(p1, p2) << endl;
    return 0;
}