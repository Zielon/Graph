#ifndef MYSCENE_H
#define MYSCENE_H
#include <QGraphicsScene>
#include <QtCore>
#include <QtGui>
#include <QVector>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QHash>
#include <QMainWindow>
#include <QGraphicsView>

#include "vertex.h"
#include "connectionway.h"
#include "mainwindow.h"
#include "weigthstable.h"
#include "customview.h"

class CustomView;
class Vertex;
class ConnectionWay;
class MainWindow;
class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyScene(QWidget *parent, MainWindow*, QGraphicsView*);
    ~MyScene(){}
    void unmakeConnection();
    void warningMessage(const QString &msg);

public slots:
    void addVetrex();
    void makeConnection(QString);
    QHash<QString, Vertex*>& getHashTable();
    void setHashTable(QHash<QString, Vertex*>);
    void weigthChange(QString);
    void setCoords(QPointF);
    void updateVertexesCoords();
    void updateCounter(bool s=true);
    void deleteVertex();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    int getWeight(QPointF);

private:
    QPointF coords;
    QHash<QString, Vertex*> vertexHashTable;
    MainWindow *window;
    QGraphicsView* view;
    bool MakingConnectionState;
    bool mousePressed;
    QString selectedVertexID;
    QGraphicsLineItem *temporaryLine;
};

#endif // MYSCENE_H
