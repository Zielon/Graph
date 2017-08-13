#include "weigthstable.h"
#include "ui_weigthstable.h"


//The Small dialog window with vertex neighbours table and weigths
//
WeigthsTable::WeigthsTable(Vertex *v, MyScene* s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WeigthsTable), vertex(v), scene(s)
{
    ui->setupUi(this);
    this->setWindowTitle("Change weigths");
    ui->label->setText("Selected Vertex " + v->getVertexID());
    vector = v->getConnectionVector();
    model = new Table(vector.length(),2,this);

    model->setHorizontalHeaderItem(0, new QStandardItem("Neighbours"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Weights"));

    for(int row = 0; row < vector.length(); ++row ){
        index1 = model->index(row,1,QModelIndex());
        model->setData(index1, (vector[row].first)->getWeight());
    }
    for(int row = 0; row < vector.length(); ++row ){
        index2 = model->index(row,0,QModelIndex());
        model->setData(index2, "Vertex: " + vector[row].second,2);

    }
    model->flags(index2);
    ui->tableView->setModel(model);

}


WeigthsTable::~WeigthsTable()
{
    delete ui;
}


void WeigthsTable::on_pushButton_2_clicked()
{
    model->clear();
}

void WeigthsTable::on_pushButton_clicked()
{
    updateT();
}

void WeigthsTable::on_pushButton_3_clicked()
{
    updateT();
    close();
}

void WeigthsTable::updateT()
{
    int i = 0;
    for(int row = 0; row < vector.length(); ++row ){
        i = model->data(model->index(row,1)).toInt();
        (vector[row].first)->setWeigth(i);
        scene->update();
    }
}

void WeigthsTable::on_label_linkActivated(const QString &link)
{

}
