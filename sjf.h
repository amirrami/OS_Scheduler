#ifndef SJF_H
#define SJF_H
#include <QVector>
#include "process.h"


class SJF
{
public:
    SJF();
    void setParameters(QVector<Process> process,bool preemptiveOrNot);
    void getResults(QString *ToDrawSignal, QString *ToDrawProcess, float* averageWaitingTime);
};

#endif // SJF_H
