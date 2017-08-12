#ifndef SERIALIZATORXML_H
#define SERIALIZATORXML_H
#include <QGraphicsScene>
#include <QtCore>
#include <QtGui>
#include <QVector>
#include <QMessageBox>
#include <QHashIterator>

#include <QtXml/QDomDocument>

#include "vertex.h"
#include "connectionway.h"
#include "mainwindow.h"
#include "weigthstable.h"
#include "customview.h"

class CustomView;
class Vertex;
class ConnectionWay;
class MainWindow;

class SerializatorXML
{
public:
    SerializatorXML(MyScene*);
    void serializeToXMLFile(QString);
    void retriveFromXMLFile(QString);
    QHash<QString, Vertex*> getHashTable();

private:
    MyScene *scene;
    QHash<QString, Vertex*> hashTable;
};

#endif // SERIALIZATORXML_H
