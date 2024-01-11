#pragma once
#include <qgraphicsitem.h>
#define TILE_SIZE			256

class TileItem :
	public QGraphicsPixmapItem
{
public:
	TileItem(int x, int y, int zoom);
	QPoint getMapPoint(QPoint pointInTile);			//瓦片内坐标对应的这个层级地图的坐标
	QPoint getTilePointFromMap(QPoint pointInMap);	//这个层级地图的坐标对应的瓦片内坐标
	int getX() { return m_x; };
	int getY() { return m_y; };
	int getZoom() { return m_zoom; };

	bool m_hasDownLoad;

private:
	void setTileIndex(int x, int y, int zoom);
	int m_x;
	int m_y;
	int m_zoom;

};

