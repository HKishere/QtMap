#include "MapGraphicsView.h"
#include <QDebug>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QEventLoop>
#include <QImageReader>
#include <QBuffer>
#include <QThread>

MapGraphicsView::MapGraphicsView(QWidget* parent)
	:QGraphicsView(parent)
{
	mapSizeX = 5;
	mapSizeY = 5;
	zoom = 5;

	for (auto i = 0; i < mapSizeX * mapSizeY; i++)
	{
		mapAreaList.append(new TileItem());
		mapAreaList.back()->setTileIndex(i % mapSizeX, i / mapSizeX, zoom);
	}
	setScene(&m_scene);
	show();
	tileInDownload = 0;
}

void MapGraphicsView::showMap()
{
	//int tileCount = qPow(2, tileSpec.zoom);
	for (auto tileItem : mapAreaList)
	{
		QMutexLocker locker(&mutex); // 自动加锁
		emit downLoadTile(tileItem);
		tileInDownload++;
	}
}


void MapGraphicsView::mousePressEvent(QMouseEvent * event)
{
	// 处理鼠标点击事件
	if (event->button() == Qt::LeftButton) {
		QPointF scenePos = mapToScene(event->pos());
		qDebug() << "Mouse Pressed at scene position:" << scenePos;
	}
	if (event->button() == Qt::MidButton)
	{

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
		// 在这里执行向上滚动的逻辑
	}
	else if (delta < 0) {
		qDebug() << "Mouse wheel scrolled down";
		// 在这里执行向下滚动的逻辑
	}

	// 将事件传递给基类以确保其他处理得以执行
	//QGraphicsView::wheelEvent(event);
}

void MapGraphicsView::onTileDownloadFinish(TileItem* tileItem)
{
	QMutexLocker locker(&mutex); // 自动加锁
	viewport()->update();
	tileInDownload--;
}



IQGraphicsItemDemo::IQGraphicsItemDemo()
{
	m_strURL = "http://t0.tianditu.gov.cn/img_c/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=img&STYLE=default&TILEMATRIXSET=c&FORMAT=tiles&TILEMATRIX={z}&TILEROW={y}&TILECOL={x}&tk=b2385d12633488be653ed20a2d4999bd";
}


void IQGraphicsItemDemo::onDownLoadTile(TileItem* tileItem)
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
	tileItem->setPos(tileItem->getX() * 256, tileItem->getY() * 256);
	m_scene->addItem(tileItem);

	emit TileDownloadFinish(tileItem);
}

void IQGraphicsItemDemo::setSence(QGraphicsScene * scene)
{
	m_scene = scene;
}
