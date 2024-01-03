#include "QGraphicsItemDemo.h"
#include <QGraphicsScene>
#include <QGraphicsitem>
#include <QApplication>
#include <QGraphicsView>
#include "MapGraphicsView.h"

QGraphicsItemDemo::QGraphicsItemDemo(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	backEnd = new IQGraphicsItemDemo();
	backEnd->setSence(ui.graphicsView->getScene());
	connect(ui.graphicsView, &MapGraphicsView::downLoadTile, backEnd, &IQGraphicsItemDemo::onDownLoadTile, Qt::QueuedConnection);
	connect(backEnd, &IQGraphicsItemDemo::TileDownloadFinish, ui.graphicsView, &MapGraphicsView::onTileDownloadFinish);

	ui.graphicsView->showMap();

}
QGraphicsItemDemo::~QGraphicsItemDemo()
{
	delete backEnd;
}

