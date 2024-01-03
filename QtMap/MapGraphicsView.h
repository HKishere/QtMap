#pragma once
#include <qgraphicsview.h>
#include <QMouseEvent>
#include <QMutex>
#include "TileItem.h"

class MapGraphicsView :
	public QGraphicsView
{
	Q_OBJECT
public:
	MapGraphicsView(QWidget* parent);

	void mousePressEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

	void showMap();
	QGraphicsScene* getScene() { return &m_scene; };

signals:
	void downLoadTile(TileItem* tileItem);

public slots:
	void onTileDownloadFinish(TileItem* tileItem);

private:
	int mapSizeX;
	int mapSizeY;
	int zoom;
	QGraphicsScene m_scene;
	QList<TileItem*> mapAreaList;

	int tileInDownload;
	mutable QMutex mutex; // mutable�ؼ���������const��Ա�������޸ĸó�Ա
};

class IQGraphicsItemDemo : public QObject
{
	Q_OBJECT
public:
	IQGraphicsItemDemo();
	void setSence(QGraphicsScene* scene);

public slots:
	void onDownLoadTile(TileItem* tileItem);

signals:
	void TileDownloadFinish(TileItem* tileItem);

private:
	QGraphicsScene* m_scene;
	QString m_strURL;//urlģ�壬���е�x��y��z����ֵҪ��{x},{y},{z}����

};