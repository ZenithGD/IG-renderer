#include "image.hpp"

#include <regex>
#include <fstream>

void readHeader(ifstream& f) {
    string header;

    f >> header;
    if ( header != "P3"){
        throw runtime_error("Different format !!");
    }
}

Image Image::readPPM(string path){
    ifstream f(path);
    if(f.is_open()){
        readHeader(f);
        string line;
        getline(f, line);
        float maxNumber;

        smatch res;
        regex coment("#*");
        regex max("#MAX=[0-9,\.]+");

        while ( regex_match(line,res,coment) ){
            
            if(regex_match(line,res,max)){
                maxNumber = stoi(res[5].str());
            }
            
            getline(f, line);
        }
        unsigned int width, height;

        f >> width >> height;
        
        float maximunValue;

        f >> maximunValue;

        vector<vector<RGB>> data;
        float red,green,blue;

        for ( unsigned int i = 0; i < height; i++ ) {
            for ( unsigned int j = 0; j < width; j++ ) {
                f >> red >> green >> blue;
                RGB color(red,green,blue);

                data[i][j] = color;
            }
        }

        Image im(width, height, data);

        return im;
        
    } else {
        throw runtime_error("Can't read to " + path);
    }
}


void Image::writeToPPM(const string path, float max, unsigned int res) const {
    ofstream out(path);

    if ( out.is_open() ) {
        
        out << "P3" << endl;
        out << "#MAX=" << max << endl;
        out << "# " << path << endl;
        out << width << " " << height << endl;
        out << res << endl;
        for ( unsigned int i = 0; i < height; i++ ) {
            for ( unsigned int j = 0; j < width; j++ ) {

                int r = imageData[i][j].red * (float)res / max;
                int g = imageData[i][j].green * (float)res / max;
                int b = imageData[i][j].blue * (float)res / max;
                out << r << " " << g << " " << b << "\t"; 
            }
        }
    } else { 
        throw runtime_error("Can't write to " + path);
    }
};