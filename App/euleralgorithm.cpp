#include "euleralgorithm.h"

EulerAlgorithm::EulerAlgorithm(QHash<QString, Vertex *> t) : hashTable(t)
{

}

bool EulerAlgorithm::eulerAlgo(QString start)
{
    stos.push(hashTable[start]);
    int index = 0;
    if(hasCycle()){
        while(!stos.empty()){
            Vertex* v = stos.top();
            if(v->getConnectionVector().count() > 0){
                Vertex *u = hashTable[v->getConnectionVector()[0].second];
                ConnectionWay *edge = v->getConnectionVector()[0].first;
                for(auto i : v->getConnectionVector()){
                    if(i.first->getWeight() > edge->getWeight()) continue;
                    else edge = i.first;
                }
                u = getVertex(v, edge, index);              //index passed by reference
                v->getConnectionVector().removeAt(index);
                index = getIndexByConnection(u, v);
                u->getConnectionVector().removeAt(index);
                index = 0;
                stos.push(u);

            }else if (v->getConnectionVector().count() == 0)
            {
                stos.pop();
                ce.push(v);
            }
        }
        return true;
    }else return false;
}

int EulerAlgorithm::getIndexByConnection(Vertex *u, Vertex *v)
{
    int j = 0;
    for(auto i : u->getConnectionVector()){
        if(i.second == v->getVertexID()) return j;
        j++;
    }
}

bool EulerAlgorithm::hasCycle()
{
    QHashIterator<QString, Vertex*> c(hashTable);
    while(c.hasNext()){
        c.next();
        Vertex * v = c.value();
        if(v->getConnectionVector().count() %2 == 0) continue;
        else return false;
    }
    return true;
}

int EulerAlgorithm::edgesCount()
{
    int egdesCount = 0;
    QHash<QString, Vertex*>::const_iterator i;
    for(i = hashTable.constBegin(); i != hashTable.constEnd(); ++i){
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

Vertex *EulerAlgorithm::getVertex(Vertex *v, ConnectionWay *edge, int &index)
{
    for(auto i : v->getConnectionVector()){
        if(i.first == edge){
            return hashTable[i.second];
        }
        index++;
    }
}

