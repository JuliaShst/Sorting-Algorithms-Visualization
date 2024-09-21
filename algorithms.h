#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

class WorkerThread : public QObject
{
    Q_OBJECT

public slots:
    void doSort(int,std::vector<QGraphicsRectItem *>,int d);

signals:
    void swap(int,int);
    void done();
    void replace(int,int);
    void highlight(int index,Qt::GlobalColor);
    void unhighlight(int index);
    void inc_comparisons();
    void inc_swaps();
private:
    std::vector<QGraphicsRectItem *> col;
    int delay;
    void bubblesort();
};

#endif // ALGORITHMS_H
