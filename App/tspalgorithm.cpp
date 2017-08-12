#include "tspalgorithm.h"
#include "graphaplorithms.h"


TSPAlgorithm::TSPAlgorithm(GraphAplorithms *s, MyScene*e) : algo(s), scene(e)
{
    min_sol = GINFINITY;
    QHashIterator<QString, Vertex*> i(e->getHashTable());
    while(i.hasNext()){
        i.next();
        Vertex *v = i.value();
        v->setVisited(false);
    }
}

//Redukcja macierzy, objekt przekazywany przez referencję
//
int TSPAlgorithm::reduceMatrix(QVector<QVector<QPair<int, Vertex *> > > &M)
{
    int min = 0;
    int r = 0;
    int size = getMatrixSize(M);

    for(int i = 0; i < size; i++){
        min = smallestElementInVactor(M[i]);
        if(min > 0){
            r += min;
            for(int j = 0; j < size; j++){
                if(M[i][j].first < GINFINITY)
                    M[i][j].first -= min;
            }
        }
    }

    QVector<QPair<int, Vertex *> > v;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            v.append(M[j][i]);
        }
        min = smallestElementInVactor(v);
        if(min > 0){
            r += min;
            for(int j = 0; j < size; j++){
                if(M[j][i].first < GINFINITY)
                    M[j][i].first -= min;
            }
        }
        v.clear();
    }
    return r;
}

int TSPAlgorithm::smallestElementInVactor(QVector<QPair<int, Vertex *> > V)
{
    int min = V[0].first;
    for(auto i : V){
        if(i.first >= min) continue;
        else min = i.first;
    }
    if(min != GINFINITY)
        return min;
    else return -1;
}

//Znalezienie max min dla miejsc zerowych
//
QPair<int, QPair<int,int> > TSPAlgorithm::findEdge(QVector<QVector<QPair<int, Vertex *> > > &M)
{
    int max = -1;
    int min_R_C = 0;
    QPair<int, int> indexToCut;
    int size = getMatrixSize(M);

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(M[i][j].first == 0){
                min_R_C = smallestElemInRowCol(M,i,j);
                if(min_R_C > max){
                    max = min_R_C;
                    indexToCut.first = i;
                    indexToCut.second = j;
                }
            }
        }
    }
    return qMakePair(max, indexToCut);
}

int TSPAlgorithm::smallestElemInRowCol(QVector<QVector<QPair<int, Vertex *> > > &M, int x, int y)
{
    int min_c = M[0][y].first;
    int min_r = M[x][0].first;
    int size = getMatrixSize(M);

    for(int i = 0; i < size; i++){
        if(M[i][y].first >= min_c)continue;
        else if (M[i][y].first != 0)
            min_c = M[i][y].first;
    }

    for(int i = 0; i < size; i++){
        if(M[x][i].first >= min_r) continue;
        else if (M[x][i].first != 0)
            min_r = M[x][i].first;

    }
    return min_c + min_r;
}

int TSPAlgorithm::getMatrixSize(QVector<QVector<QPair<int, Vertex *> > > &M)
{
    int size = 0;
    for(auto i : M){
        size += 1;
    }
    return size;
}

ConnectionWay *TSPAlgorithm::getConnectionFromIndex(QVector<QVector<QPair<int, Vertex *> > > &M, Tree *t)
{
    int i = t->getIndex().first;
    int j = t->getIndex().second;
    Vertex *from = scene->getHashTable()[t->patternMatrixForI[i]];
    Vertex *to = scene->getHashTable()[t->patternMatrixForJ[j]];

    if(to && from){
        for(auto i : from->getConnectionVector()){
            if(to->getVertexID() == i.second){
                return i.first;
            }
        }
    }
    return nullptr;
}

//Macierz 2x2 warunek kończący rekurencję
//
void TSPAlgorithm::getWay(QVector<QVector<QPair<int, Vertex *> > > &M, Tree *t)
{
    Vertex *from;
    Vertex *to;
    QString end = "";
    bool flag = false;
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            if(M[i][j].first == 0){
                from = scene->getHashTable()[t->patternMatrixForI[i]];
                to = scene->getHashTable()[t->patternMatrixForJ[j]];
                if(getConnection(from,to)) flag = true;
                ConnectionWay* c = getConnection(from,to);
                way.insert(c);
                t->setEdge(c);
                end += "[" + t->patternMatrixForI[i] + "," + t->patternMatrixForJ[j] + "]";
            }
        }
    }
    if(!flag) end += " [∞]";
    t->setVertexIndex(end);
}

ConnectionWay * TSPAlgorithm::getConnection(Vertex *from, Vertex *to)
{
    for(auto i : from->getConnectionVector()){
        if(to->getVertexID() == i.second){
            return i.first;
        }
    }
    return nullptr;
}

QSet<ConnectionWay *> &TSPAlgorithm::getWay()
{
    return way;
}

QString TSPAlgorithm::endOrElementOfCycle(Vertex *v)
{
    int count = 0;
    for(auto i : v->getConnectionVector()){
        Vertex* element = scene->getHashTable()[i.second];
        if(selectedVertexes.contains(element)){
            if(way.contains(getConnection(v, element)))
                count++;
        }
    }

    if(count == 1) return "1";
    if(count == 2) return "2";
    if(count > 2) return "else";
}

//Budowanie cyklu
//
void TSPAlgorithm::findCycle(QVector<Vertex *> &vector, Vertex *selected)
{
    for(auto j : selected->getConnectionVector()){
        Vertex *v = scene->getHashTable()[j.second];    // v sąsiad selected
        if(!v->isVisited()){
            if(selectedVertexes.contains(v))
                if(way.contains(getConnection(v, selected)))
                {
                    QString c = endOrElementOfCycle(v);
                    if(c == "2")
                    {
                        vector.append(v);
                        v->setVisited(true);
                        findCycle(vector, v);

                    }else if(c == "1")
                    {
                        vector.append(v);
                        v->setVisited(true);
                    }
                }
        }
    }
}

//Usunięcie wybranej krawędzi z macierzy oraz zablokowanie łuku odwrotnego i wszystkich dotychczas wybranych
//wybrany łuk to (c,d) blokowany łuk to (d,c) w zmniejszonej macierzy
//
QVector<QVector<QPair<int, Vertex *> > > TSPAlgorithm::cutEdgeFromVertex(QVector<QVector<QPair<int, Vertex *> > > &M, Tree *root)
{

    QVector<QVector<QPair<int, Vertex *> > > reducedM;
    QVector<QPair<int, Vertex *> > row;
    int size = getMatrixSize(M);

    for(int i = 0; i < size; i++){
        if(i == root->getIndex().first) continue;
        else{
            for(int j = 0 ; j < size;j++){
                if(j == root->getIndex().second) continue;
                else{
                    row.append(M[i][j]);
                }
            }
            reducedM.append(row);
            row.clear();
        }
    }

    //Dodanie wybranego łuku do cyklu, zbiorem jest QVector "way"
    Vertex *from = nullptr;
    Vertex *to = nullptr;

    from = scene->getHashTable()[root->patternMatrixForI[root->getIndex().first]];
    to = scene->getHashTable()[root->patternMatrixForJ[root->getIndex().second]];

    ConnectionWay *edge = nullptr;
    if(getConnection(from, to) == getConnection(to, from)){
        way.insert(getConnection(from, to));
        edge = getConnection(from, to);
    }

    selectedVertexes.insert(from);
    selectedVertexes.insert(to);

    //Pattern vectors with vertex id

    //Usuniecie wybranych łukow z szablonów vertexów
    // row(i) and col(j)
    QString r = root->patternMatrixForI[root->getIndex().first];
    QString c = root->patternMatrixForJ[root->getIndex().second];

    root->setVertexIndex(" " + r +" " + c); //ustawienie w drzewie rozwiązań
    root->patternMatrixForI.removeAt(root->getIndex().first);   //delete selected vertex from matrix in column
    root->patternMatrixForJ.removeAt(root->getIndex().second);  //delete selected vertex form matrix in row

    //zablowkowanie łuku (d,c) odwrotnego do (c,d)
    size = getMatrixSize(reducedM);
    int iToBlock = -1;
    int jToBlock = -1;
    for(int i = 0; i < size; i++){
        if(root->patternMatrixForI[i] == c){    //column
            iToBlock = i;
            break;
        }
    }
    for(int j = 0; j < size; j++){
        if(root->patternMatrixForJ[j] == r){    //row
            jToBlock = j;
            break;
        }
    }
    if(iToBlock >= 0 && jToBlock >= 0){
        reducedM[iToBlock][jToBlock].first = GINFINITY;
        reducedM[iToBlock][jToBlock].second = nullptr;
    }

    //Blokowanie łuków tworzących cykl z dotychczas wybranymi
    //
    QHashIterator<QString, Vertex*> iF(scene->getHashTable());  //zerestuj wierzchołki
    while(iF.hasNext()){
        iF.next();
        Vertex *v = iF.value();
        v->setVisited(false);
    }

    QVector<Vertex*> fwd;
    QSetIterator<Vertex *> i(selectedVertexes);
    while (i.hasNext()){
        Vertex*v = i.next();
        if(endOrElementOfCycle(v) == "1"){

            fwd.append(v);
            v->setVisited(true);
            findCycle(fwd, v);

            QString tempCyle;
            for(auto j : fwd) tempCyle += j->getVertexID() + " ";
            qDebug() <<"For "<< tempCyle << "blocked\n";

            for(auto i : fwd){                                                      //znajdź początek cyklu
                if(endOrElementOfCycle(i) == "1"){                                  // i początkiem cyklu
                    for(auto j : fwd){                                              //iteruj po kolejnych elementach
                        if(j != i && endOrElementOfCycle(j) == "1"){                //znajdź koniec cyklu
                                                                                    // j koniec cyklu
                            for(auto k : i->getConnectionVector()){                 //sprawdź czy sąsiadują ze sobą

                                Vertex *iNeighbours = scene->getHashTable()[k.second];

                                if(iNeighbours->getVertexID() == j->getVertexID()){

                                    if(!way.contains(getConnection(j, i))){
                                        qDebug() <<"Blokowanie " << i->getVertexID() << " " << j->getVertexID() << "\n";

                                        QPair<int,int> cycle = getIndexFromConnection(i->getVertexID(), j->getVertexID(), root);
                                        if(cycle.first >= 0 && cycle.second >= 0){
                                            reducedM[cycle.first][cycle.second].first = GINFINITY;
                                            reducedM[cycle.first][cycle.second].second = nullptr;
                                        }
                                        cycle = getIndexFromConnection(j->getVertexID(), i->getVertexID(), root);
                                        if(cycle.first >= 0 && cycle.second >= 0){
                                            reducedM[cycle.first][cycle.second].first = GINFINITY;
                                            reducedM[cycle.first][cycle.second].second = nullptr;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        fwd.clear();
    }

    root->setMatrix(reducedM);
    return reducedM;
}



QPair<int, int> TSPAlgorithm::getIndexFromConnection(QString v, QString u, Tree*t)
{
    QPair<int, int> pair = qMakePair(-1,-1);
    int size = t->patternMatrixForI.count();
    for(int i = 0; i < size; i++){
        if(t->patternMatrixForI[i] == v){
            pair.first = i;
            break;
        }
    }
    for(int j = 0; j < size; j++){
        if(t->patternMatrixForJ[j] == u){
            pair.second = j;
            break;
        }
    }
    return pair;
}


//Główny algorytm
//inicjalnie min_sol ustanione na nieskończoność (GINFINITY)
// Struct Tree has attribs:
//  -LB
//  -max
//  -index which is coords of edge
//  -matrixPatterns for columns and rows
//
void TSPAlgorithm::TraverseTree(QVector<QVector<QPair<int, Vertex *> > > M, Tree* t)
{
    int size = getMatrixSize(M);

    int r = reduceMatrix(M);                 //M is passing by reference (working on orginal 2D matrix)
    QPair<int, QPair<int,int> > maxAndEdge;
    maxAndEdge = findEdge(M);                //pair of max value and edge coords (QPair<int,int>)

    Tree *tree;
    tree = new Tree(M, t);
    tree->setRoot(t);

    if(tree->getLB() + r < min_sol ){

        if(size == 2){                      //warunek kończący rekurencę
            min_sol = tree->getLB() + r;
            t->setLeftSon(tree);
            getWay(M, tree);
            solutionVector.append(tree);
            return;
        }

        //Init left root's son values
        tree->setMax(maxAndEdge.first);
        tree->setIndex(maxAndEdge.second);
        tree->setLB(tree->getLB() + r);
        tree->setEdge(getConnectionFromIndex(M, tree));
        t->setLeftSon(tree);

        TraverseTree(cutEdgeFromVertex(M, tree), tree);     //LEFT SON

        tree = new Tree(M, t);                              //add rigth son
        //Init right root's son values
        tree->setRoot(t);
        tree->setMax(maxAndEdge.first);
        tree->setIndex(maxAndEdge.second);
        tree->setVertexIndex("R");

        if( tree->getLB() + r + tree->getMax() < min_sol ){
            M[tree->getIndex().first][tree->getIndex().second].first = GINFINITY;
            tree->setLB(tree->getLB() + tree->getMax() + r);
            tree->setEdge(getConnectionFromIndex(M, tree));
            t->setRightSon(tree);
            TraverseTree(M, tree);                           //RIGTH SON
        }

    }else return;
}

Tree::Tree(QVector<QVector<QPair<int, Vertex *> > > m, GraphAplorithms *algo): M(m)
{
    leftSon = nullptr;
    rigthSon = nullptr;
    root = nullptr;
    LB = 0;       //Init value
    max = 0;
    createPatternMatrix(algo);
}

Tree::Tree(QVector<QVector<QPair<int, Vertex *> > > m, Tree* t): M(m)
{
    patternMatrixForI = t->patternMatrixForI;
    patternMatrixForJ = t->patternMatrixForJ;
    LB = t->getLB();
    max = t->getMax();
    index = t->getIndex();
    leftSon = nullptr;
    rigthSon = nullptr;
    root = nullptr;
}

void Tree::createPatternMatrix(GraphAplorithms*s)
{
    for(auto i : s->getSortedMatrix()){
        Vertex *v = i.second;
        if(v){
            patternMatrixForI.push_back(v->getVertexID());
            patternMatrixForJ.push_back(v->getVertexID());
        }
    }
}

void Tree::setLeftSon(Tree *l)
{
    leftSon = l;
}

void Tree::setRightSon(Tree *r)
{
    rigthSon = r;
}

void Tree::setRoot(Tree *r)
{
    root = r;
}

void Tree::setEdge(ConnectionWay *e)
{
    edge.append(e);
}

void TSPAlgorithm::simpleTSP(Vertex*v)
{
    Vertex* start = algo->getSortedMatrix()[0].second;
    Vertex *u = nullptr;
    Sh.append(v);
    if(Sh.count() < algo->getSortedMatrix().count()){
        v->setVisited(true);
        for(auto i : v->getConnectionVector()){
            u = scene->getHashTable()[i.second];
            if(!u->isVisited()){
                ConnectionWay* c = getConnection(v,u);
                dh += c->getWeight();
                simpleTSP(u);
                dh -= c->getWeight();
            }
        }
        v->setVisited(false);
    }else if(getConnection(start, v)){
        dh += getConnection(start, v)->getWeight();
        if(dh < min_sol){
            min_sol = dh;
            while (!Sh.isEmpty()){
                S.append(Sh.top());
                Sh.pop();
            }
            dh -= getConnection(start, v)->getWeight();
        }
    }
    if(!Sh.empty()) Sh.pop();
}







