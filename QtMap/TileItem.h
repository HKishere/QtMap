#pragma once
#include <qgraphicsitem.h>
class TileItem :
	public QGraphicsPixmapItem
{
public:
	void setTileIndex(int x, int y, int zoom);
	int getX() { return m_x; };
	int getY() { return m_y; };
	int getZoom() { return m_zoom; };

	bool m_hasDownLoad;

private:
	int m_x;
	int m_y;
	int m_zoom;

};

