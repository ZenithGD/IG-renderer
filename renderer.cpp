#include <iostream>

#include <thread>

#include <getopt.h>
#include <scene/sceneSamples.hpp>
#include <image/tonemapping.hpp>
using namespace std;

SceneProps getSceneProps(int argc, char **argv)
{

    // Default properties
    SceneProps props{
        .viewportWidth = 256,
        .viewportHeight = 256,
        .antialiasingFactor = 32,
        .threads = thread::hardware_concurrency(),
        .bounces = 10};

    int c, digit_optind = 0;
    static struct option long_options[] = {
        {"width", required_argument, 0, 'w'},
        {"height", required_argument, 0, 't'},
        {"antialiasing", optional_argument, 0, 0},
        {"multithread", optional_argument, 0, 'm'},
        {"bounces", optional_argument, 0, 0},
        {"outfile", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    for (;;)
    {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;

        c = getopt_long(argc, argv, "w:t:o:m",
                        long_options, &option_index);
        if (c == -1)
            break;

        switch (c)
        {

        case 'w':
            printf("option w with value '%s'\n", optarg);
            props.viewportWidth = atoi(optarg);
            break;

        case 't':
            printf("option t with value '%s'\n", optarg);
            props.viewportHeight = atoi(optarg);
            break;

        case 'o': 
            printf("option o with value '%s'\n", optarg);
            break;

        case 'm': 
            printf("option m with value '%s'\n", optarg);
            break;

        case 'h':
        case '?': // Unrecognized option
        default:
            cout << "Usage:" << endl;
            cout << "-w | --width (required): Width of the image (in pixels)" << endl;
            cout << "-h | --width (required): Height of the image (in pixels)" << endl;
            cout << "-o | --output (required): Image name" << endl;
            cout << "-m | --multithread (optional): Specify the number of threads manually. All cores by default" << endl;
            cout << "--antialiasing (optional): Number of rays per pixel" << endl;
            cout << "--bounces (optional): Max number of bounces per ray" << endl;
            exit( 1 );
            break;
        }
        break;
    }

    return props;
}

int main(int argc, char **argv)
{

    // auto props = getSceneProps(argc, argv);

    // Default properties
    SceneProps props{
        .viewportWidth = 256,
        .viewportHeight = 256,
        .antialiasingFactor = 64,
        .threads = thread::hardware_concurrency(),
        .bounces = 100
    };

    Scene sc = cornellNoise(props);

    Image img(sc.getProps().viewportWidth, sc.getProps().viewportHeight);

    cout << "Rendering... " << flush;
    auto ms = measureTime<std::chrono::seconds>(
        [&]()
        {
            img = sc.drawScene(
                [&](const Scene &sc) -> Image
                {
                    return pathTracing(sc);
                });
        });

    cout << "done (" << ms << " ms)." << endl;

    cout << "Tonemapping image..." << flush;

    Image tmImg = tonemapping::gamma(img, 2.2);

    cout << "Writing image... " << flush;

    auto msimg = measureTime<std::chrono::milliseconds>(
        [&](){ tmImg.writeToBMP("test.bmp"); });

    cout << "done (" << msimg << " s)." << endl;
}