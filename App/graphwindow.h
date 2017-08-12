#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H
#include "myscene.h"

#include <QDialog>
class MyScene;

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GraphWindow(MyScene*, QWidget *parent = 0);
    ~GraphWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::GraphWindow *ui;
    MyScene *scene;
};

#endif // GRAPHWINDOW_H
