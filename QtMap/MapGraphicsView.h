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
	int mapSizeX;			//������ͼ�Ŀ�ȣ���λ����������Ƭ��
	int mapSizeY;
	int m_zoom;				//zoomֵ
	float m_minZoom;        // ��С���Ų㼶���պ���Ӧ���ڴ�С
	float m_maxZoom;        // ������Ų㼶����ֹ���޷Ŵ�

	int oldOffsetX;			//�ƶ�ǰ��secen���Ͻ���Ƭ��x
	int oldOffsetY;			//�ƶ�ǰ��secen���Ͻ���Ƭ��y
	QGraphicsScene m_scene;
	QList<TileItem*> mapAreaList;	//��Ƭ����

	int tileInDownload;		//��¼�м�����Ƭ�����أ�ͬʱ���������ܳ���MAX_DOWNLOAD_NUM������ۺ�����ʱ���ε��û��������
	mutable QMutex mutex; // mutable�ؼ���������const��Ա�������޸ĸó�Ա

	std::thread tCheckTileDownload;	//��Ƭ���ؼ���߳�
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
	QString m_strURL;			//urlģ�壬���е�x��y��z����ֵҪ��{x},{y},{z}����

};