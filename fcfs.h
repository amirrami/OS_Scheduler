#ifndef FCFS_H
#define FCFS_H
#include <QVector>
#include <QQueue>
#include "process.h"


class FCFS
{
public:
    FCFS();
    void setParameters(QVector<Process> process);
    void getResults(QString *ToDrawSignal, QString *ToDrawProcess, float* averageWaitingTime);

private:
    void FCFSfunction();
    QVector<Process> processList;
    QQueue<Process> readyQueue;
    QString signal;
    QString processesNames;
    int totalWaitingTime;
    int numberOfProcesses;
};

#endif // FCFS_H
