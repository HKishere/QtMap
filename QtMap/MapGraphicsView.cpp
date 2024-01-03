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
		QMutexLocker locker(&mutex); // �Զ�����
		emit downLoadTile(tileItem);
		tileInDownload++;
	}
}


void MapGraphicsView::mousePressEvent(QMouseEvent * event)
{
	// ����������¼�
	if (event->button() == Qt::LeftButton) {
		QPointF scenePos = mapToScene(event->pos());
		qDebug() << "Mouse Pressed at scene position:" << scenePos;
	}
	if (event->button() == Qt::MidButton)
	{

	}

	// ���¼����ݸ�������ȷ�������������ִ��
	QGraphicsView::mousePressEvent(event);

}

void MapGraphicsView::wheelEvent(QWheelEvent * event)
{
	// ��ȡ�����ֵĹ������루��1/8��Ϊ��λ��
	int delta = event->angleDelta().y();

	// ������ֹ����¼�
	if (delta > 0) {
		qDebug() << "Mouse wheel scrolled up";
		// ������ִ�����Ϲ������߼�
	}
	else if (delta < 0) {
		qDebug() << "Mouse wheel scrolled down";
		// ������ִ�����¹������߼�
	}

	// ���¼����ݸ�������ȷ�������������ִ��
	//QGraphicsView::wheelEvent(event);
}

void MapGraphicsView::onTileDownloadFinish(TileItem* tileItem)
{
	QMutexLocker locker(&mutex); // �Զ�����
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

	// ʹ��������ʽ�����滻
	urlStr = urlStr.replace(QRegularExpression("\\{x\\}"), QString("%1").arg(tileItem->getX()));
	urlStr = urlStr.replace(QRegularExpression("\\{z\\}"), QString("%1").arg(tileItem->getZoom()));
	urlStr = urlStr.replace(QRegularExpression("\\{y\\}"), QString("%1").arg(tileItem->getY()));
	QNetworkAccessManager manager;
	// ������Ƭ
	QNetworkReply *reply = manager.get(QNetworkRequest(urlStr));
	QEventLoop loop;
	QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();
	if (reply->error() != QNetworkReply::NoError) {
		qDebug() << "Download tile failed: " << reply->errorString();
		return;
	}

	// ������Ƭ
	QByteArray data = reply->readAll();

	QBuffer buffer(&data);
	buffer.open(QIODevice::ReadOnly);

	// �� QBuffer �� QImageReader ����
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
