#pragma once
#include <qgraphicsview.h>
#include <QMouseEvent>
#include <QMutex>
#include "TileItem.h"
#include <thread>
#include <map>
#include <tuple>

class MapGraphicsView :
	public QGraphicsView
{
	Q_OBJECT
public:
	MapGraphicsView(QWidget* parent);
	virtual ~MapGraphicsView();
	void init();

	void mouseReleaseEvent(QMouseEvent *event) override;	//重写鼠标释放事件
	void wheelEvent(QWheelEvent *event) override;			//重写鼠标滚轮事件

	void reBuildMoveScene(TileItem* centerTileItem);		//根据移动重新构造地图
	TileItem * reBuildZoomSceen(TileItem * centerTileItem, QPoint pointInTile, int nextZoomLev);	//根据缩放重新构造地图

	void downloadMap();										//地图检查是否下载线程函数
	QGraphicsScene* getScene() { return &m_scene; };


	void setmapSizeX(int val) { mapSizeX = val; }
	void setmapSizeY(int val) { mapSizeY = val; }

	void setTileMaxX(int val) { TileMaxX = val; }
	void setTileMaxY(int val) { TileMaxY = val; }

	void setm_minZoom(int val) { m_minZoom = val; }
	void setm_maxZoom(int val) { m_maxZoom = val; }

signals:
	void downloadTile(TileItem* tileItem);					//发送信号给后台去下载地图

public slots:
	void onTileDownloadFinish(TileItem* tileItem);			//瓦片下载成功槽函数

private:
	int mapSizeX;			//加载地图的宽度，单位（个）（瓦片）
	int mapSizeY;			//加载地图的宽度，单位（个）（瓦片）

	int TileMaxX;			//这个层级的最大的瓦片索引X
	int TileMaxY;			//这个层级的最大的瓦片索引Y

	int m_zoom;				//zoom值
	float m_minZoom;        //最小缩放层级，刚好适应窗口大小
	float m_maxZoom;        //最大缩放层级，防止无限放大

	int oldOffsetX;			//移动前的secen左上角瓦片的x
	int oldOffsetY;			//移动前的secen左上角瓦片的y
	QGraphicsScene m_scene;
	
	std::map<std::tuple<int, int, int>, TileItem*> mapAreaMap;	//瓦片map,键值是x,y,z

	int tileInDownload;		//记录有几个瓦片在下载，同时下载数不能超过MAX_DOWNLOAD_NUM，否则槽函数短时间多次调用会出现问题
	mutable QMutex mutex;	// mutable关键字允许在const成员函数中修改该成员

	std::thread tCheckTileDownload;	//瓦片下载检查线程
	bool theadStartFlag;			//线程启动flag
};

class IMapGraphicsView : public QObject
{
	Q_OBJECT
public:
	IMapGraphicsView();
	void setSence(QGraphicsScene* scene);
	void setUrlStr(QString strURL) { m_strURL = strURL; };

public slots:
	void onDownLoadTile(TileItem* tileItem);			//后台下载瓦片图片槽函数

signals:
	void TileDownloadFinish(TileItem* tileItem);		//瓦片地图下载完成信号

private:
	QGraphicsScene* m_scene;
	QString m_strURL;			//url模板，其中的x、y、z具体值要用{x},{y},{z}代替

};