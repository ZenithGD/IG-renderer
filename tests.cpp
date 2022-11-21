#include <tests/testsuite.hpp>

#include <geometry/ray.hpp>
#include <geometry/sphere.hpp>
#include <geometry/CSG.hpp>

bool testUnionCSG() {
    
    Ray r(Vector3(0,0.3,-1), Vector3(0, 0, 1));

    auto s1 = make_shared<Sphere>(Vector3(0, 0, 2), 1, RGB(1, 1, 0));
    auto s2 = make_shared<Sphere>(Vector3(0, 0, 3), 1, RGB(0, 1, 1));
    auto scsg = make_shared<CSG>(s2, s1, CSGOperation::Difference, RGB(1, 1, 0));

    Intersection i1 = s1->intersection(r, 0.001, INFINITY);
    Intersection i2 = s2->intersection(r, 0.001, INFINITY);

    Intersection it = scsg->intersection(r, 0.001, INFINITY);

    cout << i1 << endl;
    cout << i2 << endl;
    cout << it << endl;

    return it.intersects;
}


bool testSampleReflection() {
    
    BSDF b(RGB(), RGB(0.9,0.9,0.9));

    Vector3 in(0, 1, -1);

    Sphere sp(Vector3(0,-0.5,0), 1, b);

    Ray r(in * -2, in);

    auto it = sp.intersection(r, 0, INFINITY);

    auto [ omega, c ] = b.sample(normalize(in), r(it.closest()), it.closestNormal());

    Ray r2(r(it.closest()), omega);

    cout << r << endl;
    
    cout << c << endl;

    cout << r2 << endl;
    
    return false;
}

bool testSampleRefraction() {
    
    BSDF b(RGB(), RGB(), RGB(1, 1, 1), 1.5);

    Vector3 in(0,1, -1);

    Sphere sp(Vector3(0,-0.5,0), 1, b);

    Ray r(in * -2, in);

    auto it = sp.intersection(r, 0, INFINITY);

    auto [ omega, c ] = b.sample(normalize(in), r(it.closest()), it.closestNormal());

    Ray r2(r(it.closest()), omega);

    auto it2 = sp.intersection(r2, 0, INFINITY);

    auto [ omega2, c2 ] = b.sample(normalize(omega), r2(it2.closest()), it2.closestNormal());

    Ray r3(r2(it2.closest()), omega2);

    cout << r << endl;
    
    cout << c << endl;

    cout << r2 << endl;
    
    cout << c2 << endl;

    cout << r3 << endl;
    return false;
}

bool testSampleUniform() {
    BSDF b(RGB(1,1,1));

    Vector3 in(0,-1, 1);

    cout << "x,y,z" << endl;
    for ( int i = 0; i < 64; i++ ) {
        auto [ omega, c ] = b.sample(normalize(in), Vector3(1,1,1), Vector3(0,0,1));
        cout << omega << endl;
    }

    return true;
}

int main() {
    
    TestSuite tCSG({
        Test(testUnionCSG, true),
        Test(testSampleReflection, false),
        Test(testSampleRefraction, false),
        Test(testSampleUniform, true),
    });

    int okCSG = tCSG.runSuite();
    //cout << okCSG << endl;
}