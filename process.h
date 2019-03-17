#ifndef PROCESS_H
#define PROCESS_H


class Process
{
public:
    Process();
    void setBurstTime(int burstTime);
    int getBurstTime();
    void setArrivalTime(int arrivalTime);
    int getArrivalTime();
    void setPriority(int priority);
    int getPriority();
    void setWaitingTime(int waitingTime);
    int getWaitingTime();
private:
    int burstTime;
    int arrivalTime;
    int priority;
    int waitingTime;
};

#endif // PROCESS_H
