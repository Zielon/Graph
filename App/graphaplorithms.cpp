#include "graphaplorithms.h"
#include <QVectorIterator>
#include <QHash>
#include <QHashIterator>
#include <QPair>
#include "tspalgorithm.h"
#include "euleralgorithm.h"
#include "newtspalgorithm.h"

//The class with the every graph algorithm used in this application
//
int GraphAplorithms::GINFINITY = 999999999;

GraphAplorithms::GraphAplorithms(MyScene* s): scene(s)
{
    vertexTable = s->getHashTable();
    QHashIterator<QString, Vertex*> i(vertexTable);
    while(i.hasNext()){
        i.next();
        QPair<QString, Vertex*> pair = qMakePair(i.value()->getVertexID(), i.value());
        sortedMartix.append(pair);
    }
    qSort(sortedMartix.begin(), sortedMartix.end(), [](QPair<QString, Vertex*> &a, QPair<QString, Vertex*> &b){
        if (a.first.toInt() < b.first.toInt()) return true;
    });
    createMatrix();
}

//Travelling salesman problem
//
void GraphAplorithms::tspAlgorithm()
{
    tspalgo = new TSPAlgorithm(this, scene);
    solutionRoot = new Tree(adjacencyMatrix, this);
    solutionRoot->setVertexIndex("S");
    tspalgo->TraverseTree(adjacencyMatrix, solutionRoot);
    C = tspalgo->getWay();
    min_solution = tspalgo->getMinSol();
    solutionVector = tspalgo->getSolutionVector();
}

void GraphAplorithms::tspSimpleAlgorithm()
{
    tspalgo = new TSPAlgorithm(this, scene);
    tspalgo->simpleTSP(sortedMartix[0].second);
    stack = tspalgo->getStack();
    min_solution = tspalgo->getMinSol();
}



bool GraphAplorithms::euler(QString start)
{
    EulerAlgorithm* euler = new EulerAlgorithm(createCopy());
    bool flag = euler->eulerAlgo(start);
    stack = euler->getCe();
    return flag;
}


//Dijskta algorithm based on priority_queue min type
//
void GraphAplorithms::Dijkstra(Vertex *source)
{
    initializeSingleSource(source);
    setPriorityQueue();
    stack.clear();
    while(!Q.empty()){
        updateQueue();
        Vertex *u = Q.top();
        stack.append(u);
        Q.pop();
        for(auto i : u->getConnectionVector()){
            Vertex* v = vertexTable[i.second];
            relax(u, v, i.first->getWeight());
            v->setNumberID(QString::number(v->getWay()));
            scene->update();
        }
    }
}

//Bellman-Ford algorithm
//
bool GraphAplorithms::BellmanFord(Vertex *source)
{
    initializeSingleSource(source);
    int edges = edgesCount() - 1;
    for(int g = 0; g < edges; g++){
        QHashIterator<QString, Vertex*> i(vertexTable);
        while(i.hasNext()){
            i.next();
            Vertex *u = i.value();
            for(auto j : u->getConnectionVector()){
                Vertex* v = vertexTable[j.second];
                if(v != source){
                    relax(u, v, j.first->getWeight());
                    v->setNumberID(QString::number(v->getWay()));
                    scene->update();
                }
            }
        }
    }
    QHashIterator<QString, Vertex*> c(vertexTable);
    while(c.hasNext()){
        c.next();
        for(auto j : c.value()->getConnectionVector()){
            Vertex* v = vertexTable[j.second];
            if(v->getWay() > c.value()->getWay() + j.first->getWeight()){
                scene->warningMessage("There is\n"
                                      "negative cycle");
                return false;
            }
        }
    }
    return true;
}

//Relax method to estimate and set the shortes way
//
void GraphAplorithms::relax(Vertex *u, Vertex *v, int w)
{
    if(v->getWay() > u->getWay() + w){
        v->setWay(u->getWay() + w);
        v->setPreviousVertex(u);
    }
}

//Init every vertex with infinity value
//
void GraphAplorithms::initializeSingleSource(Vertex *selected)
{
    setEdgesUnVisited();
    QHashIterator<QString, Vertex*> i(vertexTable);
    while(i.hasNext()){
        i.next();
        if(i.value() == selected){
            i.value()->setWay(0);
        }else{
            i.value()->setWay(GINFINITY);
            i.value()->setPreviousVertex(nullptr);
        }
    }
}

// Initialize priority_queue min in Dijkstra algorithm
//
void GraphAplorithms::setPriorityQueue()
{
    QHashIterator<QString, Vertex*> i(vertexTable);
    while(i.hasNext()){
        i.next();
        Q.push(i.value());
    }
}

//Creates adjacencyMatrix
//
void GraphAplorithms::createMatrix()
{
    int size = vertexTable.count();
    QVector<QPair<int, Vertex*> > g(size, qMakePair(GINFINITY, nullptr));
    for(int i = 0; i < size; i++){
        adjacencyMatrix.append(g);
    }
    int q = 0;
    for(auto i : sortedMartix){
        Vertex *v = i.second;
        for(auto c: v->getConnectionVector()){
            QPair<int,int> pos = getPairFromHashTable(v->getVertexID(), c.second);
            adjacencyMatrix[q][pos.first] = qMakePair(pos.second, vertexTable[c.second]);
        }
        q++;
    }

}

//Returns the pair where the first vaule is index in 2D table and the second vaule is weight to this index
//
QPair<int,int> GraphAplorithms::getPairFromHashTable(QString from, QString to)
{
    int index = 0;
    for(auto i : sortedMartix){
        Vertex *v = i.second;
        if(v->getVertexID() == to){
            for(auto j : v->getConnectionVector()){
                if(j.second == from){
                    QPair<int,int> pair = qMakePair(index, j.first->getWeight());
                    return pair;
                }
            }
        }
        index++;
    }
}

QVector<QVector<QPair<int, Vertex*> > >& GraphAplorithms::getAdjacencyMatrix()
{
    return adjacencyMatrix;
}

QSet<ConnectionWay *> &GraphAplorithms::getEgdeSet()
{
    return C;
}


//Copy hash table with vertex and connections
//
QHash<QString, Vertex*> GraphAplorithms::createCopy()
{
    QHash<QString, Vertex*> copy;
    QHashIterator<QString, Vertex*> i(vertexTable);
    while(i.hasNext()){
        i.next();
        Vertex *old = i.value();
        Vertex *n = new Vertex(old);
        copy[n->getVertexID()] = n;
    }
    QVector<QPair<ConnectionWay*, QString > > vector;
    i.toFront();
    while(i.hasNext()){
        i.next();
        Vertex *old = i.value();
        for(auto j : old->getConnectionVector()){
            QPair<QPointF, Vertex*> from;
            from.second = copy[j.first->getVertexFrom().second->getVertexID()];
            from.first = j.first->getVertexFrom().first;

            QPair<QPointF, Vertex*> to;
            to.second = copy[j.first->getVertexTo().second->getVertexID()];
            to.first = j.first->getVertexTo().first;
            ConnectionWay *n = new ConnectionWay(from, to, j.first->getWeight());
            vector.append(qMakePair(n, j.second));
        }
        copy[old->getVertexID()]->setConnectionVector(vector);
        vector.clear();
    }
    return copy;
}

//Update priority_queue to get min key at the top of queue
//
void GraphAplorithms::updateQueue()
{
    std::priority_queue<Vertex*, std::vector<Vertex*>, Comparator > Q_temp;
    while(!Q.empty()){
        Q_temp.push(Q.top());
        Q.pop();
    }
    Q.swap(Q_temp);
}

//Get connection object. Function used to find egdes of the shortest way
//
ConnectionWay *GraphAplorithms::getShortestEdge(Vertex *from, Vertex *to)
{
    if(from){
        for(auto i : from->getConnectionVector()){
            if(to){
                if(i.second == to->getVertexID()){
                    return i.first;
                }
            }
        }
    }
    return nullptr;
}

//Printing vertexes and coloring edges of the shortest way
//
QString GraphAplorithms::printShortestPath(Vertex *source, Vertex *destination)
{
    QString s = "0";
    QVector <Vertex*> shortestPath;
    shortestPath.append(destination);
    Vertex *previous = destination->getPreviousVertex();
    Vertex *temp = nullptr;
    ConnectionWay *w = getShortestEdge(destination, previous);
    
    w->setShortestWayMade(true);
    
    while(s != source->getVertexID()){
        shortestPath.append(previous);
        s = previous->getVertexID();
        temp = previous;
        previous = previous->getPreviousVertex();
        if(s != source->getVertexID()){
            w = getShortestEdge(temp, previous);
            w->setShortestWayMade(true);
        }
    }
    
    QString output;
    QVectorIterator<Vertex*> i(shortestPath);
    i.toBack();
    while(i.hasPrevious()){
        Vertex* v = i.previous();
        output += "Vetex: " + v->getVertexID() + " weight: " + QString::number(v->getWay()) + "\n";
    }
    return output;
    scene->update();
}


void GraphAplorithms::setEdgesUnVisited()
{
    QHashIterator<QString, Vertex*> i(vertexTable);
    while(i.hasNext()){
        i.next();
        Vertex *v = i.value();
        for(auto j : v->getConnectionVector()){
            j.first->setVisited(false);
        }
    }
}

int GraphAplorithms::edgesCount()
{
    setEdgesUnVisited();
    int egdesCount = 0;
    QHash<QString, Vertex*>::const_iterator i;
    for(i = vertexTable.constBegin(); i != vertexTable.constEnd(); ++i){
        Vertex *v = i.value();
        for(auto j : v->getConnectionVector()){
            ConnectionWay* edge = j.first;
            if(!edge->isVisited()){
                egdesCount++;
                edge->setVisited(true);
            }
        }
    }
    return egdesCount;
}

//Clean up weigth in vertexes and printed shortest way
//
void GraphAplorithms::cleanUpGraph()
{
    QHashIterator<QString, Vertex*> i(vertexTable);
    while(i.hasNext()){
        i.next();
        Vertex* v = i.value();
        for(auto j : v->getConnectionVector()){
            j.first->setShortestWayMade(false);
            vertexTable[j.second]->restoreVertexID();
            scene->update();
        }
    }
}

