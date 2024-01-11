#include "MapGraphicsView.h"
#include <QDebug>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QEventLoop>
#include <QImageReader>
#include <QBuffer>
#include <QThread>
#include <chrono>
#include "CoordinateTransfer.h"

#define MAX_DOWNLOAD_NUM	20
#define SCREEN_WIDTH_1080P	1920
#define SCREEN_HEIGHT_1080P	1080

MapGraphicsView::MapGraphicsView(QWidget* parent)
	:QGraphicsView(parent)
{
	mapSizeX = SCREEN_WIDTH_1080P * 2 / TILE_SIZE;
	mapSizeY = SCREEN_HEIGHT_1080P * 2 / TILE_SIZE;
	m_zoom = 5;
	m_minZoom = 4;
	m_maxZoom = 17;

	oldOffsetX = 0;
	oldOffsetY = 0;

	//for (auto i = 0; i < mapSizeX * mapSizeY; i++)
	//{
	//	mapAreaList.append(new TileItem());
	//	mapAreaList.back()->setTileIndex(i % mapSizeX, i / mapSizeX, m_zoom);
	//	mapAreaList.back()->setPos(mapAreaList.back()->getX() * TILE_SIZE, mapAreaList.back()->getY() * TILE_SIZE);
	//	QPixmap tile_pic = QPixmap("defaultTile.png");
	//	mapAreaList.back()->setPixmap(tile_pic);
	//	m_scene.addItem(mapAreaList.back());
	//}

	for (int  i = 0; i < mapSizeX; i++)
	{
		for (int j = 0; j < mapSizeY; j++)
		{
			TileItem* tile = new TileItem(i, j, m_zoom);
			//tile->setTileIndex(i, j, m_zoom);
			tile->setPos(tile->getX() * TILE_SIZE, tile->getY() * TILE_SIZE);
			QPixmap tile_pic = QPixmap("defaultTile.png");
			tile->setPixmap(tile_pic);
			mapAreaMap.insert(std::make_pair(std::make_tuple(i, j, m_zoom), tile));
			m_scene.addItem(tile);
		}
	}

	setScene(&m_scene);
	show();
	tileInDownload = 0;
	theadStartFlag = true;
}

void MapGraphicsView::downloadMap()
{
	while (theadStartFlag)
	{
		//for (auto tileItem : mapAreaList)
		//{
		//	if (tileItem->m_hasDownLoad == false && tileInDownload < MAX_DOWNLOAD_NUM)
		//	{
		//		QMutexLocker locker(&mutex); // 自动加锁
		//		emit downloadTile(tileItem);
		//		tileInDownload++;
		//		tileItem->m_hasDownLoad = true;
		//	}
		//}
		for (auto tileItem : mapAreaMap)
		{
			if (tileItem.second->m_hasDownLoad == false && tileInDownload < MAX_DOWNLOAD_NUM)
			{
				QMutexLocker locker(&mutex); // 自动加锁
				emit downloadTile(tileItem.second);
				tileInDownload++;
				tileItem.second->m_hasDownLoad = true;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}


MapGraphicsView::~MapGraphicsView()
{
	theadStartFlag = false;
	tCheckTileDownload.join();
}

void MapGraphicsView::init()
{
	tCheckTileDownload = std::thread(&MapGraphicsView::downloadMap, this);
}

void MapGraphicsView::mouseReleaseEvent(QMouseEvent * event)
{
	// 处理鼠标点击事件
	if (event->button() == Qt::LeftButton) {
		QPointF scenePos = mapToScene(event->pos());
		qDebug() << "Mouse Pressed at scene position:" << scenePos;
		QSize viewsize = viewport()->size();
		TileItem* item = (TileItem*)itemAt(viewsize.width() / 2, viewsize.height() / 2);
		if (item != NULL)
		{
			reBuildMoveScene(item);
		}
	}

	// 将事件传递给基类以确保其他处理得以执行
	QGraphicsView::mousePressEvent(event);

}


void MapGraphicsView::wheelEvent(QWheelEvent * event)
{
	// 获取鼠标滚轮的滚动距离（以1/8度为单位）
	int delta = event->angleDelta().y();

	// 处理滚轮滚动事件
	if (delta > 0) {
		qDebug() << "Mouse wheel scrolled up";
		if (m_zoom <= m_maxZoom)
		{
			m_zoom++;
			QSize viewsize = viewport()->size();
			QPointF scenePos = mapToScene(viewsize.width() / 2, viewsize.height() / 2);
			//TileItem* item = (TileItem*)itemAt(viewsize.width() / 2, viewsize.height() / 2);
			TileItem* item = (TileItem*)m_scene.itemAt(scenePos, QTransform());
			QPointF pixmapItemCenter = item->mapFromScene(scenePos);
			qDebug() << "old center: X=" << scenePos.x() << "Y=" << scenePos.y();
			qDebug() << "old tile: X=" << item->getX() << "Y=" << item->getY();
			TileItem * nowitem = reBuildZoomSceen(item, QPoint(pixmapItemCenter.x(), pixmapItemCenter.y()), m_zoom);
			viewport()->update();
			centerOn(nowitem->getX() * TILE_SIZE , nowitem->getY() * TILE_SIZE);
			scenePos = mapToScene(viewsize.width() / 2, viewsize.height() / 2);
			qDebug() << "now center: X=" << scenePos.x() << "Y=" << scenePos.y();
			qDebug() << "now center tile: X=" << nowitem->getX() * TILE_SIZE << "Y=" << nowitem->getY() * TILE_SIZE;
		}
		// 在这里执行向上滚动的逻辑
	}
	else if (delta < 0) {
		qDebug() << "Mouse wheel scrolled down";
		if (m_zoom >= m_minZoom)
		{
			m_zoom--;
			QPointF scenePos = mapToScene(viewport()->rect().center());
			QSize viewsize = viewport()->size();
			TileItem* item = (TileItem*)m_scene.itemAt(scenePos, QTransform());
			QPointF pixmapItemCenter = item->mapFromScene(scenePos);
			TileItem * nowitem = reBuildZoomSceen(item, QPoint(pixmapItemCenter.x(), pixmapItemCenter.y()), m_zoom);

			centerOn(nowitem);

			qDebug() << "old tile: X=" << nowitem->getX() << "Y=" << nowitem->getY();
		}
		// 在这里执行向下滚动的逻辑
	}

	// 将事件传递给基类以确保其他处理得以执行
	//QGraphicsView::wheelEvent(event);
}

void MapGraphicsView::reBuildMoveScene(TileItem * centerTileItem)
{
	int nowCenterX = centerTileItem->getX();
	int nowCenterY = centerTileItem->getY();
	int Z = centerTileItem->getZoom();

	int offsetX = (nowCenterX - mapSizeX / 2) > 0 ? nowCenterX - mapSizeX / 2 : 0;
	int offsetY = (nowCenterY - mapSizeY / 2) > 0 ? nowCenterY - mapSizeY / 2 : 0;

	for (auto i = 0; i < mapSizeX * mapSizeY; i++)
	{
		int X = i % mapSizeX + offsetX;
		int Y = i / mapSizeX + offsetY;

		//if ((X >= oldOffsetX && X < oldOffsetX + mapSizeX) && (Y >= oldOffsetY && Y < oldOffsetY + mapSizeY))
		//{
		//	continue;
		//}

		auto tilePos = std::make_tuple(X, Y, Z);
		TileItem* tile;
		auto it = mapAreaMap.find(tilePos);
		if (it == mapAreaMap.end())
		{
			tile = new TileItem(X, Y, Z);
			//tile->setTileIndex(X, Y, Z);
			QPixmap tile_pic = QPixmap("defaultTile.png");
			tile->setPixmap(tile_pic);
			mapAreaMap.insert(std::make_pair(std::make_tuple(X, Y, Z), tile));

		}
		else
		{
			tile = it->second;
		}
		tile->setPos(tile->getX() * TILE_SIZE, tile->getY() * TILE_SIZE);
		m_scene.addItem(tile);

		if (i == mapSizeX * mapSizeY - 1)
		{
			m_scene.setSceneRect(0, 0, tile->getX() * TILE_SIZE, tile->getY() * TILE_SIZE);
		}
	}

	oldOffsetX = offsetX;
	oldOffsetY = offsetY;
}

TileItem * MapGraphicsView::reBuildZoomSceen(TileItem * centerTileItem, QPoint pointInTile, int nextZoomLev)
{
	doublePoint centerMorcator;
	QPoint centerMapPoint;
	QPoint centerTileIndex;

	centerMorcator = CoordinateTransfer::PointMapToMorcator(centerTileItem, pointInTile);
	centerMapPoint = CoordinateTransfer::PointMorcatorToMap(centerMorcator, nextZoomLev);
	centerTileIndex = CoordinateTransfer::MapPoint2TileIndex(centerMapPoint);

	auto tilePos = std::make_tuple(centerTileIndex.x(), centerTileIndex.y(), nextZoomLev);

	TileItem* tile = nullptr;

	for (auto it = mapAreaMap.begin(); it != mapAreaMap.end(); it++)
	{
		if (std::get<2>(it->first) != nextZoomLev && std::get<2>(it->first) == centerTileItem->getZoom())
		{
			m_scene.removeItem(it->second);
		}
	}

	if (mapAreaMap.find(tilePos) == mapAreaMap.end())
	{
		tile = new TileItem(centerTileIndex.x(), centerTileIndex.y(), nextZoomLev);
		//tile->setTileIndex(centerTileIndex.x(), centerTileIndex.y(), nextZoomLev);
		QPixmap tile_pic = QPixmap("defaultTile.png");
		tile->setPixmap(tile_pic);
		mapAreaMap.insert(std::make_pair(std::make_tuple(centerTileIndex.x(), centerTileIndex.y(), nextZoomLev), tile));
	}
	else
	{
		tile = mapAreaMap[tilePos];
	}
	tile->setPos(tile->getX() * TILE_SIZE, tile->getY() * TILE_SIZE);
	qDebug() << "new center: X=" << tile->getX() << "Y=" << tile->getY();
	qDebug() << "new center Pos: X=" << tile->getX() * TILE_SIZE << "Y=" << tile->getY() * TILE_SIZE;
	m_scene.addItem(tile);

	reBuildMoveScene(tile);
	return tile;
}

void MapGraphicsView::onTileDownloadFinish(TileItem* tileItem)
{
	QMutexLocker locker(&mutex); // 自动加锁
	viewport()->update();
	tileInDownload--;
}



IMapGraphicsView::IMapGraphicsView()
{
	m_strURL = "http://t0.tianditu.gov.cn/img_w/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=img&STYLE=default&TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX={z}&TILEROW={y}&TILECOL={x}&tk=b2385d12633488be653ed20a2d4999bd";
}


void IMapGraphicsView::onDownLoadTile(TileItem* tileItem)
{
	QString urlStr = m_strURL;

	// 使用正则表达式进行替换
	urlStr = urlStr.replace(QRegularExpression("\\{x\\}"), QString("%1").arg(tileItem->getX()));
	urlStr = urlStr.replace(QRegularExpression("\\{z\\}"), QString("%1").arg(tileItem->getZoom()));
	urlStr = urlStr.replace(QRegularExpression("\\{y\\}"), QString("%1").arg(tileItem->getY()));
	QNetworkAccessManager manager;
	// 下载瓦片
	QNetworkReply *reply = manager.get(QNetworkRequest(urlStr));
	QEventLoop loop;
	QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();
	if (reply->error() != QNetworkReply::NoError) {
		qDebug() << "Download tile failed: " << reply->errorString();
		return;
	}

	// 保存瓦片
	QByteArray data = reply->readAll();

	QBuffer buffer(&data);
	buffer.open(QIODevice::ReadOnly);

	// 将 QBuffer 与 QImageReader 关联
	QImageReader imageReader;
	imageReader.setDevice(&buffer);
	QImage image = imageReader.read();
	QPixmap tile_pic = QPixmap::fromImage(image);

	tileItem->setPixmap(tile_pic);

	emit TileDownloadFinish(tileItem);
}

void IMapGraphicsView::setSence(QGraphicsScene * scene)
{
	m_scene = scene;
}


