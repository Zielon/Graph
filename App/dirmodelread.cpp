#include "dirmodelread.h"
#include "ui_dirmodelread.h"
#include "serializatorxml.h"

DirModelRead::DirModelRead(MyScene*s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DirModelRead), scene(s)
{
    ui->setupUi(this);
    this->setWindowTitle("Read graph");
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

DirModelRead::~DirModelRead()
{
    delete ui;
}

// Read dialog with the retriveFromXmlFile method from the serialization object
//
void DirModelRead::on_pushButton_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(!index.isValid()) return;
    QString path = model->filePath(index);
    SerializatorXML *serializator = new SerializatorXML(scene);
    serializator->retriveFromXMLFile(path);
    scene->update();
    this->close();
}
