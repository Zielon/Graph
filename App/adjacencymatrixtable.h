#ifndef ADJACENCYMATRIXTABLE_H
#define ADJACENCYMATRIXTABLE_H

#include <QDialog>
#include <QPair>
#include <QGraphicsLineItem>
#include <QMessageBox>
#include <QGraphicsEllipseItem>
#include <QGraphicsObject>
#include <QVariant>

#include "myscene.h"
#include "vertex.h"
#include "enum.h"

class MyScene;
class Vertex;
namespace Ui {
class AdjacencyMatrixTable;
}

class AdjacencyMatrixTable : public QDialog
{
    Q_OBJECT

public:
    explicit AdjacencyMatrixTable(MyScene*, QWidget *parent = 0);
    ~AdjacencyMatrixTable();

private:
    Ui::AdjacencyMatrixTable *ui;
    MyScene* scene;
    QStandardItemModel *model;
    QModelIndex index;
};

#endif // ADJACENCYMATRIXTABLE_H
