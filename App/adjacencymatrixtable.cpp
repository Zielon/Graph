#include "adjacencymatrixtable.h"
#include "ui_adjacencymatrixtable.h"
#include "graphaplorithms.h"

AdjacencyMatrixTable::AdjacencyMatrixTable(MyScene*s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdjacencyMatrixTable), scene(s)
{
    ui->setupUi(this);
    GraphAplorithms *algorithms = new GraphAplorithms(scene);
    model = new QStandardItemModel(algorithms->getAdjacencyMatrix().count(),algorithms->getAdjacencyMatrix().count(),this);
    int p = 0;
    this->setWindowTitle("The Adjacency Matrix Table");
    for(auto c : algorithms->getSortedMatrix())
    {
        model->setHorizontalHeaderItem(p, new QStandardItem(c.first));
        model->setVerticalHeaderItem(p, new QStandardItem(c.first));
        p++;
    }
    int size = algorithms->getAdjacencyMatrix().count();
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++){
            index = model->index(i,j,QModelIndex());
            Vertex* v = algorithms->getAdjacencyMatrix()[i][j].second;
            int w = algorithms->getAdjacencyMatrix()[i][j].first;
            QString data;
            if(v != nullptr){
                //data += "W: "+ QString::number(w) + " Vertex ID: " + v->getVertexID();
                data += QString::number(w);
                QStandardItem* item = new QStandardItem(data);
                item->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
                model->setItem(index.row(), index.column(), item);

            }else if(i == j){
                data = "∞";
                QStandardItem* item = new QStandardItem(data);
                item->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
                item->setData(QVariant(QBrush(Qt::darkGray)), Qt::BackgroundRole);
                model->setItem(index.row(), index.column(), item);
            }
            else if(w == 999999999){
                data = "∞";
                QStandardItem* item = new QStandardItem(data);
                item->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
                item->setData(QVariant(QBrush(Qt::gray)), Qt::BackgroundRole);
                model->setItem(index.row(), index.column(), item);
            }
            data.clear();
        }
    ui->tableView->setModel(model);
}

AdjacencyMatrixTable::~AdjacencyMatrixTable()
{
    delete ui;
}

