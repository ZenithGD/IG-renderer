#include "scene.hpp"
#include <cmath>
#include <functional>

#include <acceleration/threadpool.hpp>

void Scene::addPrimitive(const shared_ptr<Primitive> p)
{
    _primitives.push_back(p);
}

void Scene::addLight(const shared_ptr<Light> l)
{
    _lights.push_back(l);
}

RGB Scene::nextEventEstimation(const Vector3 origin, const Vector3 obsDirection, const Intersection it) const
{
    RGB totalContrib;
    for (auto l : _lights)
    {
        auto lp = dynamic_pointer_cast<PointLight>(l);
        if ( lp == nullptr ) {
            continue;
        }
        
        Vector3 directRayDir = lp->center - origin;

        Ray r(origin, directRayDir.normalized());

        Intersection closest {
            .intersects = false
        };

        double closestT = INFINITY;

        for (auto p : _primitives)
        {

            Intersection inter = p->intersection(r, 0, directRayDir.modulus());
            if (inter.intersects && inter.closest() < closestT)
            {
                closest = inter;
                closestT = inter.closest();
            }
        }

        double geometryContrib = abs(dot(it.closestNormal(), directRayDir.normalized()));
        // TODO: more general BSDF object
        RGB materialContrib = it.bsdf.eval(r(it.closest()), obsDirection, r.direction) / M_PI;

        RGB lightContrib = l->power / dot(directRayDir, directRayDir) * materialContrib * geometryContrib;

        // Once we know if the ray intersects something, compute distance to light source
        if (!closest.intersects)
        {
            totalContrib = totalContrib + lightContrib;
        }
    }

    return totalContrib;
}

RGB Scene::pathTracing(const Ray& r, unsigned int n) const{
    
    if( n > _scprops.bounces ) {
        return RGB();
    }
    
    RGB contrib;
    Intersection closest {
        .intersects = false,
    };

    double closestT = INFINITY;
    for ( auto p : _primitives ) {

        Intersection inter = p->intersection(r, 0);
        if ( inter.intersects && inter.closest() < closestT ){
            closest = inter;
            closestT = inter.closest();
        }
    }
    
    // trace direct light ray
    if( closest.intersects ) {
        contrib = contrib + nextEventEstimation(r(closest.closest()), r.direction, closest);
    }

    
    double theta = M_PI/2, phi = M_PI/2;
    auto [ omega, li ] = closest.bsdf.sample(theta, phi, r.direction, r(closest.closest()), closest.closestNormal());
    
    contrib = contrib + pathTracing(r, n + 1);

    return contrib;
}

struct PixelResult
{
    unsigned int x, y;
    RGB contribution;
};

Image Scene::drawScene()
{

    // task function should have pixel position and rays as arguments
    using TaskFn = std::function<PixelResult(void)>;

    Threadpool<TaskFn, PixelResult> tp(_scprops.threads);

    Image img(_scprops.viewportWidth, _scprops.viewportHeight);

    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            tp.enqueueTask([&, i, j]() -> PixelResult
            {
                RGB contrib;

                auto rays = cam.perPixel(j, i, _scprops.antialiasingFactor);

                for ( const Ray& r : rays ) {
                    contrib = contrib + pathTracing(r,0);
                }
                return PixelResult{ .x = j, .y = i, .contribution = contrib / (double)rays.size() }; 
            });
        }
    }

    double maxValue = 0;
    double nextval = 0;

    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            double progress = double( i * img.width + j ) / double(img.height * img.width);
            if ( progress >= nextval ) {
                nextval += 0.05;
                cout << progress << " %" << endl;
            }

            PixelResult res = tp.getResult();

            img.imageData[res.y][res.x] = res.contribution;

            img.maxNumber = max({img.maxNumber, res.contribution.red, res.contribution.green, res.contribution.blue});
        }
    }
    return img;
}