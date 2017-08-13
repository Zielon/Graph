#ifndef GRAPHWINDOWTSP_H
#define GRAPHWINDOWTSP_H

#include <QDialog>
#include "myscene.h"
#include "graphaplorithms.h"
#include <string>


namespace Ui {
class graphwindowTSP;
}

class graphwindowTSP : public QDialog
{
    Q_OBJECT

public:
    explicit graphwindowTSP(MyScene*, QWidget *parent = 0);
    ~graphwindowTSP();
    void printBT(QString sp, QString sn, Tree * v);
    QString cutString(int, QString sp);
    void createVector(Tree*t);
    void showMatrix(Tree*t);
    ConnectionWay* getConnection(Vertex *from, Vertex *to);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_comboBox_activated();
    void on_pushButton_4_clicked();

private:
    Ui::graphwindowTSP *ui;
    MyScene* scene;
    GraphAplorithms *algorithms;
    QString cr, cl, cp;
    bool clicked;
    QVector<QVector<ConnectionWay*> > vectorOfConnections;
    QVector<ConnectionWay*> vectorTemp;
    QVector<Tree*> solutionVector;
};

#endif // GRAPHWINDOWTSP_H
