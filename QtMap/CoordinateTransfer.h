#pragma once
#include <QRect>
#include <QPoint>
#include "TileItem.h"

#define HALF_WORLD_MAP_PERIMETER	20037508.3427892				//������ܳ���һ�루m��ī�����������ֵ
#define WORLD_MAP_PERIMETER			HALF_WORLD_MAP_PERIMETER * 2	//������ܳ���m��ī�����������ֵ
#define M_PI						3.1415926
#define MERCATOR_TOP				HALF_WORLD_MAP_PERIMETER		//���Ͻ���Ƭ�����Ͻ�Y����
#define MERCATOR_LEFT				-HALF_WORLD_MAP_PERIMETER		//���Ͻ���Ƭ�����Ͻ�X����
#define MAP_ZOOM_MAX				18								//zoom ���ֵ

struct doublePoint
{
	double x;
	double y;
};

class CoordinateTransfer
{
public:
	static int long2tilex(double lon, int z);
	static int lat2tiley(double lat, int z);
	static double tilex2long(int x, int z);
	static double tiley2lat(int y, int z);
	static doublePoint Tilexy2LonLat(const doublePoint & tilePoint, int zoom);
	static int UpdateGPSPositionInView(const doublePoint & GPSPositionPoint, int zoomLev, const QRect & rect);
	static doublePoint PointMapToMorcator(TileItem * item, QPoint point);
	static QPoint PointMorcatorToMap(doublePoint MorcatorPoint, int ZoomLev);
	static QPoint MapPoint2TileIndex(QPoint mapPoint);
	static doublePoint Mercator2lonLat(doublePoint mercator);
};
class FWgsToGcj {
public:
	const double a = 6378245.0;		
	const double ee = 0.00669342162296594323;

	double transformLat(const doublePoint & GpsPoint);
	double transformLon(const doublePoint & GpsPoint);
	int outOfChina(const doublePoint & GpsPoint);
	doublePoint wgs2gcj(const doublePoint & WgsGpsPoint);
	doublePoint gcj2wgs(const doublePoint & GcjGpsPoint);
};
