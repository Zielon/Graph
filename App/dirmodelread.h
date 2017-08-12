#ifndef DIRMODELREAD_H
#define DIRMODELREAD_H


#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QDirModel>
#include "myscene.h"

class MyScene;
namespace Ui {
class DirModelRead;
}

class DirModelRead : public QDialog
{
    Q_OBJECT

public:
    explicit DirModelRead(MyScene*, QWidget *parent = 0);
    ~DirModelRead();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DirModelRead *ui;
    MyScene* scene;
    QDirModel* model;
};

#endif // DIRMODELREAD_H
