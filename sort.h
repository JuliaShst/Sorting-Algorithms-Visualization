#ifndef SORT_H
#define SORT_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QtCore>


QT_BEGIN_NAMESPACE
namespace Ui { class Sort; }
QT_END_NAMESPACE

class Sort : public QMainWindow
{
    Q_OBJECT

public:
    Sort(QWidget *parent = nullptr);
    ~Sort();
    void shuffle();

private:
    Ui::Sort *ui;
    QGraphicsScene *scene;
    QThread thread;
    std::vector<QGraphicsRectItem *> col;

    int vecCount;
    int comparisons;
    int swaps;
    int sortAlgSelect;
    double delay;

private slots:
    void switchItems(int,int);
    void restoreButtons(void);
    void highlight(int index,Qt::GlobalColor);
    void unhighlight(int index);
    void inc_swaps();
    void inc_comparisons();
    void replaceItems(int, int);

    void btnMix();
    void btnSort();
    void boxChangeSortAlg(int index);
    void btnStop();
    void spinColCount(int arg1);
    void spinDelay(int arg1);
    void btnQuit();

signals:
    void doSortingAlgorithm(int, std::vector<QGraphicsRectItem *>, int);

};
#endif // SORT_H
