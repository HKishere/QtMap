#include "CoordinateTransfer.h"
#include <cmath>
#include <iostream>
#define M_PI				3.1415926

int CoordinateTransfer::long2tilex(double lon, int z) {
	return round(floor((lon + 180.0) / 360.0 * std::pow(2.0, z)));
}

int CoordinateTransfer::lat2tiley(double lat, int z) {
	return round(floor((1.0 - std::log(std::tan(lat * M_PI / 180.0) + 1.0 / std::cos(lat * M_PI / 180.0)) / M_PI) / 2.0 * std::pow(2.0, z)));
}

double CoordinateTransfer::tilex2long(int x, int z) {
	return x / std::pow(2.0, z) * 360.0 - 180;
}

double CoordinateTransfer::tiley2lat(int y, int z) {
	double n = M_PI - 2.0 * M_PI * y / std::pow(2.0, z);
	return 180.0 / M_PI * std::atan(0.5 * (std::exp(n) - std::exp(-n)));
}

