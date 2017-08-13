#ifndef GRAPHEULERWINDOW_H
#define GRAPHEULERWINDOW_H

#include <QDialog>
#include "myscene.h"
#include "graphaplorithms.h"

namespace Ui {
class graphEulerwindow;
}

class graphEulerwindow : public QDialog
{
    Q_OBJECT

public:
    explicit graphEulerwindow(MyScene*, QWidget *parent = 0);
    ~graphEulerwindow();
    void printEdges(QString, QString);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::graphEulerwindow *ui;
    MyScene* scene;
    GraphAplorithms *algorithms;
    QStack<Vertex*> s;
    bool flag;

    class Sleeper : public QThread
    {
    public:
        Sleeper(QStack<Vertex*> &, graphEulerwindow *e, bool, QObject *parent = 0);
        QStack<Vertex*> & stos;
        graphEulerwindow* euler;
        bool flag;
    protected:
        void run();
    };
};

#endif // GRAPHEULERWINDOW_H
