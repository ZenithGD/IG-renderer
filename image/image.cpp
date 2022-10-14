#include "image.hpp"

#include <regex>
#include <fstream>

void readHeader(ifstream& f) {
    string header;
    
    getline(f, header);

    if ( header != "P3"){
        throw runtime_error("Different format !!");
    }
}

Image Image::readPPM(const string path){
    ifstream f(path);

    if( !f.is_open() ){
    
        throw runtime_error("Can't read from " + path);
    }

    readHeader(f);
    string line;
    getline(f, line);
    double maxNumber;
    bool maxCommentFound = false;

    while ( line[0] == '#' ){
        
        if ( line.substr(0, 5) == "#MAX=" ) {
            maxCommentFound = true;
            maxNumber = atof(line.substr(5, string::npos).c_str());
        }
        
        getline(f, line);

    }

    unsigned int width, height;

    istringstream is(line);
    is >> width >> height;
    

    double resolution;

    f >> resolution;

    if ( !maxCommentFound ) maxNumber = resolution;

    vector<vector<RGB>> data(height, vector<RGB>(width));
    double red,green,blue;

    for ( unsigned int i = 0; i < height; i++ ) {
        for ( unsigned int j = 0; j < width; j++ ) {
            f >> red >> green >> blue;

            red *= maxNumber / resolution;
            green *= maxNumber / resolution;
            blue *= maxNumber / resolution;

            RGB color(red,green,blue);
            data[i][j] = color;
        }
    }

    Image im(width, height, data, maxNumber);

    return im;
}


void Image::writeToPPM(const string path, double max, unsigned int res) const {
    ofstream out(path);

    if ( !out.is_open() ) {
        throw runtime_error("Can't write to " + path);
    }

    out << "P3" << endl;
    out << "#MAX=" << max << endl;
    out << "# " << path << endl;
    out << width << " " << height << endl;
    out << res << endl;
    for ( unsigned int i = 0; i < height; i++ ) {
        for ( unsigned int j = 0; j < width; j++ ) {

            int r = imageData[i][j].red * (double)res / max;
            int g = imageData[i][j].green * (double)res / max;
            int b = imageData[i][j].blue * (double)res / max;
            out << r << " " << g << " " << b << "  "; 
        }
        out << endl;
    }
};

void Image::writeToBMP(const string path) const {

}

Image Image::readBMP(const string path) {
    ifstream in(path, ios::binary);

    if ( !in.is_open() ) {
        throw runtime_error("Can't read from '" + path + "'.");
    }

    /* Read header: 14 bytes */

    // Read BMP signature and check if it is "BM", otherwise break
    char signature[2];
    in.read(signature, 2);

    if ( strcmp(signature, "BM") != 0 ) {
        throw runtime_error("Invalid signature (" + string(signature) + ")");
    }

    // read file size
    uint32_t fileSize;
    in.read((char*) &fileSize, 4);

    // Ignore 4 reserved bytes
    in.ignore(4);

    uint32_t dataOffset;
    in.read((char *) &dataOffset, 4);

    /* Read info header: 40 bytes by default, read header size just in case */

    // Read header size
    uint32_t infoHeaderSize;
    in.read((char *) &infoHeaderSize, 4);

    // read width and height
    uint32_t width, height;
    in.read((char *) &width, 4);
    in.read((char *) &height, 4);

    // Ignore planes
    in.ignore(2);

    // Read bit count (log2 of color depth)
    uint16_t bitCount;
    in.read((char *) &bitCount, 2);

    // Ignore compression data ( assume it's type 0 - no compression )
    in.ignore(4);

    // Read raster data size
    uint32_t rasterSize;
    in.read((char *) &rasterSize, 4);

    // Ignore 16 bytes (x,y resolutions; )
    in.ignore(16);

    if ( bitCount <= 8 ) {
        // ignore palette for now
        in.ignore(4 * (1 << bitCount));
    }

    cout << "signature: " << string(signature) << endl;
    cout << "file size (B): " << fileSize << endl;
    cout << "data offset (B): " << dataOffset << endl;
    cout << "info header size (B): " << infoHeaderSize << endl;
    cout << "width, height (px): (" << width << ", " << height << ")" << endl;
    cout << "bits per pixel: " << bitCount << endl;

    // Ignore additional info header file (palettes and such) for now
    in.ignore(dataOffset - 54);

    Image img(width, height);
    img.maxNumber = 255;
    uint8_t* dataPtr = (uint8_t*)malloc((size_t)rasterSize);

    in.read((char*)dataPtr, rasterSize);

    unsigned int readIndex = 0;

    cout << "padding : " << (4 - (width * 3) % 4) % 4 << endl;
    unsigned int padding = (4 - (width * 3) % 4) % 4; 
    // assume bits per pixel is 24 for now
    for ( int i = height - 1; i >= 0; i-- ) {
        for ( int j = 0; j < width; j++ ) {
            uint8_t b = dataPtr[readIndex++];
            uint8_t g = dataPtr[readIndex++];
            uint8_t r = dataPtr[readIndex++];

            img.imageData[i][j] = RGB(r,g,b);
        }
        // skip padding
        readIndex += padding;
    }

    // free memory after loading image into object
    free(dataPtr);

    return img;
}

ostream& operator<<(ostream& os, const Image& image){
    os << "Image { w:"<< image.width << ", h:" << image.height << " }" << endl;
    return os;
}
