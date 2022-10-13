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
    
        throw runtime_error("Can't read to " + path);
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
}

ostream& operator<<(ostream& os, const Image& image){
    os << "Image { "<< image.width << ", " << image.height << " }" << endl;
    return os;
}
