#include "serializatorxml.h"

#include <QtXml/QDomElement>

//The Object to serialize to xml a file
//
SerializatorXML::SerializatorXML(MyScene* s): scene(s){}

void SerializatorXML::serializeToXMLFile(QString filepath)
{
    scene->updateVertexesCoords();
    QFile file(filepath);
    QTextStream out(&file);
    if(file.open(QIODevice::ReadWrite)){

        out << "<SceneElements>" << endl;
        out << "<vertexNumber>"<< scene->getHashTable()["1"]->getVertexNumber() << "</vertexNumber>" << endl;
        QHashIterator<QString, Vertex*> vertex(scene->getHashTable());
        while(vertex.hasNext()){
            vertex.next();
            out << "<Vertex>" <<endl;
            out << "    <myCoordsX>" << vertex.value()->getCoords().x() << "</myCoordsX>" << endl;
            out << "    <myCoordsY>" << vertex.value()->getCoords().y() << "</myCoordsY>" << endl;
            out << "    <vertexID>" <<vertex.value()->getVertexID() << "</vertexID>" << endl;
            out << "    <connectionVector>" << endl;
            QVector<QPair<ConnectionWay*, QString > >::iterator i;
            for(i = vertex.value()->getConnectionVector().begin(); i !=vertex.value()->getConnectionVector().end(); ++i){
                out << "        <vectorQPairElement>" << endl;
                out << "                <vertexFrom>" << endl;
                out << "                    <vertex>" << (*i).first->getVertexFrom().second->getVertexID() <<  "</vertex>"<< endl;
                out << "                    <XpointF>" << (*i).first->getVertexFrom().first.x() <<  "</XpointF>"<< endl;
                out << "                    <YpointF>" << (*i).first->getVertexFrom().first.y() <<  "</YpointF>"<< endl;
                out << "                </vertexFrom>" << endl;
                out << "                <vertexTo>" << endl;
                out << "                    <vertex>" << (*i).first->getVertexTo().second->getVertexID() <<  "</vertex>"<< endl;
                out << "                    <XpointF>" << (*i).first->getVertexTo().first.x() <<  "</XpointF>"<< endl;
                out << "                    <YpointF>" << (*i).first->getVertexTo().first.y() <<  "</YpointF>"<< endl;
                out << "                </vertexTo>" << endl;
                out << "                <weight>" << (*i).first->getWeight() << "</weight>" << endl;
                out << "        </vectorQPairElement>" << endl;
            }

            out << "    </connectionVector>" << endl;
            out<< "</Vertex>" <<endl;

        }
        out << "</SceneElements>" << endl;
    }
    file.close();
}

void SerializatorXML::retriveFromXMLFile(QString filepath)
{
    QFile file(filepath);
    QDomDocument doc;
    if(file.open(QIODevice::ReadOnly)){
        doc.setContent(&file);
        file.close();
    }

    QDomElement root = doc.firstChildElement();
    QDomNodeList vertexes = root.elementsByTagName("Vertex");
    for(int i = 0; i < vertexes.count(); ++i){
        QDomNode vertex = vertexes.at(i);
        int x = vertex.firstChildElement("myCoordsX").text().toInt();
        int y = vertex.firstChildElement("myCoordsY").text().toInt();
        QString id = vertex.firstChildElement("vertexID").text();
        Vertex *v = new Vertex(scene);
        v->setCoords(QPointF(x,y));
        v->setVertexID(id);
        hashTable[id] = v;
    }
    // init vertex object from xml file
    QVector<ConnectionWay *> wayVector;
    for(int i = 0; i < vertexes.count(); ++i){
        QDomNode vertex = vertexes.at(i);
        QDomElement v = vertex.firstChildElement("connectionVector");
        QDomNodeList connectionVector = v.elementsByTagName("vectorQPairElement");
        int count = connectionVector.count();
        int j = 0;
        while(j < count){
            QDomNode pair = connectionVector.at(j);
            int weigth = pair.firstChildElement("weight").text().toInt();

            QDomElement vertexFrom = pair.firstChildElement("vertexFrom");
            int fromId = vertexFrom.firstChildElement("vertex").text().toInt();
            int fromX = vertexFrom.firstChildElement("XpointF").text().toInt();
            int fromY = vertexFrom.firstChildElement("YpointF").text().toInt();

            QDomElement vertexTo = pair.firstChildElement("vertexTo");
            int toId = vertexTo.firstChildElement("vertex").text().toInt();
            int toX = vertexTo.firstChildElement("XpointF").text().toInt();
            int toY = vertexTo.firstChildElement("YpointF").text().toInt();

            Vertex *from = hashTable[QString::number(fromId)];
            Vertex *to = hashTable[QString::number(toId)];
            ConnectionWay* wayBetweenVertexes = new ConnectionWay(qMakePair(QPointF(fromX,fromY), from),qMakePair(QPointF(toX,toY),to), weigth);

            //Add only unique connection to vertex neighbours list
            //
            int unique = 0;
            for(auto i : wayVector){
                if(!i->isCoordsAreEqual(wayBetweenVertexes)){
                    unique++;
                }
            }
            if(unique == wayVector.count()){
                wayVector.append(wayBetweenVertexes);
                if(from->isConnected(QString::number(toId))){
                    from->addToConnectionVector(wayBetweenVertexes, QString::number(toId));
                }

                if(to->isConnected(QString::number(fromId))){
                    to->addToConnectionVector(wayBetweenVertexes, QString::number(fromId));
                }
            }
            j++;
        }
    }

    // set pos to vertex on scene
    for(auto w : wayVector){
        scene->addItem(w);
    }
    QHashIterator<QString, Vertex*> vertex(hashTable);
    while(vertex.hasNext()){
        vertex.next();
        vertex.value()->setPos(vertex.value()->getCoords().x(), vertex.value()->getCoords().y());
        scene->addItem(vertex.value());
        scene->update();
    }

    scene->setHashTable(hashTable);
    scene->updateCounter();
}

QHash<QString, Vertex *> SerializatorXML::getHashTable()
{
    return hashTable;
}









