#ifndef FCFS_H
#define FCFS_H
#include <QVector>
#include "process.h"

class FCFS
{
public:
    FCFS();
    void setParameters(QVector<Process> process);
    void getResults(QString *ToDrawSignal, QString *ToDrawProcess, float* averageWaitingTime);
};

#endif // FCFS_H
