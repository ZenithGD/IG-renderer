#include "scene.hpp"

void Scene::addPrimitive(const shared_ptr<Primitive> p){
    _primitives.push_back(p);    
}

Image Scene::drawScene(){
    
    //TODO: scene should carry all the info 
    //(camera frustum size, antialiasing factor and stuff)
    Image img(256, 256);
    for ( int i = 0; i < img.height; i++) {
        for ( int j = 0; j < img.width; j++ ) {
            auto rays = cam.perPixel(i, j, 64);

            RGB contrib;
            for ( Ray r : rays ) {
                Intersection closest;
                closest.t = INFINITY;
                for ( auto p : _primitives ) {

                    Intersection inter = p->intersection(r);
                    if ( inter.intersects && inter.t < closest.t ){
                        closest = inter;
                    }
                }

                Vector3 normal = closest.normal;
                //cout << "intersection " << closest.emission << endl;
                contrib = contrib + closest.emission * abs(dot(normalize(r.direction), normalize(normal)));
            }

            //cout << "total : " << contrib / (double)rays.size() << endl;
            img.imageData[i][j] = contrib / (double)rays.size();
        }
    }
    
    return img;
}