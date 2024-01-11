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
	pointInMap.setX(m_x * TILE_SIZE + pointInTile.x());
	pointInMap.setY(m_y * TILE_SIZE + pointInTile.y());

	return pointInMap;
}
QPoint TileItem::getTilePointFromMap(QPoint pointInMap)
{
	QPoint pointInTile;
	pointInTile.setX(pointInMap.x() % TILE_SIZE);
	pointInTile.setY(pointInMap.y() % TILE_SIZE);

	return pointInTile;
}
