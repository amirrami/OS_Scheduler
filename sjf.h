#ifndef SJF_H
#define SJF_H
#include <QVector>
#include <QQueue>
#include "process.h"


class SJF
{
public:
    SJF();
    void setParameters(QVector<Process> process,bool preemptiveOrNot);
    void getResults(QString *ToDrawSignal, QString *ToDrawProcess, float* averageWaitingTime);
    void SJF_NonPreemative();
    void SJF_Preemative();
private :
    QVector<Process> processList;
    QQueue<Process> readyQueue;
    QString signal;
    QString processesNames;
    QQueue<Process> remainingTime;
    int totalWaitingTime;
    int numberOfProcesses;

};

#endif // SJF_H
