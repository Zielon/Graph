#include "dirmodelsave.h"
#include "ui_dirmodelsave.h"
#include "serializatorxml.h"
#include <QMessageBox>

// Dialog window which is using QDirModel to navigate between files

DirModelSave::DirModelSave(MyScene* s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DirModelSave), scene(s)
{
    ui->setupUi(this);
    this->setWindowTitle("Save graph");
    model = new QDirModel(this);
    model->setReadOnly(false);
    model->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);

    ui->treeView->setModel(model);

    QModelIndex index = model->index("C:/Users/Wojtek/Desktop/SavedGraphs");
    ui->treeView->expand(index);
    ui->treeView->scrollTo(index);
    ui->treeView->setCurrentIndex(index);
    ui->treeView->resizeColumnToContents(0);
}

DirModelSave::~DirModelSave()
{
    delete ui;
}

void DirModelSave::on_pushButton_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(!index.isValid()) return;
    QString name = ui->lineEdit->text();
    if(name.isEmpty()) return;

    model->mkdir(index,name);

}

void DirModelSave::on_pushButton_2_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(!index.isValid()) return;
    QString fileName = ui->lineEdit->text();
    if(fileName.isEmpty()) return;

    QString path = model->filePath(index);
    QString absPath = path + "/"+ fileName;

    if(scene->items().count() > 0){
        SerializatorXML *serializator = new SerializatorXML(scene);
        serializator->serializeToXMLFile(absPath);
        model->refresh();
    }else{
        scene->warningMessage("Empty scene\nNothing to save");
        this->close();
    }
}

void DirModelSave::on_pushButton_3_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(!index.isValid()) return;
    if(model->fileInfo(index).isDir()){
        model->rmdir(index);
    }else{
        model->remove(index);
    }
}
