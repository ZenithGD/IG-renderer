#include <iostream>

#include <math/vector3.hpp>
#include <math/mat4.hpp>

using namespace std;

int main() {
    cout << "Hello" << endl;

    Vector3 v1(1, 1, 1), v2(2, 2, 2), v3(0, -1, 4);

    cout << v1 << "; " << v2 << endl;
    
    cout << "cross: " << cross(v1, v2) << endl;
    cout << "dot: " << dot(v1, v2) << endl;;
    
    cout << "suma: " << v1 + v2 << endl;
    cout << "resta: " << v1 - v2 << endl;
    cout << "multiplicacion: " << v1 * 5 << endl;
    cout << "division: "<< v2 / 2 << endl;

    cout << "modulo: " << v2.modulus() << endl;
    cout << "normalizar: " << v2.normalized() << endl;

    cout << "0 = " << dot(cross(v1, v3), v2) << endl;

    Mat4 m1, m2;

    m1[0][0] = 3;
    m1[1][1] = 2;
    m1[0][1] = 1;

    m2 = m1;

    cout << m1 * m1 << endl;

    cout << (m1 * m1).det() << endl;
    return 0;
}