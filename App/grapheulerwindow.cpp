#include "grapheulerwindow.h"
#include "ui_grapheulerwindow.h"

graphEulerwindow::graphEulerwindow(MyScene* s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::graphEulerwindow), scene(s)
{
    ui->setupUi(this);
    this->setWindowTitle("Euler cycle");
    algorithms = new GraphAplorithms(scene);
    flag = false;
}

graphEulerwindow::~graphEulerwindow()
{
    delete ui;
}

void graphEulerwindow::printEdges(QString a, QString b)
{
    Vertex* v = algorithms->getHashTable()[a];
    Vertex* u = algorithms->getHashTable()[b];
    for(auto i : v->getConnectionVector()){
        if(i.second == u->getVertexID()){
            i.first->setShortestWayMade(true);
            scene->update();
            return;
        }
    }
}

void graphEulerwindow::on_pushButton_clicked()
{
    QString source = ui->lineEdit->text();
    bool isNumber = Q_NULLPTR;
    source.toInt(&isNumber);

    if(source.isEmpty() || !isNumber || !scene->getHashTable().contains(source)) {
        ui->plainTextEdit->appendPlainText("Provide the correct start vertex number !");
        return;
    }
    else ui->plainTextEdit->clear();

    flag = algorithms->euler(source);
    s = algorithms->getStack();
    if(flag){
        QString vertex;
        ui->plainTextEdit->appendPlainText("Grpah has Euler's cycle");
        ui->plainTextEdit->appendPlainText("Way to get cycle:");
        QString a, b;
        while(!algorithms->getStack().empty()){
            a = algorithms->getStack().top()->getVertexID();
            vertex = "Vertex " + a;
            ui->plainTextEdit->appendPlainText(vertex);
            algorithms->getStack().pop();
        }
    }else{
        ui->plainTextEdit->appendPlainText("Grpah has not Euler's cycle !");
    }
}

void graphEulerwindow::on_pushButton_2_clicked()
{
    ui->plainTextEdit->clear();
    algorithms->cleanUpGraph();
    flag = false;
}

void graphEulerwindow::on_pushButton_3_clicked()
{
    Sleeper *sleep = new Sleeper(s,this,flag);
    sleep->start();
}


graphEulerwindow::Sleeper::Sleeper(QStack<Vertex*> & s, graphEulerwindow*e, bool f, QObject *parent): flag(f), stos(s), euler(e){}

void graphEulerwindow::Sleeper::run()
{
    if(flag){
        QString a, b;
        while(!stos.empty()){
            a = stos.top()->getVertexID();
            stos.pop();
            if(!stos.empty()){
                b = stos.top()->getVertexID();
                euler->printEdges(a,b);
            }
            msleep(700);
        }
    }
}
