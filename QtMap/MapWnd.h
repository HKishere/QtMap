#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtMap.h"
#include <QGraphicsitem>
#include <QGraphicsView>
#include <QApplication>
#include <QGraphicsScene>
#include "TileItem.h"

class IMapGraphicsView;

class MapWnd : public QMainWindow
{
    Q_OBJECT

public:
    MapWnd(QWidget *parent = nullptr);

	void init();

    ~MapWnd();

private:
    Ui::MapWndClass ui;
	IMapGraphicsView* backEnd;
};

