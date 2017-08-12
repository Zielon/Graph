#ifndef GRAPHAPLORITHMS_H
#define GRAPHAPLORITHMS_H
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
#include "tspalgorithm.h"


class Vertex;
class MyScene;
class ConnectionWay;
class TSPAlgorithm;
struct Tree;
class GraphAplorithms
{
public:
    GraphAplorithms(MyScene*);
    int edgesCount();
    void Dijkstra(Vertex*);
    bool BellmanFord(Vertex*);
    void tspAlgorithm();
    void tspSimpleAlgorithm();
    bool euler(QString);
    QString printShortestPath(Vertex *source, Vertex *destination);
    void cleanUpGraph();
    QVector<QVector<QPair<int, Vertex*> > >& getAdjacencyMatrix();
    QSet<ConnectionWay *> &getEgdeSet();
    QStack<Vertex*> &getStack(){return stack;}
    QHash<QString, Vertex *> createCopy();
    QHash<QString, Vertex *> getHashTable(){return vertexTable;}
    int getMinSolution(){return min_solution;}
    TSPAlgorithm * getTSPPtr(){return tspalgo;}
    Tree* getSolutionTree(){return solutionRoot;}
    QVector<QPair<QString, Vertex*> >& getSortedMatrix(){return sortedMartix;}
    QVector<Tree*> &getSolutionVector(){return solutionVector;}

private:
    void initializeSingleSource(Vertex*);
    void extracMin();
    void setEdgesUnVisited();
    void relax(Vertex*, Vertex*, int);
    void setPriorityQueue();
    void createMatrix();
    void updateQueue();
    QPair<int,int> getPairFromHashTable(QString, QString);
    bool isNegativCycle(Vertex *, Vertex *);
    ConnectionWay *getShortestEdge(Vertex*, Vertex*);
    struct Comparator{
        bool operator() ( Vertex* i, Vertex* j){
            if(i->getWay() > j->getWay()) return true;
            else if(i->getWay() < j->getWay()) return false;
        }
    };

private:
    std::priority_queue<Vertex*, std::vector<Vertex*>, Comparator > Q;
    QSet<ConnectionWay*> C;
    static int GINFINITY;
    QStack<Vertex*> stack;
    QHash<QString, Vertex*> vertexTable;
    QVector<QVector<QPair<int, Vertex*> > > adjacencyMatrix;
    MyScene *scene;
    Tree* solutionRoot;
    TSPAlgorithm *tspalgo;
    QString estimetedPath;
    int min_solution;
    QVector<QPair<QString, Vertex*> > sortedMartix;
    QVector<Tree*> solutionVector;
};

#endif // GRAPHAPLORITHMS_H
