#include <tests/testsuite.hpp>

#include <material/SimpleBRDF.hpp>
#include <geometry/ray.hpp>
#include <geometry/sphere.hpp>
#include <geometry/plane.hpp>
#include <geometry/CSG.hpp>

bool testUnionCSG() {
    
    Ray r(Vector3(0,0.3,-1), Vector3(0, 0, 1));

    auto b = make_shared<SimpleBRDF>(RGB(1,1,1));

    auto s1 = make_shared<Sphere>(Vector3(0, 0, 2), 1, b);
    auto s2 = make_shared<Sphere>(Vector3(0, 0, 3), 1, b);
    auto scsg = make_shared<CSG>(s2, s1, CSGOperation::Difference, b);

    Intersection i1 = s1->intersection(r, 0.001, INFINITY);
    Intersection i2 = s2->intersection(r, 0.001, INFINITY);

    Intersection it = scsg->intersection(r, 0.001, INFINITY);

    cout << i1 << endl;
    cout << i2 << endl;
    cout << it << endl;

    return it.intersects;
}


bool testSampleReflection() {
    
    auto b = make_shared<SimpleBRDF>(RGB(), RGB(0.9,0.9,0.9));

    Vector3 in(0, 1, -1);

    Sphere sp(Vector3(0,-0.5,0), 1, b);

    Ray r(in * -2, in);

    auto it = sp.intersection(r, 0, INFINITY);

    auto [ omega, c ] = b->sample(normalize(in), r(it.closest()), it).value();

    Ray r2(r(it.closest()), omega);

    cout << r << endl;
    
    cout << c << endl;

    cout << r2 << endl;
    
    return false;
}

bool testSampleRefraction() {
    
    auto b = make_shared<SimpleBRDF>(RGB(), RGB(), RGB(1, 1, 1), 1.5);

    Vector3 in(0,1, -1);

    Sphere sp(Vector3(0,-0.5,0), 1, b);

    Ray r(in * -2, in);

    auto it = sp.intersection(r, 0, INFINITY);

    auto [ omega, c ] = b->sample(normalize(in), r(it.closest()), it).value();

    Ray r2(r(it.closest()), omega);

    auto it2 = sp.intersection(r2, 0, INFINITY);

    auto [ omega2, c2 ] = b->sample(normalize(omega), r2(it2.closest()), it2).value();

    Ray r3(r2(it2.closest()), omega2);

    cout << r << endl;
    
    cout << c << endl;

    cout << r2 << endl;
    
    cout << c2 << endl;

    cout << r3 << endl;
    return false;
}

bool testSampleUniform() {
    auto b = make_shared<SimpleBRDF>(RGB(1,1,1));

    auto pl = make_shared<Plane>(1, Vector3(0, -1, 0), b);

    Ray r(Vector3(), Vector3(1, 1, 1));

    auto it = pl->intersection(r, 0, INFINITY);

    cout << "x,y,z" << endl;
    for ( int i = 0; i < 128; i++ ) {
        auto [ omega, c ] = b->sample(normalize(r(it.closest())), Vector3(1,1,1), it).value();
        cout << omega << endl;
    }

    return true;
}

int main() {
    
    TestSuite tCSG({
        Test(testUnionCSG, true),
        Test(testSampleReflection, true),
        Test(testSampleRefraction, true),
        Test(testSampleUniform, false),
    });

    int okCSG = tCSG.runSuite();
    //cout << okCSG << endl;
}