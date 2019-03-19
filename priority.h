#ifndef PRIORITY_H
#define PRIORITY_H

#include <QVector>
#include "process.h"

class Priority
{
public:
    Priority();
    void setParameters(QVector<Process> process,bool preemptiveOrNot,int QuantumTime);
    void getResults(QString *ToDrawSignal, QString *ToDrawProcess, float* averageWaitingTime);
};

#endif // PRIORITY_H
