#ifndef RR_H
#define RR_H
#include <QVector>
#include "process.h"

class RR
{
public:
    RR();
    void setParameters(QVector<Process> process,int QuantumTime);
    void getResults(QString *ToDrawSignal, QString *ToDrawProcess, int* averageWaitingTime);

};

#endif // RR_H
