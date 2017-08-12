#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QList>
#include <QVector>
#include <QPair>
#include <QGraphicsLineItem>
#include <QMessageBox>
#include <QGraphicsEllipseItem>
#include <QGraphicsObject>
#include <QtCore>
#include <QtGui>
#include "vertex.h"
#include "connectionway.h"
#include "myscene.h"

class MyScene;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setCoordsInMainWindowMouse(QString);
    void setCoordsInMainWindowVertex(QString);
    void setEdgesCount(QString);
    void setVertexesCount(QString);

private slots:
    void on_actionMenu_triggered();

    void on_actionZapisz_triggered();

    void on_actionOdczytaj_triggered();

    void on_pushButton_clicked();

    void on_actionBellman_ford_triggered();

    void on_actionAdjacency_Matrix_triggered();

    void on_actionTSP_triggered();

    void on_actionEuler_Cycle_triggered();

private:
    Ui::MainWindow *ui;
    MyScene *scene;
};

#endif // MAINWINDOW_H
