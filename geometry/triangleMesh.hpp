#pragma once 

#include <iostream>

#include <math/vector3.hpp>
#include <geometry/ray.hpp>
#include <geometry/primitive.hpp>
#include <geometry/plane.hpp>

using namespace std;

/**
 * @brief Class for representing Triangle
 * 
 */
class TriangleMesh : public Primitive {
public:
    void addVertex(const Vector3& v);
    void addTriangle(const unsigned int)

private:
    vector<Vector3> _vertices;
    vector<unsigned int> _indices;
};