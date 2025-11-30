
#ifndef TRFUNCTIONS_H
#define TRFUNCTIONS_H

#include <vector>
#include <string>

struct Pixel {
    unsigned short r; // red
    unsigned short g; // green
    unsigned short b; // blue
};

// Part 1

void getUserInput(std::string& filename, unsigned int& rows, unsigned int& cols);

std::vector<std::vector<unsigned int>> loadElevations(std::string filename, unsigned int rows, unsigned int cols);

void findMinMax(const std::vector<std::vector<unsigned int>>& elevations, unsigned int& min, unsigned int& max);

unsigned short scaleValue(unsigned int value, unsigned int min, unsigned int max);

std::vector<std::vector<Pixel>> createImage(const std::vector<std::vector<unsigned int>>& elevations);
// throws exceptions

void saveImage(std::string filename, const std::vector<std::vector<Pixel>>& image);


void processRoutes(std::vector<std::vector<Pixel>>& image, const std::vector<std::vector<unsigned int>>& elevations, Pixel colorAll, Pixel colorShortest);

unsigned int colorRoute(std::vector<std::vector<Pixel>>& image, const std::vector<std::vector<unsigned int>>& elevations, Pixel color, size_t startRow);

#endif
