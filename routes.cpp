
#include <iostream>
#include <vector>
#include "tr-functions.h"

using std::cin, std::cout, std::endl;
using std::vector, std::string, std::exception;

int main() {
    string filename;
    unsigned int numRows, numCols;
    numRows = 0;
    numCols = 0;
    

    try {
        getUserInput(filename, numRows, numCols);

        vector<vector<unsigned int>> elevations = loadElevations(filename, numRows, numCols);

        vector<vector<Pixel>> image = createImage(elevations);


        Pixel routeColor = {201, 121, 167};
        Pixel shortestColor = {0, 158, 115};

        processRoutes(image, elevations, routeColor, shortestColor);


        saveImage( filename, image);
    }
    catch (exception& e) {
        cout << e.what() << endl;
		return 1; // exit with an error
    }
}