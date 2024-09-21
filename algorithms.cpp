#include "algorithms.h"

#include <QThread>
#include <QBrush>
#include <QGraphicsRectItem>

#include <iostream>
#include <stdlib.h>
#include <unistd.h>

void WorkerThread::doSort(int sortSelect,std::vector<QGraphicsRectItem *> c,int d){
    col = c;
    delay = d;

    std::cout << "Received a signal to make a sorting: " << sortSelect << std::endl;

    for(int i = 0; i < (int)col.size(); i++)
    {
        std::cout << col[i]->rect().height() << " ";
    }
    std::cout << std::endl;

    switch(sortSelect)
    {
    case 0:
        bubblesort();
        break;
    default:
        break;
    }
    emit done();
}

void WorkerThread::bubblesort()
{
    int iterations = 0;
    int maxIterations = col.size() - 1;

    int delays[] = {700000, 400000, 230000, 130000, 75000, 43000, 25000, 13000, 8000, 5000, 1000};
    int maxDelay = 11;

    while(iterations < maxIterations)
    {
        bool sorted = true; // Assume sorted at the beginning of each pass

        for(int i = 0; i < (int)col.size() - 1 - iterations; i++)
        {
            emit highlight(i, Qt::green);
            emit highlight(i - 1, Qt::cyan);

            emit inc_comparisons();

            if(col[i]->rect().height() > col[i + 1]->rect().height())
            {
                sorted = false; // A swap is performed, so the array is not sorted

                emit swap(i, i + 1);
                emit inc_swaps();

                auto temp_p = col[i];
                col[i] = col[i + 1];
                col[i + 1] = temp_p;

                if(QThread::currentThread()->isInterruptionRequested())
                {
                    QThread::currentThread()->quit();
                    return;
                }

                int currentDelay = (delay >= 1 && delay <= maxDelay) ? delays[delay - 1] : 0;
                usleep(currentDelay);
            }
        }

        if(sorted)
            break; // If no swaps are performed in a pass, the array is sorted

        iterations++;
        emit highlight(col.size() - iterations, Qt::green);
    }

    for(int i = 0; i < (int)col.size() - iterations; i++)
    {
        emit highlight(i, Qt::green);
    }
}


