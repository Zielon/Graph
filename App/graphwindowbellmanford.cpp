#include "graphwindowbellmanford.h"
#include "ui_graphwindowbellmanford.h"
#include "graphaplorithms.h"

//BellmanFord dialog window
//
GraphWindowBellmanFord::GraphWindowBellmanFord(MyScene* s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphWindowBellmanFord), scene(s)
{
    ui->setupUi(this);
    this->setWindowTitle("Bellman-Ford");
}

GraphWindowBellmanFord::~GraphWindowBellmanFord()
{
    delete ui;
}

void GraphWindowBellmanFord::on_pushButton_9_clicked()
{
    QString source = ui->lineEdit_9->text();
    QString destination = ui->lineEdit_10->text();

    bool isSource = Q_NULLPTR;
    bool isDestination = Q_NULLPTR;
    source.toInt(&isSource); destination.toInt(&isDestination);

    if(!isSource || !isDestination) {
        ui->plainTextEdit_5->appendPlainText("Provide the correct vertexs number !");
        return;
    }
    else ui->plainTextEdit_5->clear();

    GraphAplorithms *algorithms = new GraphAplorithms(scene);
    if(algorithms->BellmanFord(scene->getHashTable()[source])){
        const QString s = algorithms->printShortestPath(scene->getHashTable()[source], scene->getHashTable()[destination]);
        ui->plainTextEdit_5->appendPlainText(s);
    }
    else{
        ui->plainTextEdit_5->appendPlainText("Negativ weight in path");
    }
}

void GraphWindowBellmanFord::on_pushButton_10_clicked()
{
    GraphAplorithms *algorithms = new GraphAplorithms(scene);
    algorithms->cleanUpGraph();
    ui->plainTextEdit_5->clear();
}
