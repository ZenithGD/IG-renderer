#pragma once 

#include <iostream>
#include <memory>
#include <vector>

#include <math/vector3.hpp>
#include <geometry/ray.hpp>
#include <geometry/primitive.hpp>
#include <geometry/plane.hpp>
#include <material/BRDF.hpp>
#include <geometry/triangle.hpp>

using namespace std;

/**
 * @brief Class for representing Triangle
 * 
 */
class TriangleMesh : public Primitive {
public:
    TriangleMesh(const vector<Vector3>& vertices, const vector<unsigned int>& indices, 
        const vector<Vector3>& normals, const vector<Vector2>& uvCoords,
        const shared_ptr<BRDF>& brdf) : Primitive(brdf)
    {
        int ind = 0;
        for (int i = 0; i < indices.size(); i += 3){
            _triangles.push_back(make_shared<Triangle>(
                VertexInfo{vertices[indices[i]], normals[ind], uvCoords[indices[i]]},
                VertexInfo{vertices[indices[i + 1]], normals[ind], uvCoords[indices[i + 1]]},
                VertexInfo{vertices[indices[i + 2]], normals[ind], uvCoords[indices[i + 2]]},
                brdf
            ));
            ind++;
        }
    }

    Intersection intersection(const Ray& r, double minT, double maxT) override;

private:
    vector<shared_ptr<Triangle>> _triangles;
};