#include "process.h"

Process::Process()
{

}


void Process::setBurstTime(int burstTime){
    this->burstTime = burstTime;
}

void Process::setArrivalTime(int arrivalTime){
    this->arrivalTime = arrivalTime;
}

void Process::setPriority(int priority){
    this->priority = priority;
}

void Process::setWaitingTime(int waitingTime){
    this->waitingTime = waitingTime;
}

int Process::getBurstTime(){
    return this->burstTime;
}

int Process::getArrivalTime(){
    return this->arrivalTime;
}

int Process::getPriority(){
    return this->priority;
}

int Process::getWaitingTime(){
    return this->waitingTime;
}

