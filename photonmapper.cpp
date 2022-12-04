#include <iostream>

#include <thread>

#include <photonmapper/photonmapping.hpp>
#include <scene/sceneSamples.hpp>
#include <image/tonemapping.hpp>
using namespace std;

int main(int argc, char** argv) {
    
    SceneProps props {
        .viewportWidth = 300,
        .viewportHeight = 300,
        .antialiasingFactor = 2,
        .threads = std::thread::hardware_concurrency(),
        .bounces = 10
    };

    Scene sc = cornellDiffuse(props);

    Image img(props.viewportWidth, props.viewportHeight);
    
    cout << "Rendering... " << flush;
    auto ms = measureTime<std::chrono::milliseconds>( 
        [&](){ img = sc.drawScene(
            [&](const Scene& sc) -> Image { 
                return photonMapping(sc, 100000, 100000); 
            }
        );
    });

    cout << "done (" << ms << " ms)." << endl;

    cout << "Tonemapping image..." << flush;

    Image tmImg = tonemapping::gamma(img, 2.2);

    cout << "Writing image... " << flush;

    auto msimg = measureTime<std::chrono::milliseconds>( [&](){ tmImg.writeToBMP("test.bmp"); } );

    cout << "done (" << msimg << " ms)." << endl;
}