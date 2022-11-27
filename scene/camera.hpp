#pragma once 

#include <list>

#include <math/vector3.hpp>
#include <geometry/ray.hpp>
#include <random>

class Camera {
public:
    Vector3 left, up, forward, origin;
    int width, height;

    /**
     * @brief Construct a new Camera object
     * 
     * Note: l, u, f should make an orthogonal basis of R3. 
     * 
     * @param l The left vector
     * @param u The up vector
     * @param f The forward vector
     * @param o The camera's position
     * @param _w The width of the image
     * @param _h The height of the image
     */
    Camera(Vector3 l, Vector3 u, Vector3 f, Vector3 o, unsigned int _w, unsigned int _h)
        : left(l), up(u), forward(f), origin(o), width(_w), height(_h) {}

    /**
     * @brief Construct a new Camera object, parameterized by an origin,
     * target, field of view and focal length.
     * 
     * @param o The camera's position
     * @param t The camera's target
     * @param fov The field of view, in radians
     * @param focal The focal length
     */
    Camera(const Vector3 o, const Vector3 t, double fov, double focal);

    /**
     * @brief Scatter n rays through the pixel (x, y) on the camera plane
     * 
     * @param x The x position of the pixel on the camera plane
     * @param y The y position of the pixel on the camera plane
     * @param n The number of rays to scatter through the pixel
     * @return list<Ray> The list of created rays 
     */
    list<Ray> perPixel(unsigned int x, unsigned int y, unsigned int n) const;
};