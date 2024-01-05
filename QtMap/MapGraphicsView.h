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

	void reBuildMoveScene(TileItem* centerTileItem);

	void reBuildZoomSceen();

	void downloadMap();
	QGraphicsScene* getScene() { return &m_scene; };

signals:
	void downloadTile(TileItem* tileItem);

public slots:
	void onTileDownloadFinish(TileItem* tileItem);

private:
	int mapSizeX;			//整个地图的宽度，单位（个）（瓦片）
	int mapSizeY;
	int m_zoom;				//zoom值
	float m_minZoom;        // 最小缩放层级，刚好适应窗口大小
	float m_maxZoom;        // 最大缩放层级，防止无限放大

	int oldOffsetX;			//移动前的secen左上角瓦片的x
	int oldOffsetY;			//移动前的secen左上角瓦片的y
	QGraphicsScene m_scene;
	QList<TileItem*> mapAreaList;	//瓦片链表

	int tileInDownload;		//记录有几个瓦片在下载，同时下载数不能超过MAX_DOWNLOAD_NUM，否则槽函数短时间多次调用会出现问题
	mutable QMutex mutex; // mutable关键字允许在const成员函数中修改该成员

	std::thread tCheckTileDownload;	//瓦片下载检查线程
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
	QString m_strURL;			//url模板，其中的x、y、z具体值要用{x},{y},{z}代替

};