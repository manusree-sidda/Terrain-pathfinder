
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>
#include "tr-functions.h"
using std::vector, std::string;
using std::cout, std::cin;
void getUserInput(std::string& filename, unsigned int& rows, unsigned int& cols) {
    // Update to guarantee filename is not an empty string and rows and cols are values from 1 to 4000
    cout << "Enter filename: ";
    cin >> filename;
    while (filename.empty()) {
        cout << "Invalid input, enter filename: ";
        cin >> filename;
    }
    cout << "Enter number of rows (1 - 4000): ";
    while (true) {
        if (cin >> rows && rows >= 1 && rows <= 4000) {
            break;
        } else {
            cout << "Invalid input, enter number of rows (1 - 4000): ";
            cin.clear();
            string temp;
            getline(cin, temp); 
        }
    }
    cout << "Enter number of columns (1 - 4000): ";
    while (true) {
        if (cin >> cols && cols >= 1 && cols <= 4000) {
            break;
        } else {
            cout << "Invalid input, enter number of columns (1 - 4000): ";
            cin.clear();
            string temp;
            getline(cin, temp); 
        }}}
vector<vector<unsigned int>> loadElevations(string filename, unsigned int rows, unsigned int cols) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::invalid_argument("loadElevations: Failed to open " + filename);
    }
    vector<vector<unsigned int>> elevations(rows, vector<unsigned int>(cols));
    std::string num;
    unsigned int count = 0;

    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int j = 0; j < cols; j++) {
            if (!(file >> num)) {  
                throw std::runtime_error("loadElevations: Not enough values");
            }
            std::istringstream iss(num);
            int val;
            if (!(iss >> val) || val < 0) {  
                throw std::runtime_error("loadElevations: Invalid elevation value");
            }

            elevations.at(i).at(j) = static_cast<unsigned int>(val);
            count++;
        }
    }
    if (file >> num) {  
        throw std::runtime_error("loadElevations: Too many values");
    }
    return elevations;
}
void findMinMax(const vector<vector<unsigned int>>& elevations, unsigned int& min, unsigned int& max) {
    
    if (elevations.empty() || elevations.at(0).empty()) {
        throw std::invalid_argument("findMinMax: Invalid elevations vector");
    }

    for (unsigned int i = 0; i < elevations.size(); i++) {
        if (elevations.at(i).empty()) {
            throw std::invalid_argument("findMinMax: Invalid elevations vector");
        }
    }
    max = elevations.at(0).at(0);
    min = elevations.at(0).at(0);
    for(unsigned int i =0; i < elevations.size(); i++){
        for(unsigned int j = 0; j < elevations.at(i).size(); j++ ){
            
            if(elevations.at(i).at(j) > max){
                max = elevations.at(i).at(j);
            }
            if(elevations.at(i).at(j) < min){
                min = elevations.at(i).at(j);
            }}}}
unsigned short scaleValue(unsigned int value, unsigned int min, unsigned int max) {

    if((min>max) || min == max){
        throw std::out_of_range("scaleValue: min must be less than max");
    }
    if((value< min || value>max)){
        throw std::out_of_range("scaleValue: value must be in the range min <= value <= max");
    }
    unsigned short scale = static_cast<unsigned short>(
        round(((value - min) / static_cast<double>(max - min)) * 255));
    return scale;
}
vector<vector<Pixel>> createImage(const vector<vector<unsigned int>>& elevations) {
    if (elevations.empty() || elevations.at(0).empty()) {
        throw std::invalid_argument("createImage: Invalid elevations vector");
    }
    unsigned int rows = elevations.size();
    unsigned int cols = elevations.at(0).size();
    for (unsigned int i = 0; i < rows; i++) {
        if (elevations.at(i).size() != cols) {
            throw std::invalid_argument("createImage: Invalid elevations vector");
        }
    }
    unsigned int min, max;
    findMinMax(elevations, min, max);
    vector<vector<Pixel>> image(cols, vector<Pixel>(rows));
    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int j = 0; j < cols; j++) {
            unsigned short grayShade = scaleValue(elevations.at(i).at(j), min, max);
            image.at(j).at(i).r = grayShade;
            image.at(j).at(i).g = grayShade;
            image.at(j).at(i).b = grayShade;
        }
    }
    return image;
}
void saveImage(string filename, const vector<vector<Pixel>>& image) {
    if(image.empty() || image.at(0).empty()){
        throw std::invalid_argument("saveImage: Invalid image vector");
    }
    for(unsigned int i = 0; i<image.size(); i++){
        if(image.at(i).empty()){
            throw std::invalid_argument("saveImage: Invalid image vector");
        }
    }
    std::ofstream file(filename + ".ppm");
    if(!file.is_open()){
        throw std::invalid_argument("saveImage: Failed to open " + filename);
    }
    unsigned int width = image.size();
    unsigned int height = image.at(0).size();
    file << "P3\n" << width << " " << height << "\n255\n";
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            file << image.at(j).at(i).r << " "
                 << image.at(j).at(i).g << " "
                 << image.at(j).at(i).b << " ";
        }
        file << "\n"; 
    }file.close();
}


void processRoutes(std::vector<std::vector<Pixel>>& image, const std::vector<std::vector<unsigned int>>& elevations, Pixel colorAll, Pixel colorShortest) { 
   
    if (elevations.empty() || elevations.at(0).empty()) {

        throw std::invalid_argument("processRoutes: Invalid elevations vector");
    }
    if (image.empty() || image.at(0).empty()) {

        throw std::invalid_argument("processRoutes: Invalid image vector");
    }
    if (colorAll.r > 255|| colorAll.g > 255 || colorAll.b > 255 ||colorShortest.r > 255 || colorShortest.g > 255 || colorShortest.b > 255) {

        throw std::invalid_argument("processRoutes: Invalid Pixel");
    }
    for(unsigned int i = 0; i<image.size(); i++){
        if(image.at(i).empty()){
            throw std::invalid_argument("processRoutes: Invalid image vector");
        }
    }
    for(unsigned int i = 0; i<elevations.size(); i++){
        if(elevations.at(i).empty()){
            throw std::invalid_argument("processRoutes: Invalid elevations vector");
        }
    }

    unsigned int rows = elevations.size();
    unsigned int minChange = std::numeric_limits<unsigned int>::max();
    unsigned int bestRow = 0;

    for (unsigned int i = 0; i < rows; i++) {
        unsigned int elevationChange = colorRoute(image, elevations, colorAll, i); //we get total elevation change from colorrout
        if (elevationChange < minChange) {
            minChange = elevationChange;
            bestRow = i;
        }
    }
    colorRoute(image, elevations, colorShortest, bestRow); //coloring the best route using colorshortest

}




unsigned int colorRoute(std::vector<std::vector<Pixel>>& image, const std::vector<std::vector<unsigned int>>& elevations, Pixel color, size_t startRow) { 

    if (elevations.empty() || elevations.at(0).empty()) {
    throw std::invalid_argument("colorRoute: Invalid elevations vector");
    }

    if (image.empty() || image.at(0).empty()) {
    throw std::invalid_argument("colorRoute: Invalid image vector");

    }
    if (color.r > 255 || color.g > 255 || color.b > 255) {

    throw std::invalid_argument("colorRoute: Invalid Pixel");
    }
    for(unsigned int i = 0; i<image.size(); i++){
            if(image.at(i).empty()){
                throw std::invalid_argument("colorRoute: Invalid image vector");
            }
        }
    for(unsigned int i = 0; i<elevations.size(); i++){
        if(elevations.at(i).empty()){
            throw std::invalid_argument("colorRoute: Invalid elevations vector");
        }
    }
    unsigned int rows = elevations.size();
    unsigned int cols = elevations.at(0).size();

    unsigned int row = startRow;
    unsigned int totalElevationChange = 0;

    for (unsigned int i = 0; i < cols - 1; i++) {
        image.at(i).at(row) = color;

        int currentElevation = elevations.at(row).at(i);

        int minChange = std::numeric_limits<int>::max();

        unsigned int bestMove = row;

        for (int d = -1; d <= 1; ++d) {
            unsigned int newRow = row + d;
            if (newRow < rows) {  
                int elevationChange = std::abs(static_cast<int>(currentElevation) - static_cast<int>(elevations.at(newRow).at(i+1)));

            if (elevationChange < minChange) {
                minChange = elevationChange;
                bestMove = newRow;
            } else if (elevationChange == minChange) {
            if (newRow > bestMove) {
                bestMove = newRow;
        }}}}

        totalElevationChange += minChange;
        row = bestMove;
    }

    image.at(cols-1).at(row) = color;
    return totalElevationChange;
}