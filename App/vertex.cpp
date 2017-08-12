#include "vertex.h"
#include <QColor>

int Vertex::vertexNumber = 1;


Vertex::Vertex(MyScene *ptr): scene(ptr)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    QGraphicsItem::setAcceptHoverEvents(true);
    vertexID = QString::number(vertexNumber++);
    numberID = vertexID;
    selected = false;
    visited = false;
    way = 0;
    setZValue(5);
}

Vertex::Vertex(Vertex *old) : scene(old->getScene())
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    QGraphicsItem::setAcceptHoverEvents(true);
    way = old->getWay();
    numberID = old->getVertexID();
    vertexID = old->getVertexID();
    selected = false;
    visited = false;
    setZValue(5);
}

QRectF Vertex::boundingRect() const
{
    return QRectF(0, 0, 50, 50);
}

void Vertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QFont font("consolas", 11);
    QPen pen;
    pen.setColor(QColor(Qt::red));
    QBrush brush(Qt::red);
    if(selected){
       // pen.setColor(Qt::blue);
         brush.setColor(Qt::red);
    }else{
        brush.setColor(Qt::gray);
        //pen.setColor(Qt::black);
    }

    painter->setBrush(brush);
    painter->drawEllipse(rec);
    painter->setFont(font);
    painter->drawText(rec, Qt::AlignCenter, numberID);
}

void Vertex::setSelected()
{
    selected = true;
    update();
}

void Vertex::unsetSelected()
{
    selected = false;
    update();
}

QString Vertex::getVertexID()
{
    return vertexID;
}

void Vertex::setVertexID(QString id)
{
    vertexID = id;
    numberID = id;
    update();
}

void Vertex::setCoords(QPointF points)
{
    myCoords = points;
    update();
}

QPointF Vertex::getCoords()
{
    return myCoords;
}


void Vertex::addToConnectionVector(ConnectionWay * conn, QString id)
{
    connectionVector.append(qMakePair(conn, id));
}

QVector<QPair<ConnectionWay *, QString> > &Vertex::getConnectionVector()
{
    return connectionVector;
}

bool Vertex::isConnected(QString to)
{
    for(auto i : connectionVector){
        if(i.second == to) return false;
    }
    return true;
}

int Vertex::getWay()
{
    return way;
}

void Vertex::setWay(int w)
{
    way = w;
}

void Vertex::setPreviousVertex(Vertex *v)
{
    previousVertex = v;
}

Vertex *Vertex::getPreviousVertex()
{
    return previousVertex;
}


int Vertex::getVertexNumber()
{
    return vertexNumber;
}

void Vertex::setVertexNumber(int number)
{
    vertexNumber = number;
}

void Vertex::setVisited(bool flag)
{
    visited = flag;
}

bool Vertex::isVisited()
{
    return visited;
}

void Vertex::setNumberID(QString s)
{
    numberID = vertexID + "(" + s + ")";
}

void Vertex::restoreVertexID()
{
    numberID = vertexID;
    update();
}










