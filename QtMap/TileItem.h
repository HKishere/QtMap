#pragma once
#include <qgraphicsitem.h>
#define TILE_SIZE			256

class TileItem :
	public QGraphicsPixmapItem
{
public:
	TileItem(int x, int y, int zoom);
	QPoint getMapPoint(QPoint pointInTile);
	QPoint getTilePointFromMap(QPoint pointInMap);
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

