#pragma once
#include "process.h"

#include<QQueue>
class Priority
{
    QString result_ones = "";
    QString result_names = "";
    int waiting_time = 0;
    int number_process;


public:


    QVector<Process> rearrange_perimative(Process , QVector <Process>, int ,int);
    Process check_piriority(QVector<int>, QVector <Process>my_process);
    void my_result_nonperimative(Process);
    bool check_existance(QQueue <Process>, Process);
    void setParameters(QVector<Process> process, bool preemptiveOrNot, int QuantumTime);
    void getResults(QString *ToDrawSignal, QString *ToDrawProcess, float* averageWaitingTime);
    bool check_remaining(QVector <Process>);
    void my_result_perimative(QQueue <Process>);
    Process check_piriority_perimitive( QVector <Process>);


    // (111/ a)  /  process1 /
};
