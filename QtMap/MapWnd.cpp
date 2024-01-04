#include "MapWnd.h"
#include <QGraphicsScene>
#include <QGraphicsitem>
#include <QApplication>
#include <QGraphicsView>
#include "MapGraphicsView.h"

MapWnd::MapWnd(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	backEnd = new IMapGraphicsView();
	backEnd->setSence(ui.graphicsView->getScene());
	connect(ui.graphicsView, &MapGraphicsView::downloadTile, backEnd, &IMapGraphicsView::onDownLoadTile, Qt::QueuedConnection);
	connect(backEnd, &IMapGraphicsView::TileDownloadFinish, ui.graphicsView, &MapGraphicsView::onTileDownloadFinish);

	//ui.graphicsView->downloadMap();
	
	init();
}
void MapWnd::init()
{
	ui.graphicsView->init();
}

MapWnd::~MapWnd()
{
	delete backEnd;
}

