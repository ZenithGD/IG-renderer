#include <iostream>

#include <thread>
#include <getopt.h>

#include <photonmapper/photonmapping.hpp>
#include <scene/sceneSamples.hpp>
#include <image/tonemapping.hpp>
using namespace std;


tuple<string, SceneProps, PhotonConfig> getSceneProps(int argc, char **argv)
{
    SceneProps props{
        .viewportWidth = 256,
        .viewportHeight = 256,
        .antialiasingFactor = 64,
        .threads = thread::hardware_concurrency(),
        .bounces = 10
    };

    PhotonConfig photonConfig {
        .total = 50000,
        .maxPhotons = 50000,
        .bounces = 10,
        .photonBoost = 0.8,
        .causticBoost = 1.2,
        .nee = false
    };

    int c, digit_optind = 0;
    static struct option long_options[] = {
        {"width", required_argument, 0, 'w'},
        {"height", required_argument, 0, 't'},
        {"rays-per-pixel", required_argument, 0, 'r'},
        {"multithread", optional_argument, 0, 'm'},
        {"bounces", optional_argument, 0, 0},
        {"photons", required_argument, 0, 'n'},
        {"outfile", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    string outFile = "test";

    for (;;)
    {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;

        c = getopt_long(argc, argv, "w:t:o:m:r:n:",
                        long_options, &option_index);
        if (c == -1)
            break;

        switch (c)
        {

        case 'w':
            printf("Setting image width to %s\n", optarg);
            props.viewportWidth = strtol(optarg, nullptr, 10);
            break;

        case 't':
            printf("Setting image height to %s\n", optarg);
            props.viewportHeight = strtol(optarg, nullptr, 10);
            break;

        case 'o': 
            printf("option o with value '%s'\n", optarg);
            outFile = string(optarg);
            break;

        case 'm': 
            printf("option m with value '%s'\n", optarg);
            break;

        case 'r':
            printf("Setting rays per pixel to %s\n", optarg);
            props.antialiasingFactor = strtol(optarg, nullptr, 10);
            break;

        case 'n':
            printf("Setting max photons to %s", optarg);
            photonConfig.maxPhotons = strtol(optarg, nullptr, 10);
            photonConfig.total = strtol(optarg, nullptr, 10);
            break;

        case 'h':
        case '?': // Unrecognized option
        default:
            cout << "Usage:" << endl;
            cout << "--width (required): Width of the image (in pixels)" << endl;
            cout << "--width (required): Height of the image (in pixels)" << endl;
            cout << "--output (required): Image name" << endl;
            cout << "--multithread (optional): Specify the number of threads manually. All cores by default" << endl;
            cout << "--rays-per-pixel (required): Number of rays per pixel" << endl;
            cout << "--bounces (optional): Max number of bounces per ray" << endl;
            cout << "--photons: Max number of photons" << endl;
            exit( 1 );
            break;
        }
    }

    return { outFile, props, photonConfig };
}


int main(int argc, char** argv) {

    string outFile;
    SceneProps props;
    PhotonConfig photonConfig;
    std::tie(outFile, props, photonConfig) = [&]{ return getSceneProps(argc, argv); }();

    Scene sc = cornellMatVariations(props);

    Image img(props.viewportWidth, props.viewportHeight);

    cout << "Rendering... " << flush;
    auto ms = measureTime<std::chrono::milliseconds>( 
        [&, photonConfig](){ img = sc.drawScene(
            [&, photonConfig](const Scene& sc) -> Image { 
                return photonMapping(sc, photonConfig); 
            }
        );
    });

    cout << "done (" << ms << " ms)." << endl;
    img.maxNumber = img.getMaxNumber();

    cout << "Tonemapping image..." << flush;

    Image tmImg = tonemapping::gamma(img, 3);
    tmImg.maxNumber = tmImg.getMaxNumber();

    cout << "Writing image... " << flush;

    auto msimg = measureTime<std::chrono::milliseconds>( [&](){ tmImg.writeToBMP("test.bmp"); } );

    cout << "done (" << msimg << " ms)." << endl;
}