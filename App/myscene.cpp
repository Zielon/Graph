#include "myscene.h"
#include "graphaplorithms.h"
#include <QMessageBox>
#include <QWidgetAction>
#include <QLineEdit>

//Factory object to produce vertex and egdes on scene
//
MyScene::MyScene(QWidget *parent, MainWindow* w, QGraphicsView*v): QGraphicsScene(parent), window(w), view(v)
{
    QMainWindow *main = static_cast<QMainWindow*>(parent);
    MakingConnectionState = false;
    mousePressed = false;
}

//Add to scene and hash table vertex. Slot for menu
//
void MyScene::addVetrex()
{
    Vertex *v = new Vertex(this);
    QPointF mousePos = v->mapToScene(coords);
    v->setPos(mousePos.x(),mousePos.y());
    vertexHashTable[v->getVertexID()]= v;
    updateCounter();
    addItem(v);
}

//Function which is creating temporary line during making connection between vertexes
//In the end temporary line is deleteing from scene
//
void MyScene::makeConnection(QString id)
{
    MakingConnectionState = true;
    temporaryLine = new QGraphicsLineItem();
    addItem(temporaryLine);
    selectedVertexID = id;
    Vertex *v = vertexHashTable[selectedVertexID];
    v->setSelected();
    QVector<QPair<ConnectionWay*, QString > > vector = v->getConnectionVector();
    for(QPair<ConnectionWay *, QString> i : vector){
        (i.first)->setSelectedConnection();
        update();
    }
}

//Disable making connection mode. Vertex color is changed form red to gray
//
void MyScene::unmakeConnection()
{
    MakingConnectionState = false;
    Vertex *v = vertexHashTable[selectedVertexID];
    removeItem(temporaryLine);
    v->unsetSelected();
    QVector<QPair<ConnectionWay*, QString > > vector = v->getConnectionVector();
    for(QPair<ConnectionWay *, QString> i : vector){
        (i.first)->unsetSelectedConnection();
        update();
    }
}

QHash<QString, Vertex *> &MyScene::getHashTable()
{
    return vertexHashTable;
}

void MyScene::setHashTable(QHash<QString, Vertex *> tab)
{
    vertexHashTable = tab;
}

//Warning message box
//
void MyScene::warningMessage(const QString &msg)
{
    QMessageBox::warning(window, "Warning!", msg, QMessageBox::Ok);
}

int MyScene::getWeight(QPointF pos)
{
    QMenu *menu = new QMenu();
    QLineEdit *edit = new QLineEdit(menu);
    QWidgetAction *a1 = new QWidgetAction(menu);
    a1->setDefaultWidget(edit);
    menu->addAction(a1);
    menu->exec(QCursor::pos());
    QValidator *validator = new QIntValidator(1, 999, this);
    edit->setValidator(validator);
    int w = edit->text().toInt();
    if(w > 0)
        return w;
}

//Shows table with vertex neighbours and allows to change weight between them
//
void MyScene::weigthChange(QString id)
{
    Vertex* v = vertexHashTable[id];
    WeigthsTable *table = new WeigthsTable(v, this, window);
    table->show();
    update();
}

void MyScene::setCoords(QPointF c)
{
    coords = c;
}

void MyScene::updateVertexesCoords()
{
    QHashIterator<QString, Vertex*> vertex(vertexHashTable);
    while(vertex.hasNext()){
        vertex.next();
        //vertex.value()->setCoords(vertex.value()->mapToScene(vertex.value()->boundingRect().center()));
        vertex.value()->setCoords(vertex.value()->pos());
    }
}

//Update edges and vertex count on windows view in MainWindow object
//
void MyScene::updateCounter(bool s)
{
    if(!s){
        window->setEdgesCount("0");
        window->setVertexesCount("0");
    }else{
        GraphAplorithms *counter = new GraphAplorithms(this);
        window->setEdgesCount(QString::number(counter->edgesCount()));
        window->setVertexesCount(QString::number(vertexHashTable.count()));
    }
}

//Deleting vertex with all of connections in neighbours list
//
void MyScene::deleteVertex()
{
    Vertex *v = vertexHashTable[selectedVertexID];
    vertexHashTable.remove(selectedVertexID);
    int temp = 0;
    for(auto i : v->getConnectionVector()){
        Vertex *u = vertexHashTable[i.second];
        int index = 0;
        for(auto j : u->getConnectionVector()){
            if(j.second == v->getVertexID()){
                removeItem(j.first);
                u->getConnectionVector().remove(index);
                break;
            }
            index++;
        }
        temp++;
    }
    selectedVertexID = "";
    removeItem(v);
    updateCounter();
    update();
}

//If making connection state is set on true allows to make connection between vertexes,
//Create ConnectionWay object which is an edge between them
//
void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(MakingConnectionState){
        QPointF selfCoords;
        QPointF destinationCoords;
        QGraphicsItem *toCast = itemAt(event->scenePos().x(), event->scenePos().y(), QTransform());
        if(toCast){
            Vertex *v;
            ConnectionWay *c;
            if((v = dynamic_cast<Vertex *>(toCast))  && v->className() == "Vertex" ){

                Vertex *from = vertexHashTable[selectedVertexID];
                if(from->getVertexID() != v->getVertexID())
                {
                    if(from->isConnected(v->getVertexID())){
                        int weight = getWeight(v->pos());
                        selfCoords = v->mapToScene(v->boundingRect().center());
                        destinationCoords = from->mapToScene(from->boundingRect().center());
                        ConnectionWay *line = new ConnectionWay(qMakePair(selfCoords, v), qMakePair(destinationCoords, from), weight);
                        from->addToConnectionVector(line, v->getVertexID());
                        v->addToConnectionVector(line, from->getVertexID());
                        addItem(line);
                        updateCounter();
                        unmakeConnection();
                    }else{
                        warningMessage("Already connected !");
                    }
                }

            }else if((c = dynamic_cast<ConnectionWay *>(toCast)) && c->className() == "ConnectionWay"){

            }
        }
        update();
        unmakeConnection();
        removeItem(temporaryLine);
    }
    update();
    mousePressed = true;
    QGraphicsScene::mousePressEvent(event);
}

//If making connection state is set function change temporary line posision to mouse pos()
//
void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem *toCast = itemAt(event->scenePos().x(), event->scenePos().y(), QTransform());
    if(MakingConnectionState){
        QPen pen(Qt::red);
        Vertex *from = vertexHashTable[selectedVertexID];
        QPointF me = from->mapToScene(from->boundingRect().center());
        temporaryLine->setLine(me.x(), me.y(), event->scenePos().x(), event->scenePos().y());
        Vertex *v;
        if(!(v = dynamic_cast<Vertex *>(toCast))){
            pen.setColor(Qt::black);
        }
        temporaryLine->setPen(pen);
        update();
    }

    if(toCast && mousePressed){
        Vertex *v;
        ConnectionWay *c;
        if((v = dynamic_cast<Vertex *>(toCast))  && v->className() == "Vertex" ){
            QString msg = "Press    x: " + QString::number(event->scenePos().x()) + " y: " + QString::number(event->scenePos().y());
            window->setCoordsInMainWindowMouse(msg);
            v->setCoords(v->pos());
            QVector<QPair<ConnectionWay*, QString > > vector = v->getConnectionVector();
            QVariant qV = QVariant::fromValue(v);
            for(QPair<ConnectionWay *, QString> i : vector){
                (i.first)->itemChange(QGraphicsItem::ItemPositionChange, qV);
                update();
            }
        }
        else if((c = dynamic_cast<ConnectionWay *>(toCast)) && c->className() == "ConnectionWay"){

        }
    }
    else
    {
        QString msg = "Mouse  x: " + QString::number(event->scenePos().x()) + " y: " + QString::number(event->scenePos().y());
        window->setCoordsInMainWindowMouse(msg);
    }
    update();
    QGraphicsScene::mouseMoveEvent(event);
}


void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem *toCast = itemAt(event->scenePos().x(), event->scenePos().y(), QTransform());
    if(toCast){
        Vertex *v;
        ConnectionWay *c;
        if((v = dynamic_cast<Vertex *>(toCast))  && v->className() == "Vertex" ){

            QPointF mapped = v->mapToScene(v->boundingRect().center());
            QString msg = "Vertex " + v->getVertexID() +  " x: " + QString::number(v->pos().x()) +" y: " + QString::number(v->pos().y());
            window->setCoordsInMainWindowVertex(msg);

        }
        else if((c = dynamic_cast<ConnectionWay *>(toCast)) && c->className() == "ConnectionWay"){
            QString coords = "ConnectionWay  x: " + QString::number(c->pos().x()) +" y: " + QString::number(c->pos().y());
            window->setCoordsInMainWindowVertex(coords);
        }
    }
    update();
    mousePressed = false;
    QGraphicsScene::mouseReleaseEvent(event);
}
void MyScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsScene::mouseDoubleClickEvent(event);
}

//Contex menu which is activated when mouse is on object in scene
//
void MyScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QGraphicsItem *toCast = itemAt(event->scenePos().x(), event->scenePos().y(), QTransform());
    if(toCast){
        Vertex *v;
        ConnectionWay *c;
        if((v = dynamic_cast<Vertex *>(toCast))  && v->className() == "Vertex" ){
            QString vertexid = v->getVertexID();
            QAction *a1 = menu.addAction("Make Connection");
            QAction *a3 = menu.addAction("Change weigths");
            QAction *a4 = menu.addAction("Delete Vertex");

            QSignalMapper *mapper = new QSignalMapper(this);
            QSignalMapper *mapper1 = new QSignalMapper(this);

            selectedVertexID = vertexid;
            connect(a4, SIGNAL(triggered(bool)),this, SLOT(deleteVertex()));

            connect(a1, SIGNAL(triggered(bool)), mapper, SLOT(map()));
            mapper->setMapping(a1, vertexid);
            connect(mapper, SIGNAL(mapped(QString)), this, SLOT(makeConnection(QString)));

            connect(a3, SIGNAL(triggered(bool)), mapper1, SLOT(map()));
            mapper1->setMapping(a3, vertexid);
            connect(mapper1, SIGNAL(mapped(QString)), this, SLOT(weigthChange(QString)));
            menu.exec(QCursor::pos());
        }
        else if((c = dynamic_cast<ConnectionWay *>(toCast)) && c->className() == "ConnectionWay"){

        }
    }
    else
    {
        QAction *a2 = menu.addAction("Add Vertex");
        const QPointF p = event->scenePos();

        QString c = "Menu pos  x: " + QString::number(p.x()) +" y: " + QString::number(p.y());
        window->setCoordsInMainWindowMouse(c);
        setCoords(QPointF(p.x(), p.y()));

        connect(a2, SIGNAL(triggered(bool)), this, SLOT(addVetrex()));
        menu.exec(QCursor::pos());
    }
    update();
    QGraphicsScene::contextMenuEvent(event);
}
