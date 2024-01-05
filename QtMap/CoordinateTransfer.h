#pragma once

struct MydoublePoint
{
	double x;
	double y;
};

class CoordinateTransfer
{
	static int long2tilex(double lon, int z);
	static int lat2tiley(double lat, int z);
	static double tilex2long(int x, int z);
	static double tiley2lat(int y, int z);
};

