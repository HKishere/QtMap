#pragma once
#include <qgraphicsview.h>
#include <QMouseEvent>
#include <QMutex>
#include "TileItem.h"
#include <thread>

class MapGraphicsView :
	public QGraphicsView
{
	Q_OBJECT
public:
	MapGraphicsView(QWidget* parent);
	virtual ~MapGraphicsView();
	void init();

	void mouseReleaseEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

	void reBuildScene(TileItem* centerTileItem);

	void downloadMap();
	QGraphicsScene* getScene() { return &m_scene; };

signals:
	void downloadTile(TileItem* tileItem);

public slots:
	void onTileDownloadFinish(TileItem* tileItem);

private:
	int mapSizeX;
	int mapSizeY;
	int zoom;

	int oldOffsetX;
	int oldOffsetY;
	QGraphicsScene m_scene;
	QList<TileItem*> mapAreaList;

	int tileInDownload;
	mutable QMutex mutex; // mutable关键字允许在const成员函数中修改该成员

	std::thread tCheckTileDownload;
	bool theadStartFlag;
};

class IMapGraphicsView : public QObject
{
	Q_OBJECT
public:
	IMapGraphicsView();
	void setSence(QGraphicsScene* scene);

public slots:
	void onDownLoadTile(TileItem* tileItem);

signals:
	void TileDownloadFinish(TileItem* tileItem);

private:
	QGraphicsScene* m_scene;
	QString m_strURL;//url模板，其中的x、y、z具体值要用{x},{y},{z}代替

};