#include <iostream>
#include <tuple>
#include <thread>

#include <getopt.h>
#include <scene/sceneSamples.hpp>
#include <image/tonemapping.hpp>
using namespace std;

tuple<string, SceneProps> getSceneProps(int argc, char **argv)
{

    SceneProps props{
        .viewportWidth = 256,
        .viewportHeight = 256,
        .antialiasingFactor = 64,
        .threads = thread::hardware_concurrency(),
        .bounces = 10
    };

    int c, digit_optind = 0;
    static struct option long_options[] = {
        {"width", required_argument, 0, 'w'},
        {"height", required_argument, 0, 't'},
        {"rays-per-pixel", required_argument, 0, 'r'},
        {"multithread", optional_argument, 0, 'm'},
        {"bounces", optional_argument, 0, 0},
        {"outfile", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    string outFile = "test";

    for (;;)
    {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;

        c = getopt_long(argc, argv, "w:t:o:m:r:",
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

        case 'h':
        case '?': // Unrecognized option
        default:
            cout << "Usage:" << endl;
            cout << "-w | --width (required): Width of the image (in pixels)" << endl;
            cout << "-h | --width (required): Height of the image (in pixels)" << endl;
            cout << "-o | --output (required): Image name" << endl;
            cout << "-m | --multithread (optional): Specify the number of threads manually. All cores by default" << endl;
            cout << "--rays-per-pixel (required): Number of rays per pixel" << endl;
            cout << "--bounces (optional): Max number of bounces per ray" << endl;
            exit( 1 );
            break;
        }
    }

    return { outFile, props };
}

int main(int argc, char **argv)
{
    auto [ outFile, props ] = getSceneProps(argc, argv);

    string ext = outFile.substr(outFile.find_last_of(".") + 1);
    if ( outFile.find_last_of(".") == string::npos ) {
        cout << "Specify the extension !" << endl;
        return 1;
    }
    
    if ( ext != "ppm" && ext != "bmp" ) {
        cout << "Unknown output format ! (" << ext << ")" << endl;
        return 1;
    }

    Scene sc = cornellCSGs(props);

    Image img(sc.getProps().viewportWidth, sc.getProps().viewportHeight);

    cout << "Rendering... " << flush;
    auto s = measureTime<std::chrono::milliseconds>(
        [&]()
        {
            img = sc.drawScene(
                [&](const Scene &sc) -> Image
                {
                    return pathTracing(sc);
                });
        });

    cout << "done (" << (double)s / 1000.0 << " s)." << endl;

    cout << "Tonemapping image..." << flush;
    
    img.maxNumber = img.getMaxNumber();
    //Image tmImg = tonemapping::extendedReinhard(img, 0.45, 0.001, 1);
    //Image tmImg = tonemapping::simpleReinhard(img, 0.18, 0.001);
    Image tmImg = tonemapping::gamma(img, 2.2);
    cout << "Writing image... " << flush;

    tmImg.maxNumber = tmImg.getMaxNumber();

    if ( ext == "bmp" ) tmImg.writeToBMP(outFile);
    else if ( ext == "ppm" ) tmImg.writeToPPM(outFile, 65535);

    cout << "done." << endl;
}