#ifndef WEIGTHSTABLE_H
#define WEIGTHSTABLE_H

#include <QDialog>
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
#include <QVariant>

#include "myscene.h"
#include "vertex.h"
#include "enum.h"

class Vertex;
class MyScene;

namespace Ui {
class WeigthsTable;
}

class WeigthsTable : public QDialog
{
    Q_OBJECT

public:
    explicit WeigthsTable(Vertex*, MyScene*, QWidget *parent = 0);
    ~WeigthsTable();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_label_linkActivated(const QString &link);

private:
    void updateT();
    Vertex* vertex;
    MyScene* scene;
    QStandardItemModel *model;
    Ui::WeigthsTable *ui;
    QModelIndex index1;
    QModelIndex index2;
    QVector<QPair<ConnectionWay*, QString > > vector;

    //Custom model for disable first column in table
    //
    class Table : public QStandardItemModel{
        public:
            Table(int row, int col, QObject *parent = 0): QStandardItemModel(row, col, parent){}
            Qt::ItemFlags flags ( const QModelIndex & index ) const
            {
              if (index.column() == 0)
                  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
              else
                  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
            }
    };
};

#endif // WEIGTHSTABLE_H
