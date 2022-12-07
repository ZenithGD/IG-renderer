#pragma once

#include <vector>
#include <memory>
#include <functional>

#include <geometry/primitive.hpp>
#include <image/image.hpp>
#include <scene/camera.hpp>
#include <scene/light.hpp>
#include <texture/texture.hpp>

using namespace std;

struct SceneProps {
    uint32_t viewportWidth, viewportHeight;
    uint32_t antialiasingFactor;
    uint32_t threads;
    
    unsigned int bounces = 5;
};

class Scene {
public:
    // Rendering method describes how should the scene be rendered.
    using RenderMethod = function<Image(const Scene&)>;

    /**
     * @brief Construct a new Scene object
     * 
     * @param s The scene properties
     * @param c The main camera object
     */
    Scene(SceneProps s, Camera c, const shared_ptr<Texture<RGB>> tex = nullptr) : cam(c), _scprops(s), _envMap(tex) {};

    Camera cam;

    // TODO change to acceleration data structure
    vector<shared_ptr<Primitive>> primitives;
    vector<shared_ptr<Light>> lights;

    /**
     * @brief Add a new primitive to the scene
     * 
     * @param p A pointer to the new primitive
     */
    void addPrimitive(const shared_ptr<Primitive> p);

    /**
     * @brief Add a new light to the scene
     * 
     * @param p A pointer to the new light
     */
    void addLight(const shared_ptr<Light> l);

    /**
     * @brief Get the scene's properties
     * 
     * @return SceneProps The scene properties
     */
    inline SceneProps getProps() const { return _scprops; } 

    /**
     * @brief Render the image with a rendering method.
     * 
     * @param method The rendering method
     * @return Image The rendered image
     */
    inline Image drawScene(const RenderMethod& method) { return method(*this); }

    /**
     * @brief Get value from the enviroment map
     * 
     * @param direction The outgoing ray direction
     * @return RGB The sampled colour
     */
    RGB environment(const Vector3& direction) const;
    
private:
    SceneProps _scprops;
    shared_ptr<Texture<RGB>> _envMap;
};