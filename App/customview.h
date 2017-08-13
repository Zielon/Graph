#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QList>
#include <QVector>
#include <QPair>
#include <QGraphicsLineItem>
#include <QMessageBox>
#include <QGraphicsEllipseItem>
#include <QGraphicsObject>
#include <QVariant>

#include <QGraphicsView>

#include "myscene.h"
#include "vertex.h"
#include "enum.h"
#include "mainwindow.h"
#include "connectionway.h"

class ConnectionWay;
class MyScene;
class Vertex;
class MainWindow;

class CustomView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CustomView(MainWindow*, MyScene*, QWidget *parent);

signals:

public slots:
    //void mousePressEvent(QMouseEvent * event);
    // void mouseReleaseEvent(QMouseEvent * event);
    // void mouseDoubleClickEvent(QMouseEvent * event);
    // void mouseMoveEvent(QMouseEvent * event);
    // void contextMenuEvent(QContextMenuEvent * event);
private:
    MainWindow *window;
    MyScene *myScene;
};

#endif // CUSTOMVIEW_H
