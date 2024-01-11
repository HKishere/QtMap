#include "TileItem.h"

TileItem::TileItem(int x, int y, int zoom)
{
	m_hasDownLoad = false;
	setTileIndex(x, y, zoom);
}

void TileItem::setTileIndex(int x, int y, int zoom)
{
	m_x = x;
	m_y = y;
	m_zoom = zoom;
}

QPoint TileItem::getMapPoint(QPoint pointInTile)
{
	QPoint pointInMap;
	pointInMap.setX(m_x * 256 + pointInTile.x());
	pointInMap.setY(m_y * 256 + pointInTile.y());

	return pointInMap;
}
QPoint TileItem::getTilePointFromMap(QPoint pointInMap)
{
	QPoint pointInTile;
	pointInTile.setX(pointInMap.x() % 256);
	pointInTile.setY(pointInMap.y() % 256);

	return pointInTile;
}
