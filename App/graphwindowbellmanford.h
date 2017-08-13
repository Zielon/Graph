#ifndef GRAPHWINDOWBELLMANFORD_H
#define GRAPHWINDOWBELLMANFORD_H

#include <QDialog>
#include "myscene.h"

class MyScene;
namespace Ui {
class GraphWindowBellmanFord;
}

class GraphWindowBellmanFord : public QDialog
{
    Q_OBJECT

public:
    explicit GraphWindowBellmanFord(MyScene*, QWidget *parent = 0);
    ~GraphWindowBellmanFord();

private slots:
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();

private:
    Ui::GraphWindowBellmanFord *ui;
    MyScene* scene;
};

#endif // GRAPHWINDOWBELLMANFORD_H
