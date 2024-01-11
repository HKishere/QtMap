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

	void mouseReleaseEvent(QMouseEvent *event) override;	//��д����ͷ��¼�
	void wheelEvent(QWheelEvent *event) override;			//��д�������¼�

	void reBuildMoveScene(TileItem* centerTileItem);		//�����ƶ����¹����ͼ
	TileItem * reBuildZoomSceen(TileItem * centerTileItem, QPoint pointInTile, int nextZoomLev);	//�����������¹����ͼ

	void downloadMap();										//��ͼ����Ƿ������̺߳���
	QGraphicsScene* getScene() { return &m_scene; };


	void setmapSizeX(int val) { mapSizeX = val; }
	void setmapSizeY(int val) { mapSizeY = val; }

	void setTileMaxX(int val) { TileMaxX = val; }
	void setTileMaxY(int val) { TileMaxY = val; }

	void setm_minZoom(int val) { m_minZoom = val; }
	void setm_maxZoom(int val) { m_maxZoom = val; }

signals:
	void downloadTile(TileItem* tileItem);					//�����źŸ���̨ȥ���ص�ͼ

public slots:
	void onTileDownloadFinish(TileItem* tileItem);			//��Ƭ���سɹ��ۺ���

private:
	int mapSizeX;			//���ص�ͼ�Ŀ�ȣ���λ����������Ƭ��
	int mapSizeY;			//���ص�ͼ�Ŀ�ȣ���λ����������Ƭ��

	int TileMaxX;			//����㼶��������Ƭ����X
	int TileMaxY;			//����㼶��������Ƭ����Y

	int m_zoom;				//zoomֵ
	float m_minZoom;        //��С���Ų㼶���պ���Ӧ���ڴ�С
	float m_maxZoom;        //������Ų㼶����ֹ���޷Ŵ�

	int oldOffsetX;			//�ƶ�ǰ��secen���Ͻ���Ƭ��x
	int oldOffsetY;			//�ƶ�ǰ��secen���Ͻ���Ƭ��y
	QGraphicsScene m_scene;
	
	std::map<std::tuple<int, int, int>, TileItem*> mapAreaMap;	//��Ƭmap,��ֵ��x,y,z

	int tileInDownload;		//��¼�м�����Ƭ�����أ�ͬʱ���������ܳ���MAX_DOWNLOAD_NUM������ۺ�����ʱ���ε��û��������
	mutable QMutex mutex;	// mutable�ؼ���������const��Ա�������޸ĸó�Ա

	std::thread tCheckTileDownload;	//��Ƭ���ؼ���߳�
	bool theadStartFlag;			//�߳�����flag
};

class IMapGraphicsView : public QObject
{
	Q_OBJECT
public:
	IMapGraphicsView();
	void setSence(QGraphicsScene* scene);
	void setUrlStr(QString strURL) { m_strURL = strURL; };

public slots:
	void onDownLoadTile(TileItem* tileItem);			//��̨������ƬͼƬ�ۺ���

signals:
	void TileDownloadFinish(TileItem* tileItem);		//��Ƭ��ͼ��������ź�

private:
	QGraphicsScene* m_scene;
	QString m_strURL;			//urlģ�壬���е�x��y��z����ֵҪ��{x},{y},{z}����

};