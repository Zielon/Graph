#ifndef VERTEX_H
#define VERTEX_H

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
#include "myscene.h"
#include "enum.h"


class MyScene;
class ConnectionWay;
class Vertex : public QGraphicsObject
{
    Q_OBJECT
public:
    Vertex(MyScene*);
    Vertex(Vertex*old);
    ~Vertex(){}
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setSelected();
    void unsetSelected();
    QString getVertexID();
    void setVertexID(QString);
    void setCoords(QPointF);
    QPointF getCoords();
    QString className(){ return "Vertex"; }
    void setConnection(ConnectionWay *);
    void addToConnectionVector(ConnectionWay*, QString);
    QVector<QPair<ConnectionWay*, QString > >& getConnectionVector();
    void setConnectionVector(QVector<QPair<ConnectionWay*, QString > > v){connectionVector = v;}
    bool isConnected(QString);
    int getWay();
    void setWay(int);
    void setPreviousVertex(Vertex*);
    Vertex *getPreviousVertex();
    int getVertexNumber();
    void setVertexNumber(int);
    void setVisited(bool);
    bool isVisited();
    void setNumberID(QString);
    void restoreVertexID();
    MyScene* getScene(){return scene;}

protected:

public slots:

private:
    Vertex* previousVertex;
    QPointF myCoords;
    static int vertexNumber;
    QString vertexID;
    QString numberID;
    bool selected;
    MyScene *scene;
    QVector<QPair<ConnectionWay*, QString > > connectionVector;
    int way;
    bool visited;
};

#endif // VERTEX_H
