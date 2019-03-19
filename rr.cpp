#include "rr.h"

RR::RR()
{

}

void RR::setParameters(QVector<Process> process, int QuantumTime){
    processList = process ;
    quantumTime = QuantumTime;
    signal ="";
    processesNames ="";
    totalWaitingTime = 0;
    numberOfProcesses = process.size();
}

void RR::getResults(QString *ToDrawSignal, QString *ToDrawProcess, float *averageWaitingTime){
    RRfunction();
    *ToDrawSignal = signal;
    *ToDrawProcess = processesNames;
    *averageWaitingTime = (float)totalWaitingTime/(float)numberOfProcesses;
}

void RR::RRfunction(){
int mainCounter=0;
int remainingQuantum=quantumTime;
int proNumber = processList.size();
bool emptyQueue = true;
while (!processList.isEmpty() || !readyQueue.isEmpty() ) {
    proNumber = processList.size();
    for(int i=0; i<proNumber;i++){
        if(mainCounter == processList.front().getArrivalTime())
        {
            if(mainCounter!=0 && emptyQueue)
            {
                 signal+="/";
                 processesNames+="No Process/";
                 emptyQueue = false;
            }
            readyQueue.push_back(processList.front());
            processList.pop_front();
        }
    }
    if(!readyQueue.isEmpty())
    {
        if(readyQueue.front().getBurstTime()>1 && remainingQuantum>1)
        {
            for(int i =1;i<readyQueue.size();i++){
                readyQueue[i].incrementWaitingTime();
                totalWaitingTime++;
            }
            signal += "1";
            readyQueue.front().decrementBurstTime();
            remainingQuantum--;
        }
        else if(readyQueue.front().getBurstTime()==1 && remainingQuantum >1) {
            for(int i =1;i<readyQueue.size();i++){
                readyQueue[i].incrementWaitingTime();
                totalWaitingTime++;
            }
            signal += "1";
            Process p = readyQueue.front();
            readyQueue.pop_front();
            if(!readyQueue.isEmpty() || !processList.isEmpty())
            {
                signal +="/";
                processesNames+=p.getName()+"/";
            }
            else {
                processesNames += p.getName();
            }
            remainingQuantum--;
        }
        else if(readyQueue.front().getBurstTime()==1 && remainingQuantum==1) {
            for(int i =1;i<readyQueue.size();i++){
                readyQueue[i].incrementWaitingTime();
                totalWaitingTime++;
            }
            signal += "1";
            Process p = readyQueue.front();
            readyQueue.pop_front();
            if(!readyQueue.isEmpty() || !processList.isEmpty())
            {
                signal +="/";
                processesNames+=p.getName()+"/";
            }
            else {
                processesNames += p.getName();
            }
            remainingQuantum=quantumTime;
        }
        else if(remainingQuantum==1) {
            for(int i =1;i<readyQueue.size();i++){
                readyQueue[i].incrementWaitingTime();
                totalWaitingTime++;
            }
            signal += "1";
            readyQueue.front().decrementBurstTime();
            Process p = readyQueue.front();
            readyQueue.pop_front();
            readyQueue.push_back(p);
            remainingQuantum = quantumTime;
            signal +="/";
            processesNames+=p.getName()+"/";
        }
    }
    else {
        signal+="1";
        emptyQueue=true;
    }


    mainCounter++;
}






}
