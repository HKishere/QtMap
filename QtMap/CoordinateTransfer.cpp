#include "CoordinateTransfer.h"
#include <cmath>
#include <iostream>
#include "TileItem.h"

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

doublePoint CoordinateTransfer::Tilexy2LonLat(const doublePoint& tilePoint, int zoom) {
	doublePoint lonLat;
	lonLat.x = tiley2lat(tilePoint.y, zoom);
	lonLat.y = tilex2long(tilePoint.x, zoom);

	FWgsToGcj wgsToGcj;  // Assuming FWgsToGcj is a class with gcj2wgs method
	lonLat = wgsToGcj.gcj2wgs(lonLat);
	return lonLat;
}


int ZoomMapUpdate(const QPoint& m_ZoomViewPoint, int zDelta, int zoomNow) {
	int result = 0;

	//int tmpMapLevel = zoomNow;
	//QPoint m_ZoomMapPoint = PointDeviceToMap(m_ZoomViewPoint);
	//doublePoint m_ZoomMercatorPoint = PointMapToMorcator(m_ZoomMapPoint, MapTileState.ZoomLevel);

	// Determine the new map level

	//if (tmpMapLevel >= TMapTileState::MAPLEVELCOUNT || tmpMapLevel < 3)
	//	return result;

	//MapTileState.ZoomLevel = tmpMapLevel;

	//QPoint m_ZoomNewMapPoint = { 0, 0 };
	//m_ZoomNewMapPoint = PointMorcatorToMap(m_ZoomMercatorPoint, MapTileState.ZoomLevel);

	//QPoint m_ViewOriPointFromMap = { 0, 0 };
	//m_ViewOriPointFromMap.x = m_ZoomNewMapPoint.x - m_ZoomViewPoint.x;
	//m_ViewOriPointFromMap.y = m_ZoomNewMapPoint.y - m_ZoomViewPoint.y;

	//MapTileState.m_OnImageOriPointFromView.x = m_ViewOriPointFromMap.x % 256;
	//MapTileState.m_OnImageOriPointFromView.y = m_ViewOriPointFromMap.y % 256;
	//MapTileState.m_ViewOriPointOffsetFromMap.x = m_ViewOriPointFromMap.x - MapTileState.m_OnImageOriPointFromView.x;
	//MapTileState.m_ViewOriPointOffsetFromMap.y = m_ViewOriPointFromMap.y - MapTileState.m_OnImageOriPointFromView.y;

	//MapTileState.m_ViewOriPointInTile.x = MapTileState.m_ViewOriPointOffsetFromMap.x / 256;
	//MapTileState.m_ViewOriPointInTile.y = MapTileState.m_ViewOriPointOffsetFromMap.y / 256;

	result = 1;
	return result;
}

int CoordinateTransfer::UpdateGPSPositionInView(const doublePoint& GPSPositionPoint, int zoomLev, const QRect& rect) {
	int result = 0;

	//doublePoint tmpMapPoint = { 0.0, 0.0 };
	//doublePoint tmpPoint = { 0.0, 0.0 };

	//doublePoint tmpGpsPoint = { GPSPositionPoint.x, GPSPositionPoint.y };

	//if (1) {
	//	//if ((MapTileState.MapSrcSel == 0) ||
	//	//	((MapTileState.MapSrcSel == 1) && (MapTileState.MapModeSel == 1))) {
	//	//	tmpGpsPoint = FWgsToGcj.wgs2gcj(tmpGpsPoint);
	//	//}

	//	// 由 GPS (GCJ) 坐标转墨卡托坐标
	//	doublePoint tmpMercatorPoint = FMercatorTransfer.GPS2Mercator(tmpGpsPoint);

	//	// 由 (GCJ 的) 墨卡托坐标定位到地图像素坐标
	//	tmpMapPoint = PointMorcatorToMap(tmpMercatorPoint, zoomLev);

	//	// 先将滚动点视图坐标转换为地图像素坐标
	//	QPoint m_ZoomViewPoint = { rect.right / 2, rect.bottom / 2 };

	//	// 计算视图原点在新等级地图上坐标
	//	QPoint m_ViewOriPointFromMap = { tmpMapPoint.x - m_ZoomViewPoint.x, tmpMapPoint.y - m_ZoomViewPoint.y };

	//	// 计算视图原点所在瓦片原点相对于地图坐标
	//	MapTileState.m_OnImageOriPointFromView.x = m_ViewOriPointFromMap.x % 256;
	//	MapTileState.m_OnImageOriPointFromView.y = m_ViewOriPointFromMap.y % 256;
	//	MapTileState.m_ViewOriPointOffsetFromMap.x = m_ViewOriPointFromMap.x - MapTileState.m_OnImageOriPointFromView.x;
	//	MapTileState.m_ViewOriPointOffsetFromMap.y = m_ViewOriPointFromMap.y - MapTileState.m_OnImageOriPointFromView.y;

	//	MapTileState.m_ViewOriPointInTile.x = m_ViewOriPointFromMap.x / 256;
	//	MapTileState.m_ViewOriPointInTile.y = m_ViewOriPointFromMap.y / 256;

	//	MapTileState.ZoomLevel = zoomLev;
	//}
	//else if (m_FixedPointFlag > 2) {
	//	POINT FromPoint = PointGPSToDevice(GPSPositionPoint);
	//	POINT tmpMoveFromPoint = PointDeviceToMap(FromPoint);

	//	POINT ToPoint = { static_cast<long>(rect.right * 0.5), static_cast<long>(rect.bottom * 0.5) };
	//	POINT tmpMoveToPoint = { static_cast<long>(MapTileState.fMapPicture.m_MapOrigpoint.x
	//						   + (ToPoint.x * MapTileState.fMapPicture.m_tmpK)),
	//						   static_cast<long>(MapTileState.fMapPicture.m_MapOrigpoint.y
	//						   + (ToPoint.y * MapTileState.fMapPicture.m_tmpK)) };

	//	MapTileState.fMapPicture.m_MapOrigpoint.x -= (tmpMoveToPoint.x - tmpMoveFromPoint.x);
	//	MapTileState.fMapPicture.m_MapOrigpoint.y -= (tmpMoveToPoint.y - tmpMoveFromPoint.y);

	//	MapTileState.fMapPicture.m_FixedViewPointa.x = static_cast<long>((MapTileState.fMapPicture.m_FixedPointA.x
	//		- MapTileState.fMapPicture.m_MapOrigpoint.x) / MapTileState.fMapPicture.m_tmpK);
	//	MapTileState.fMapPicture.m_FixedViewPointa.y = static_cast<long>((MapTileState.fMapPicture.m_FixedPointA.y
	//		- MapTileState.fMapPicture.m_MapOrigpoint.y) / MapTileState.fMapPicture.m_tmpK);
	//	MapTileState.fMapPicture.m_FixedViewPointb.x = static_cast<long>((MapTileState.fMapPicture.m_FixedPointB.x
	//		- MapTileState.fMapPicture.m_MapOrigpoint.x) / MapTileState.fMapPicture.m_tmpK);
	//	MapTileState.fMapPicture.m_FixedViewPointb.y = static_cast<long>((MapTileState.fMapPicture.m_FixedPointB.y
	//		- MapTileState.fMapPicture.m_MapOrigpoint.y) / MapTileState.fMapPicture.m_tmpK);
	//}

	//result = 1;
	return result;
}

doublePoint CoordinateTransfer::PointMapToMorcator(TileItem* item, QPoint point) {
	doublePoint tmpMorcatorPoint = { 0.0, 0.0 };

	//if (MapTileState.OffLineMapFlag < 2) //未标定
	{
		if (item->getZoom() < MAP_ZOOM_MAX && item->getZoom() >= 0) {
			tmpMorcatorPoint.x = MERCATOR_LEFT + (item->getMapPoint(point).x()) * (WORLD_MAP_PERIMETER / static_cast<double>(TILE_SIZE) / pow(2.0, item->getZoom()));
			tmpMorcatorPoint.y = MERCATOR_TOP - (item->getMapPoint(point).y()) * (WORLD_MAP_PERIMETER / static_cast<double>(TILE_SIZE) / pow(2.0, item->getZoom()));
		}
	}
	//else if (m_FixedPointFlag > 2) {
	//	tmpMorcatorPoint.x = MapTileState.fMapPicture.m_FixedMercatorPointB.x
	//		- (MapTileState.fMapPicture.m_FixedPointB.x - point.x)
	//		* (MapTileState.fMapPicture.m_FixedMercatorPointB.x - MapTileState.fMapPicture.m_FixedMercatorPointA.x)
	//		/ (MapTileState.fMapPicture.m_FixedPointB.x - MapTileState.fMapPicture.m_FixedPointA.x);
	//	tmpMorcatorPoint.y = MapTileState.fMapPicture.m_FixedMercatorPointB.y
	//		- (MapTileState.fMapPicture.m_FixedPointB.y - point.y)
	//		* (MapTileState.fMapPicture.m_FixedMercatorPointB.y - MapTileState.fMapPicture.m_FixedMercatorPointA.y)
	//		/ (MapTileState.fMapPicture.m_FixedPointB.y - MapTileState.fMapPicture.m_FixedPointA.y);
	//}

	return tmpMorcatorPoint;
}
QPoint CoordinateTransfer::PointMorcatorToMap(doublePoint MorcatorPoint, int ZoomLev) {
	QPoint MapPoint;
	MapPoint.setX(0);
	MapPoint.setY(0);

	//if (MapTileState.OffLineMapFlag < 2) 
	{
		if (ZoomLev < MAP_ZOOM_MAX && ZoomLev >= 0) {
			doublePoint MorcatorOffset;
			MorcatorOffset.x = MorcatorPoint.x - MERCATOR_LEFT;
			MorcatorOffset.y = MERCATOR_TOP - MorcatorPoint.y;
			MapPoint.setX(round(MorcatorOffset.x / (WORLD_MAP_PERIMETER / static_cast<double>(TILE_SIZE) / pow(2.0, ZoomLev))));
			MapPoint.setY(round(MorcatorOffset.y / (WORLD_MAP_PERIMETER / static_cast<double>(TILE_SIZE) / pow(2.0, ZoomLev))));
		}
	}
	//else if (m_FixedPointFlag > 2) {
	//	MapPoint.x = round(MapTileState.fMapPicture.m_FixedPointB.x
	//		- (MapTileState.fMapPicture.m_FixedPointB.x - MapTileState.fMapPicture.m_FixedPointA.x)
	//		* (MapTileState.fMapPicture.m_FixedMercatorPointB.x - MorcatorPoint.x)
	//		/ (MapTileState.fMapPicture.m_FixedMercatorPointB.x - MapTileState.fMapPicture.m_FixedMercatorPointA.x));
	//	MapPoint.y = round(MapTileState.fMapPicture.m_FixedPointB.y
	//		- (MapTileState.fMapPicture.m_FixedPointB.y - MapTileState.fMapPicture.m_FixedPointA.y)
	//		* (MapTileState.fMapPicture.m_FixedMercatorPointB.y - MorcatorPoint.y)
	//		/ (MapTileState.fMapPicture.m_FixedMercatorPointB.y - MapTileState.fMapPicture.m_FixedMercatorPointA.y));
	//}

	return MapPoint;
}

QPoint CoordinateTransfer::MapPoint2TileIndex(QPoint mapPoint)
{
	QPoint TileIndex;
	TileIndex.setX(mapPoint.x() / TILE_SIZE);
	TileIndex.setY(mapPoint.y() / TILE_SIZE);

	return TileIndex;
}

doublePoint CoordinateTransfer::Mercator2lonLat(doublePoint mercator) {
	doublePoint lonLat;
	double x = mercator.x / 20037508.34 * 180;
	double y = mercator.y / 20037508.34 * 180;
	y = 180 / M_PI * (2 * atan(exp(y * M_PI / 180)) - M_PI / 2);
	lonLat.x = y;
	lonLat.y = x;

	return lonLat;
}

double FWgsToGcj::transformLat(const doublePoint& GpsPoint) {
	double ret = -100.0 + 2.0 * GpsPoint.x + 3.0 * GpsPoint.y + 0.2 * GpsPoint.y * GpsPoint.y +
		0.1 * GpsPoint.x * GpsPoint.y + 0.2 * std::sqrt(std::abs(GpsPoint.x));
	ret += (20.0 * std::sin(6.0 * GpsPoint.x * M_PI) + 20.0 * std::sin(2.0 * GpsPoint.x * M_PI)) * 2.0 / 3.0;
	ret += (20.0 * std::sin(GpsPoint.y * M_PI) + 40.0 * std::sin(GpsPoint.y / 3.0 * M_PI)) * 2.0 / 3.0;
	ret += (160.0 * std::sin(GpsPoint.y / 12.0 * M_PI) + 320 * std::sin(GpsPoint.y * M_PI / 30.0)) * 2.0 / 3.0;
	return ret;
}
double FWgsToGcj::transformLon(const doublePoint& GpsPoint) {
	double ret = 300.0 + GpsPoint.x + 2.0 * GpsPoint.y + 0.1 * GpsPoint.x * GpsPoint.x +
		0.1 * GpsPoint.x * GpsPoint.y + 0.1 * std::sqrt(std::abs(GpsPoint.x));
	ret += (20.0 * std::sin(6.0 * GpsPoint.x * M_PI) + 20.0 * std::sin(2.0 * GpsPoint.x * M_PI)) * 2.0 / 3.0;
	ret += (20.0 * std::sin(GpsPoint.x * M_PI) + 40.0 * std::sin(GpsPoint.x / 3.0 * M_PI)) * 2.0 / 3.0;
	ret += (150.0 * std::sin(GpsPoint.x / 12.0 * M_PI) + 300.0 * std::sin(GpsPoint.x / 30.0 * M_PI)) * 2.0 / 3.0;
	return ret;
}
int FWgsToGcj::outOfChina(const doublePoint& GpsPoint) {
	if (GpsPoint.y < 72.004 || GpsPoint.y > 137.8347 || GpsPoint.x < 0.8293 || GpsPoint.x > 55.8271) {
		return 1;
	}
	return 0;
}


doublePoint FWgsToGcj::wgs2gcj(const doublePoint& WgsGpsPoint) {
	double dLat = 0.0;
	double dLon = 0.0;
	double radLat = 0.0;
	double magic = 0.0;
	double sqrtMagic = 0.0;
	doublePoint GcjGpsPoint;

	if (outOfChina(WgsGpsPoint) != 0) {
		GcjGpsPoint.x = WgsGpsPoint.x;
		GcjGpsPoint.y = WgsGpsPoint.y;
		return GcjGpsPoint;
	}

	doublePoint tmpWgsGpsPoint = { WgsGpsPoint.y - 105.0, WgsGpsPoint.x - 35.0 };
	dLat = transformLat(tmpWgsGpsPoint);
	dLon = transformLon(tmpWgsGpsPoint);

	radLat = WgsGpsPoint.x / 180.0 * M_PI;
	magic = std::sin(radLat);
	magic = 1 - ee * magic * magic;

	sqrtMagic = std::sqrt(magic);
	dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * M_PI);
	dLon = (dLon * 180.0) / (a / sqrtMagic * std::cos(radLat) * M_PI);

	GcjGpsPoint.x = WgsGpsPoint.x + dLat;
	GcjGpsPoint.y = WgsGpsPoint.y + dLon;
	return GcjGpsPoint;
}

doublePoint FWgsToGcj::gcj2wgs(const doublePoint& GcjGpsPoint) {
	doublePoint tmpGcjGpsPoint = wgs2gcj(GcjGpsPoint);
	doublePoint WgsGpsPoint = { 2 * GcjGpsPoint.x - tmpGcjGpsPoint.x, 2 * GcjGpsPoint.y - tmpGcjGpsPoint.y };
	return WgsGpsPoint;
}
