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

bool testSample() {
    
    BSDF b(RGB(0,1,1));

    for ( double th = 0; th <= 1; th += 0.2 ) {
        for ( double phi = 0; phi <= 1; phi += 0.2 ) {
            auto [ omega, c ] = b.sample(th, phi, Vector3(0,-1, 1), Vector3(1,1,1), Vector3(0,1,0));
            cout << "th: " << th << ", phi: " << phi << "; " << omega << endl;
        }
    }
    return false;
}

int main() {
    
    TestSuite tCSG({
        Test(testUnionCSG, true),
        Test(testSample, false)
    });

    int okCSG = tCSG.runSuite();
    cout << okCSG << endl;
}