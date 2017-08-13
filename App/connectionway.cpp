#include "connectionway.h"
#include <QVariant>

//The constructor which takes 3 arguments as a pair from and to with a weight between them
//
ConnectionWay::ConnectionWay(QPair<QPointF, Vertex*> from, QPair<QPointF, Vertex*> to, int w) : vertexFrom(from), vertexTo(to), weight(w)
{
    //setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    SelectedForConnection = false;
    shortestWayMade = false;
    visited = false;
    setZValue(1);
}

ConnectionWay::ConnectionWay(ConnectionWay *old, QPair<QPointF, Vertex*> from, QPair<QPointF, Vertex*> to):
    vertexFrom(from), vertexTo(to), weight(old->getWeight())
{
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    SelectedForConnection = false;
    shortestWayMade = false;
    visited = false;
    setZValue(1);
}

QRectF ConnectionWay::boundingRect() const
{
    return QRectF(0, 0, 50, 50);
}

//The paint function which changes color depended on the bool flag
//
void ConnectionWay::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QFont font("consolas", 10);
    QPen pen(Qt::gray);
    pen.setWidth(1);
    if(SelectedForConnection){
        pen.setColor(Qt::red);
    }
    if(shortestWayMade){
        pen.setColor(Qt::blue);
        pen.setWidth(2);
    }
    QRectF rec = boundingRect();
    painter->setPen(pen);
    QPointF from(vertexFrom.first);
    QPointF to(vertexTo.first);
    painter->drawLine(from.x(), from.y(),to.x(),to.y());
    int centerX=(from.x()+to.x())/2;
    int centerY=(from.y()+to.y())/2;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setFont(font);
    painter->drawText(centerX, centerY - 3, QString::number(weight));

}

//The function is responsible for the item change during moving vertex on the scene, the connection cant by broken
//
QVariant ConnectionWay::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &val)
{
    if (change == ItemPositionChange && scene()) {
        Vertex *v = val.value<Vertex*>();
        QPointF newPos = v->getCoords();
        if(vertexFrom.second == v){
            vertexFrom.first = moveLineToCenter(newPos, v);

        }else if(vertexTo.second == v){

            vertexTo.first = moveLineToCenter(newPos, v);
        }

    }
    return QGraphicsItem::itemChange(change, val);
}

QPointF ConnectionWay::moveLineToCenter(QPointF newPos, Vertex *v)
{
    QPointF rect = v->boundingRect().center();
    int xOffset = 25;
    int yOffset = 25;
    QPointF newCenterPos = QPointF(newPos.x() + xOffset, newPos.y() + yOffset);
    return newCenterPos;
}

void ConnectionWay::setSelectedConnection()
{
    SelectedForConnection = true;
    update();
}

void ConnectionWay::unsetSelectedConnection()
{
    SelectedForConnection = false;
    update();
}

int ConnectionWay::getWeight()
{
    return weight;
}

void ConnectionWay::setWeigth(int w)
{
    weight = w;
}

QPair<QPointF, Vertex *> ConnectionWay::getVertexFrom()
{
    return vertexFrom;
}

QPair<QPointF, Vertex *> ConnectionWay::getVertexTo()
{
    return vertexTo;
}

void ConnectionWay::swapConnections()
{
    QPair<QPointF, Vertex*> temp = vertexFrom; vertexFrom = vertexTo; vertexTo
            = temp;
}

// The Function to check if two connections are equal, it is used in the serialization object
//
bool ConnectionWay::isCoordsAreEqual(ConnectionWay *conn)
{
    int xFrom = conn->getVertexFrom().first.x();
    int yFrom = conn->getVertexFrom().first.y();
    int xF = vertexFrom.first.x();
    int yF = vertexFrom.first.y();

    int xTo = conn->getVertexTo().first.x();
    int yTo = conn->getVertexTo().first.y();
    int xT = vertexTo.first.x();
    int yT = vertexTo.first.y();

    if(xTo == xT && yTo == yT && xFrom == xF && yFrom == yF) return true;
    else return false;
}

bool ConnectionWay::isVisited()
{
    return visited;
}

void ConnectionWay::setVisited(bool flag)
{
    visited = flag;
}

void ConnectionWay::setShortestWayMade(bool flag)
{
    shortestWayMade = flag;
    update();
}
