#ifndef EULERALGORITHM_H
#define EULERALGORITHM_H
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
#include <QStack>
#include <algorithm>
#include <iterator>
#include <functional>
#include <queue>
#include "myscene.h"
#include "vertex.h"
#include "connectionway.h"


class Vertex;
class MyScene;
class ConnectionWay;

class EulerAlgorithm
{
public:
    EulerAlgorithm(QHash<QString, Vertex*>);
    bool eulerAlgo(QString);
    QStack<Vertex*> getCe(){return ce;}
    int getIndexByConnection(Vertex*, Vertex *v);
    bool hasCycle();
    int edgesCount();

private:
    Vertex *getVertex(Vertex*, ConnectionWay*, int &index);
    QHash<QString, Vertex*> hashTable;
    QStack<Vertex*> ce;
    QStack<Vertex*> stos;

};

#endif // EULERALGORITHM_H
