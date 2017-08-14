#include "graphwindow.h"
#include "ui_graphwindow.h"
#include "graphaplorithms.h"


//Dijkstra dialog window
//
GraphWindow::GraphWindow(MyScene* s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphWindow), scene(s)
{
    ui->setupUi(this);
    this->setWindowTitle("Dijkstra");
}


GraphWindow::~GraphWindow()
{
    delete ui;
}

void GraphWindow::on_pushButton_clicked()
{
    QString source = ui->lineEdit->text();
    QString destination = ui->lineEdit_2->text();

    bool isSource = Q_NULLPTR;
    bool isDestination = Q_NULLPTR;
    source.toInt(&isSource); destination.toInt(&isDestination);

    if(!isSource || !isDestination) {
        ui->plainTextEdit->appendPlainText("Provide the correct vertexs number !");
        return;
    }
    else ui->plainTextEdit->clear();

    GraphAplorithms *algorithms = new GraphAplorithms(scene);
    algorithms->Dijkstra(scene->getHashTable()[source]);
    const QString s = algorithms->printShortestPath(scene->getHashTable()[source], scene->getHashTable()[destination]);
    ui->plainTextEdit->appendPlainText(s);
}

void GraphWindow::on_pushButton_2_clicked()
{
    GraphAplorithms *algorithms = new GraphAplorithms(scene);
    algorithms->cleanUpGraph();
    ui->plainTextEdit->clear();
}
