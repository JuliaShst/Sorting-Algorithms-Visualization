#include "sort.h"
#include "ui_sort.h"
#include "algorithms.h"

#include <QBrush>
#include <QGraphicsRectItem>
#include <iostream>
#include <stdlib.h>
#include <QDebug>



Sort::Sort(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Sort)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    vecCount = 50;
    ui->colCount->setRange(5,200);
    ui->colCount->setValue(vecCount);

    delay = 5;
    ui->delay->setRange(0,11);
    ui->delay->setValue(delay);

    ui->SortBut->setEnabled(true);
    ui->StopBut->setEnabled(false);

    sortAlgSelect = 0;
    swaps = 0;
    comparisons = 0;

    shuffle();

    WorkerThread *worker = new WorkerThread;
    worker->moveToThread(&thread);

    connect(ui->MixBut, &QPushButton::clicked, this, &Sort::btnMix);
    connect(ui->SortBut, &QPushButton::clicked, this, &Sort::btnSort);
    connect(ui->StopBut, &QPushButton::clicked, this, &Sort::btnStop);
    connect(ui->delay, &QSpinBox::valueChanged, this, &Sort::spinDelay);
    connect(ui->colCount, &QSpinBox::valueChanged, this, &Sort::spinColCount);
    connect(ui->QuitBut, &QPushButton::clicked, this, &Sort::btnQuit);



    connect(this, &Sort::doSortingAlgorithm, worker, &WorkerThread::doSort);
    connect(worker,&WorkerThread::swap,this,&Sort::switchItems);
    connect(worker,&WorkerThread::done,this,&Sort::restoreButtons);
    connect(worker,&WorkerThread::replace,this,&Sort::replaceItems);
    connect(worker,&WorkerThread::highlight,this,&Sort::highlight);
    connect(worker,&WorkerThread::unhighlight,this,&Sort::unhighlight);
    connect(worker,&WorkerThread::inc_comparisons,this,&Sort::inc_comparisons);
    connect(worker,&WorkerThread::inc_swaps,this,&Sort::inc_swaps);
    thread.start();

}


void Sort::shuffle()
{
    col.clear();
    scene->clear();
    QBrush brush(Qt::cyan);
    QPen pen(Qt::black);

    auto colwidth = ui->graphicsView->size().width() / vecCount;
    int max = ui->graphicsView->size().height() - 5;
    int viewHeight = ui->graphicsView->size().height();

    for(int i = 0; i < vecCount; i++)
    {
        int height = std::rand() % max + 5;
        auto r = new QGraphicsRectItem(i*colwidth,viewHeight - height,colwidth,height);
        r->setBrush(brush);
        r->setPen(pen);
        col.push_back(r);
        scene->addItem(r);
    }

}

void Sort::btnMix(){

    swaps = 0;
    comparisons = 0;
    QString s = "Swaps: " + QString::number(swaps);
    QString c = "Comparisons: " + QString::number(comparisons);
    ui->Swaps->setText(s);
    ui->Comparisons->setText(c);

    shuffle();
    ui->SortBut->setEnabled(true);
}

void Sort::btnSort()
{
    ui->SortBut->setEnabled(false);
    ui->MixBut->setEnabled(false);
    ui->StopBut->setEnabled(true);

    swaps = 0;
    comparisons = 0;
    QString s = "Swaps: " + QString::number(swaps);
    QString c = "Comparisons: " + QString::number(comparisons);
    ui->Swaps->setText(s);
    ui->Comparisons->setText(c);

    if(thread.isFinished()){
        thread.start();
    }

    emit doSortingAlgorithm(sortAlgSelect,col,delay);
}

void Sort::restoreButtons(){
    ui->SortBut->setEnabled(true);
    ui->MixBut->setEnabled(true);
    ui->StopBut->setEnabled(false);
}


void Sort::switchItems(int i,int j){
    auto iRect = col[i]->rect();
    auto minRect = col[j]->rect();
    auto temp = iRect.left();

    iRect.moveLeft(minRect.left());
    minRect.moveLeft(temp);

    col[i]->setRect(iRect);
    col[j]->setRect(minRect);

    auto temp_p = col[i];
    col[i] = col[j];
    col[j] = temp_p;
}

void Sort::boxChangeSortAlg(int index)
{
    sortAlgSelect = index;
}

void Sort::replaceItems(int i,int j){
    auto jRect = col[j]->rect();
    auto iRect = col[i]->rect();

    jRect.moveLeft(iRect.left());
    col[j]->setRect(jRect);

    col[i] = col[j];
}

void Sort::btnStop()
{
    ui->StopBut->setEnabled(false);

        thread.quit(); //Issue thread.quit because if it is alive but not executing a sort function, it will never check request interruption
        if(thread.isRunning()){
            thread.requestInterruption();
            thread.wait();
        }
        restoreButtons();
}

void Sort::spinColCount(int arg1)
{
    vecCount = arg1;
}

void Sort::spinDelay(int arg1)
{
    delay = arg1;
}

void Sort::highlight(int index,Qt::GlobalColor c){
    if(index < 0 || index >= (int)col.size())return;
    QBrush b(c);
    col[index]->setBrush(b);
}

void Sort::unhighlight(int index){
    if(index < 0 || index >= (int)col.size())return;
    QBrush b(Qt::cyan);
    col[index]->setBrush(b);
}

void Sort::inc_comparisons(){
    comparisons++;
    QString c = "Comparisons: " + QString::number(comparisons);
    ui->Comparisons->setText(c);
}

void Sort::inc_swaps(){
    swaps++;
    QString s = "Swaps: " + QString::number(swaps);
    ui->Swaps->setText(s);
}

void Sort::btnQuit()
{
    QApplication::quit();
}


Sort::~Sort()
{
    delete ui;
}


