#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphwindow.h"
#include "graphwindowbellmanford.h"
#include "serializatorxml.h"
#include "dirmodelsave.h"
#include "dirmodelread.h"
#include "adjacencymatrixtable.h"
#include "graphaplorithms.h"
#include "graphwindowtsp.h"
#include "grapheulerwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Graph Application");
    scene = new MyScene(parent, this, ui->graphicsView);
    ui->graphicsView->setScene(scene);

    // CustomView *view = new CustomView(this, scene, this);
    // view->setGeometry(QRect(35, 35, 900, 500));
    // view->setScene(scene);
    //view->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCoordsInMainWindowMouse(QString mouse)
{
    ui->label_2->setText(mouse);
}

void MainWindow::setCoordsInMainWindowVertex(QString vertex)
{
    ui->label->setText(vertex);
}

void MainWindow::setEdgesCount(QString s)
{
    ui->label_5->setText(s);
}

void MainWindow::setVertexesCount(QString s)
{
    ui->label_6->setText(s);
}

void MainWindow::on_actionMenu_triggered()
{
    GraphWindow *graphWindow = new GraphWindow(scene, this);
    graphWindow->show();
}

void MainWindow::on_actionZapisz_triggered()
{
    DirModelSave *dir = new DirModelSave(scene, this);
    dir->show();
}

void MainWindow::on_actionOdczytaj_triggered()
{
    DirModelRead *dir = new DirModelRead(scene, this);
    dir->show();
}

void MainWindow::on_pushButton_clicked()
{
    scene->clear();
    if(scene->getHashTable()["1"]){
        scene->getHashTable()["1"]->setVertexNumber(1);
    }
    scene->updateCounter(false);
    scene->getHashTable().clear();
    scene->update();
}

void MainWindow::on_actionBellman_ford_triggered()
{
    GraphWindowBellmanFord *graphWindow = new GraphWindowBellmanFord(scene, this);
    graphWindow->show();
}

void MainWindow::on_actionAdjacency_Matrix_triggered()
{
    AdjacencyMatrixTable *tab = new AdjacencyMatrixTable(scene, this);
    tab->show();
}

void MainWindow::on_actionTSP_triggered()
{
    graphwindowTSP *tsp = new graphwindowTSP(scene, this);
    tsp->show();
}

void MainWindow::on_actionEuler_Cycle_triggered()
{
    graphEulerwindow *euler = new graphEulerwindow(scene, this);
    euler->show();
}
