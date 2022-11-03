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

RGB Scene::directLight(const Intersection it, const Vector3 obsDirection) const
{
    RGB totalContrib;
    for (auto l : _lights)
    {
        Vector3 directRayDir = l->center - it.point;

        Ray r(it.point, directRayDir.normalized());

        Intersection closest {
            .intersects = false,
            .t = INFINITY
        };

        for (auto p : _primitives)
        {

            Intersection inter = p->intersection(r, 0.001, directRayDir.modulus());
            if (inter.intersects && inter.t < closest.t)
            {
                closest = inter;
            }
        }

        double geometryContrib = abs(dot(it.normal, directRayDir.normalized()));
        // TODO: more general BSDF object
        RGB materialContrib = it.emission / M_PI;

        RGB lightContrib = l->power / dot(directRayDir, directRayDir) * materialContrib * geometryContrib;

        // Once we know if the ray intersects something, compute distance to light source
        if (!closest.intersects)
        {
            totalContrib = totalContrib + lightContrib;
        }
    }

    return totalContrib;
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
                    Intersection closest {
                        .intersects = false,
                        .t = INFINITY
                    };
                    for ( auto p : _primitives ) {

                        Intersection inter = p->intersection(r, 0.001);
                        if ( inter.intersects && inter.t < closest.t ){
                            closest = inter;
                        }
                    }
                    
                    // trace direct light ray
                    if( closest.intersects ) {
                        contrib = contrib + directLight(closest, r.direction);
                    }
                }
                return PixelResult{ .x = j, .y = i, .contribution = contrib / (double)rays.size() }; 
            });
        }
    }

    double maxValue = 0;

    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            PixelResult res = tp.getResult();

            img.imageData[res.y][res.x] = res.contribution;

            img.maxNumber = max({img.maxNumber, res.contribution.red, res.contribution.green, res.contribution.blue});
        }
    }
    return img;
    /*
        RGB contrib;

        auto rays = cam.perPixel(j, i, _scprops.antialiasingFactor);

        for (const Ray r : rays)
        {
            Intersection closest;
            closest.t = INFINITY;
            for (auto p : _primitives)
            {

                Intersection inter = p->intersection(r);
                if (inter.intersects && inter.t < closest.t)
                {
                    closest = inter;
                }
            }
            // cout << "intersection " << closest.emission << endl;
            //  contrib = contrib + closest.emission * abs(dot(normalize(r.direction), normalize(normal)));

            // trazar rayo luz directa

            if ( closest.intersects ) {
                contrib = contrib + directLight(closest, r.direction);
            }
        }
        img.imageData[i][j] = contrib;
        */
}