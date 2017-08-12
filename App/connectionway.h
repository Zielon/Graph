#ifndef CONNECTIONWAY_H
#define CONNECTIONWAY_H

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

#include "myscene.h"
#include "vertex.h"
#include "enum.h"

class MyScene;
class Vertex;
class ConnectionWay : public QGraphicsItem
{
   // Q_OBJECT
public:
    ConnectionWay(QPair<QPointF, Vertex*>, QPair<QPointF, Vertex*>, int);
    ConnectionWay(ConnectionWay *old, QPair<QPointF, Vertex*> from, QPair<QPointF, Vertex*> to);
    ~ConnectionWay(){}
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    QString className(){ return "ConnectionWay"; }
    QPointF moveLineToCenter(QPointF newPos, Vertex *v);
    void setSelectedConnection();
    void unsetSelectedConnection();
    int getWeight();
    void setWeigth(int w);
    QPair<QPointF, Vertex*> getVertexFrom();
    QPair<QPointF, Vertex*> getVertexTo();
    void swapConnections();
    bool isCoordsAreEqual(ConnectionWay*conn);
    bool isVisited();
    void setVisited(bool);
    void setShortestWayMade(bool);

protected:

private:
    QPair<QPointF, Vertex*> vertexFrom;
    QPair<QPointF, Vertex*> vertexTo;
    bool SelectedForConnection;
    bool shortestWayMade;
    int weight;
    bool visited;
};
#endif // CONNECTIONWAY_H
