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


void Image::writeToPPM(const string path, float res) const {
    ofstream out(path);

    if ( out.is_open() ) {
        
        out << "P3" << endl;
        out << "#MAX=" << res << endl;
        out << "# " << path << endl;
        out << width << " " << height << endl;

        for ( unsigned int i = 0; i < height; i++ ) {
            for ( unsigned int j = 0; j < width; j++ ) {
                out << imageData[i][j].red   * << " " 
                    << imageData[i][j].green * << " " 
                    << imageData[i][j].blue  << "\t"; 
            }
        }
    } else { 
        throw runtime_error("Can't write to " + path);
    }
};