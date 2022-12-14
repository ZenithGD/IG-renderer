#include <pathtracer/pathtracing.hpp>

#include <cmath>
#include <functional>
#include <random>
#include <iomanip>

#include <acceleration/threadpool.hpp>
#include <material/BRDF.hpp>

/**
 * @brief Estimate direct light contribution on a point given the observation
 * direction and intersection data.
 * 
 * @param sc The scene
 * @param origin The point of intersection
 * @param obsDirection The observation direction
 * @param it The intersection data
 * @return RGB The direct light contribution
 */
RGB nextEventEstimation(const Scene& sc, const Vector3 origin, 
    const Vector3& obsDirection, const Intersection& it) {
    RGB totalContrib;
    for (auto l : sc.lights)
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

        for (auto& p : sc.primitives)
        {

            Intersection inter = p->intersection(r, 0, directRayDir.modulus());
            if (inter.intersects && inter.closest() < closestT)
            {
                closest = inter;
                closestT = inter.closest();
            }
        }

        // If the shadow ray doesn't intersect with anything, add contribution
        if (!closest.intersects)
        {
            double geometryContrib = abs(dot(it.closestNormal(), normalize(directRayDir)));

            RGB materialContrib = it.brdf->eval(r(it.closest()), directRayDir, r.direction, it) / M_PI;

            RGB lightContrib = l->power / dot(directRayDir, directRayDir) * materialContrib * geometryContrib;
            totalContrib = totalContrib + lightContrib;
        }
    }

    return totalContrib;
}

/**
 * @brief Perform path tracing up to n bounces starting with ray r on the scene.
 * 
 * @param sc The scene
 * @param r The ray
 * @param n The number of remaining bounces
 * @return RGB 
 */
RGB pathTraceRay(const Scene& sc, const Ray& r, unsigned int n) {
    
    //cout << "bounce " << n << endl;
    if( n > sc.getProps().bounces ) {
        throw std::runtime_error("what the hell");
    }
    
    RGB contrib;
    Intersection closest {
        .intersects = false,
    };

    double closestT = INFINITY;
    for ( auto p : sc.primitives ) {

        Intersection inter = p->intersection(r, 0);
        if ( inter.intersects && inter.closest() < closestT ){
            closest = inter;
            closestT = inter.closest();
        }
    }
    
    // trace direct light ray
    if( closest.intersects ) {
        if ( closest.brdf->emitter ) {
            // The object is an emitter, just add the emission value
            return closest.brdf->eval(Vector3(), Vector3(), Vector3(), Intersection{});
        } else {

            auto result = closest.brdf->sample(normalize(r.direction), r(closest.closest()), closest);
            
            // Check if ray was absorbed
            if ( !result.has_value() ) return RGB();

            auto [ omega, li, isDelta ] = result.value();
            Ray out(r(closest.closest()), omega);

            contrib = contrib 
                + nextEventEstimation(sc, r(closest.closest()), r.direction, closest);
                
            if ( n < sc.getProps().bounces ) {
                contrib = contrib + li * pathTraceRay(sc, out, n + 1);
            }   
        }
    } else {
        return sc.environment(r.direction);
    }

    //cout << "contrib final del bounce: " << n << "," << contrib << endl;
    return contrib;
}

struct PixelResult
{
    unsigned int x, y;
    RGB contribution;
};

Image pathTracing(const Scene& sc) {

    // task function should have pixel position and rays as arguments
    using TaskFn = std::function<PixelResult(void)>;

    Threadpool<TaskFn, PixelResult> tp(sc.getProps().threads);

    Image img(sc.getProps().viewportWidth, sc.getProps().viewportHeight);

    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            tp.enqueueTask([&, i, j]() -> PixelResult
            {
                RGB contrib;

                auto rays = sc.cam.perPixel(j, i, sc.getProps().antialiasingFactor);

                for ( const Ray& r : rays ) {
                    contrib = contrib + pathTraceRay(sc, r, 0);
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
                nextval += 0.01;
                cout << setprecision(3) << progress << " % .. " << flush;
            }

            PixelResult res = tp.getResult();

            img.imageData[res.y][res.x] = res.contribution;

            img.maxNumber = max({img.maxNumber, res.contribution.red, res.contribution.green, res.contribution.blue});
        }
    }
    return img;
}