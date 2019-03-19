#ifndef RR_H
#define RR_H
#include <QVector>
#include <QQueue>
#include "process.h"

class RR
{
public:
    RR();
    void setParameters(QVector<Process> process,int QuantumTime);
    void getResults(QString *ToDrawSignal, QString *ToDrawProcess, float* averageWaitingTime);
private:
    void RRfunction();
    QVector<Process> processList;
    int quantumTime;
    QQueue<Process> readyQueue;
    QString signal;
    QString processesNames;
    int totalWaitingTime;
    int numberOfProcesses;
};

#endif // RR_H
