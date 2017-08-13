#include "graphwindowtsp.h"
#include "ui_graphwindowtsp.h"
#include "graphaplorithms.h"
#include <string>

using namespace std;

graphwindowTSP::graphwindowTSP(MyScene*s ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::graphwindowTSP), scene(s)
{
    ui->setupUi(this);
    algorithms = new GraphAplorithms(scene);
    this->setWindowTitle("Travelling salesman problem");
    cr = "┌─";
    cl = "└─";
    cp = "│ ";
    clicked = false;
}

graphwindowTSP::~graphwindowTSP()
{
    delete ui;
}

void graphwindowTSP::on_pushButton_clicked()
{
    algorithms->tspAlgorithm();
    solutionVector = algorithms->getSolutionVector();
    clicked = true;

    int counter = 1;
    for(auto i : solutionVector){
        createVector(i);
        vectorOfConnections.append(vectorTemp);
        vectorTemp.clear();
        ui->comboBox->addItem("Solution " + QString::number(counter));
        counter++;
    }

    ui->plainTextEdit->appendPlainText("The solutions was generated successfully !");
    QString e;
    ui->plainTextEdit->appendPlainText("Way: " + QString::number(algorithms->getMinSolution()));
    for(auto i : algorithms->getEgdeSet()){
        i->setShortestWayMade(false);
        scene->update();
    }
}

void graphwindowTSP::on_pushButton_2_clicked()
{
    ui->plainTextEdit->clear();
    QHashIterator<QString, Vertex*> j(scene->getHashTable());
    while (j.hasNext()){
        j.next();
        Vertex* v = j.value();
    }
    QHashIterator<QString, Vertex*> i(scene->getHashTable());
    while(i.hasNext()){
        i.next();
        Vertex *v = i.value();
        for(auto j : v->getConnectionVector()){
            j.first->setShortestWayMade(false);
            scene->update();
        }
    }
}

void graphwindowTSP::on_pushButton_3_clicked()
{
    if(clicked){
        ui->plainTextEdit->clear();
        printBT("","",algorithms->getSolutionTree());
    }else{
        QMessageBox::warning(this, "Warning!", "First click the TSP button !", QMessageBox::Ok);
    }
}

void graphwindowTSP::printBT(QString sp, QString sn, Tree *v)
{
    QString s;

    if(v)
    {
        s = sp;
        if(sn == cr) s[s.length() - 2] = ' ';
        printBT(s + cp, cr, v->getRigth());

        s = cutString(sp.length()-2, sp);
        ui->plainTextEdit->appendPlainText( s + sn + v->getVertexIndex());

        s = sp;
        if(sn == cl) s[s.length() - 2] = ' ';
        printBT(s + cp, cl, v->getLeft());
    }
}

QString graphwindowTSP::cutString(int len, QString sp)
{
    QString s = "";
    for(int i = 0; i < len; i++){
        s+=sp[i];
    }
    return s;
}

void graphwindowTSP::createVector(Tree *t)
{
    if(t){
        try{
            for(auto i : t->getEdge()){
                vectorTemp.append(i);
            }
        }catch(exception &e){
            if(t->getRoot())
                createVector(t->getRoot());
        }
    }
    if(t->getRoot())
        createVector(t->getRoot());
}

void graphwindowTSP::showMatrix(Tree *t)
{
    if(t){
        QString text;
        text.append("   |");
        for(int i = 0; i < t->getMatrix()[0].count(); i++){
            text.append(t->patternMatrixForJ[i] + " ");
        }
        ui->plainTextEdit->appendPlainText(text);
        text.clear();
        for(int i = 0; i < t->getMatrix()[0].count(); i++){
            text.append(t->patternMatrixForI[i] + " |");
            for(int c = 0; c < t->getMatrix()[0].count(); c++){
                if(t->getMatrix()[i][c].second){
                    ConnectionWay *w = getConnection(scene->getHashTable()[t->patternMatrixForI[i]], scene->getHashTable()[t->getMatrix()[i][c].second->getVertexID()]);
                    text.append(QString::number(t->getMatrix()[i][c].first) +" ");
                }else if(t->getMatrix()[i][c].first == 999999999){
                    text.append("N ");
                }
            }
            ui->plainTextEdit->appendPlainText(text);
            text.clear();
        }
        ui->plainTextEdit->appendPlainText("\n");
        showMatrix(t->getLeft());
    }
}


void graphwindowTSP::on_comboBox_activated()
{
    QHashIterator<QString, Vertex*> i(scene->getHashTable());
    while(i.hasNext()){
        i.next();
        Vertex *v = i.value();
        for(auto j : v->getConnectionVector()){
            j.first->setShortestWayMade(false);
            scene->update();
        }
    }
    ui->plainTextEdit->clear();

    int selected = 0;
    for(int i = 0; i < vectorOfConnections[0].count(); i++){
        if(ui->comboBox->currentText() == "Solution " + QString::number(selected + 1)){
            break;
        }
        selected++;
    }
    int counter = 1;
    QString e;
    for(auto c : vectorOfConnections[selected]){
        ConnectionWay*i = c;
        e = QString::number(counter) + ") Edge with weigth: " + QString::number(i->getWeight()) + " ( From " + i->getVertexFrom().second->getVertexID() + " To "
                + i->getVertexTo().second->getVertexID() +" )";
        ui->plainTextEdit->appendPlainText(e);
        i->setShortestWayMade(true);
        scene->update();
        counter++;
    }
}

ConnectionWay * graphwindowTSP::getConnection(Vertex *from, Vertex *to)
{
    for(auto i : from->getConnectionVector()){
        if(to->getVertexID() == i.second){
            return i.first;
        }
    }
}

void graphwindowTSP::on_pushButton_4_clicked()
{
    if(scene->getHashTable().count() < 21){
        Vertex *u = nullptr;
        Vertex *v = nullptr;
        algorithms->tspSimpleAlgorithm();
        ui->plainTextEdit->appendPlainText("Way: " + QString::number(algorithms->getMinSolution()));
        while(!algorithms->getStack().empty()){
            v = algorithms->getStack().top();
            QString e = algorithms->getStack().top()->getVertexID();
            ui->plainTextEdit->appendPlainText(e);
            e.clear();
            algorithms->getStack().pop();
            if(!algorithms->getStack().empty()){
                u = algorithms->getStack().top();
                getConnection(v,u)->setShortestWayMade(true);
                scene->update();
            }
        }
        getConnection(algorithms->getSortedMatrix()[0].second, u)->setShortestWayMade(true);
        scene->update();
    }else{
        QMessageBox::warning(this, "Warning!", "Too many vertexes for the factorial complexity!", QMessageBox::Ok);
    }
}
