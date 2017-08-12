#ifndef TSPALGORITHM_H
#define TSPALGORITHM_H
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
#include <QSet>
#include "myscene.h"
#include "vertex.h"
#include "connectionway.h"
#include "graphaplorithms.h"

class Vertex;
class MyScene;
class ConnectionWay;
struct Tree;
class GraphAplorithms;
class TSPAlgorithm
{
public:
    TSPAlgorithm(GraphAplorithms*s, MyScene*);
    int reduceMatrix(QVector<QVector<QPair<int, Vertex *> > > &M);
    int smallestElementInVactor(QVector<QPair<int, Vertex*> > V);
    int smallestElemInRowCol(QVector<QVector<QPair<int, Vertex *> > > &M, int x, int y);
    QPair<int, QPair<int, int> > findEdge(QVector<QVector<QPair<int, Vertex *> > > &M);
    QVector<QVector<QPair<int, Vertex*> > > cutEdgeFromVertex(QVector<QVector<QPair<int, Vertex *> > > &M, Tree *root);
    void TraverseTree(QVector<QVector<QPair<int, Vertex *> > > M, Tree *t);
    int getMatrixSize(QVector<QVector<QPair<int, Vertex *> > > &M);
    ConnectionWay *getConnectionFromIndex(QVector<QVector<QPair<int, Vertex *> > > &M, Tree*);
    void getWay(QVector<QVector<QPair<int, Vertex *> > > &M, Tree*);
    ConnectionWay *getConnection(Vertex*, Vertex*);
    QSet<ConnectionWay *> &getWay();
    int getMinSol(){return min_sol;}
    QPair<int, int> getIndexFromConnection(QString v, QString u, Tree *t);
    void simpleTSP(Vertex *v);
    QStack<Vertex*> getStack(){return S;}
    void findCycle(QVector<Vertex*>&, Vertex *selected);
    QVector<Tree*> getSolutionVector(){return solutionVector;}
    QString endOrElementOfCycle(Vertex*);

private:
    QSet<ConnectionWay *> way;
    QSet<Vertex*> selectedVertexes;
    QStack<Vertex*> Sh;
    QStack<Vertex*> S;
    GraphAplorithms* algo;
    int GINFINITY = 999999999;
    int min_sol;
    int dh = 0;
    Tree* rootTree;
    MyScene* scene;
    QVector<Tree*> solutionVector;
    QVector<Vertex*> fwd;
};

struct Tree{
public:
    Tree(QVector<QVector<QPair<int, Vertex*> > > m, GraphAplorithms *algo);
    Tree(QVector<QVector<QPair<int, Vertex*> > > m, Tree *t);
    void createPatternMatrix(GraphAplorithms *);
    void setLeftSon(Tree*);
    void setRightSon(Tree*);
    void setRoot(Tree*);
    void setEdge(ConnectionWay*);
    void setVertexIndex(QString s){vertexIndex = s;}
    QString getVertexIndex(){return vertexIndex;}
    QVector<ConnectionWay *> getEdge(){return edge;}
    int &getLB(){ return LB; }
    void setLB(int lb){ LB = lb; }
    int &getMax(){ return max; }
    void setMax(int m){ max = m; }
    QPair<int,int> &getIndex(){ return index; }
    void setIndex(QPair<int,int> i){ index = i; }
    void setMatrix(QVector<QVector<QPair<int, Vertex*> > > m){M = m;}
    QList<QString> patternMatrixForI;
    QList<QString> patternMatrixForJ;
    Tree* getLeft(){return leftSon;}
    Tree* getRigth(){return rigthSon;}
    Tree* getRoot(){return root;}
    QVector<QVector<QPair<int, Vertex*> > >& getMatrix(){return M;}

private:
    QVector<QVector<QPair<int, Vertex*> > > M;
    QPair<int,int> index;
    Tree *leftSon;
    Tree *rigthSon;
    Tree *root;
    QVector<ConnectionWay *> edge;
    QString vertexIndex;
    int LB;
    int max;
};

#endif // TSPALGORITHM_H
