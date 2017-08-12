#ifndef DIRMODELSAVE_H
#define DIRMODELSAVE_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QDirModel>
#include "myscene.h"

class MyScene;
namespace Ui {
class DirModelSave;
}

class DirModelSave : public QDialog
{
    Q_OBJECT

public:
    explicit DirModelSave(MyScene*, QWidget *parent = 0);
    ~DirModelSave();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::DirModelSave *ui;
    MyScene* scene;
    QDirModel* model;
};

#endif // DIRMODELSAVE_H
