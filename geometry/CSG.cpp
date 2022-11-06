#include <geometry/CSG.hpp>
#include <functional>

/**
 * @brief Join two intersection objects.
 * 
 * @param i1 The first intersection object
 * @param i2 The second intersection object
 * @param operation The join operation.
 * @return Intersection 
 */
Intersection joinIntersectionObjects(const Intersection& i1, const Intersection& i2, 
    function<bool(bool, bool)> operation, const Vector3& rayDirection ) {

    bool in1 = dot(i1.intersections.begin()->second, rayDirection) < 0,
         in2 = dot(i2.intersections.begin()->second, rayDirection) < 0,
         prevIn = operation(in1, in2);

    Intersection intersection;

    auto it1 = i1.intersections.begin(), it2 = i2.intersections.begin();
    double t;
    Vector3 normal;

    while ( it1 != i1.intersections.end() && it2 != i2.intersections.end() ) {

        if ( it1->first < it2->first ) {
            t = it1->first;
            normal = it1->second;
            in1 = !in1;
            it1++;
        } else if ( it1->first > it2->first ) {
            t = it2->first;
            normal = it2->second;
            in2 = !in2;
            it2++;
        } else {
            // edge case, rarely found in practice
            t = it1->first;
            normal = it1->second;
            in1 = !in1;
            in2 = !in2;
            it1++; it2++;
        }

        bool in = operation(in1, in2);
        if ( in != prevIn ) {
            intersection.intersections.emplace(t, normal);
            prevIn = in;
        }
    }

    for ( it1; it1 != i1.intersections.end(); it1++ ) {
        t = it1->first;
        normal = it1->second;
        bool in = operation(in1, in2);
        if ( in != prevIn ) {
            intersection.intersections.emplace(t, normal);
            prevIn = in;
        }
    }

    for ( it2; it2 != i2.intersections.end(); it2++ ) {
        t = it2->first;
        normal = it2->second;
        bool in = operation(in1, in2);
        if ( in != prevIn ) {
            intersection.intersections.emplace(t, normal);
            prevIn = in;
        }
    }

    intersection.intersects = !intersection.intersections.empty();

    return intersection;
}

Intersection CSG::intersection(const Ray& r, double minT, double maxT) {

    Intersection i1 = left->intersection(r, minT, maxT);
    Intersection i2 = right->intersection(r, minT, maxT);

    Intersection it;

    // check op types
    switch ( operation ) {
        case CSGOperation::Union:
            it = joinIntersectionObjects(i1, i2, [](bool a, bool b) -> bool { return a || b; }, r.direction);
            break;
        case CSGOperation::Intersection:
            it = joinIntersectionObjects(i1, i2, [](bool a, bool b) -> bool { return a && b; }, r.direction);
            break;
        case CSGOperation::Difference:
            it = joinIntersectionObjects(i1, i2, [](bool a, bool b) -> bool { return a && !b; }, r.direction);
            break;
    }

    it.emission = emission;
    return it;
}
