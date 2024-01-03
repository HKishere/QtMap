#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtMap.h"
#include <QGraphicsitem>
#include <QGraphicsView>
#include <QApplication>
#include <QGraphicsScene>
#include "TileItem.h"

class IQGraphicsItemDemo;

class QGraphicsItemDemo : public QMainWindow
{
    Q_OBJECT

public:
    QGraphicsItemDemo(QWidget *parent = nullptr);
    ~QGraphicsItemDemo();

private:
    Ui::QGraphicsItemDemoClass ui;
	IQGraphicsItemDemo* backEnd;
};

