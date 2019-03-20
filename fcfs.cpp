#include "fcfs.h"

FCFS::FCFS()
{

}
void FCFS::setParameters(QVector<Process> process){
    processList = process ;
    signal ="";
    processesNames ="";
    totalWaitingTime = 0;
    numberOfProcesses = process.size();
}

void FCFS::getResults(QString *ToDrawSignal, QString *ToDrawProcess, float *averageWaitingTime){
    FCFSfunction();
    *ToDrawSignal = signal;
    *ToDrawProcess = processesNames;
    *averageWaitingTime = (float)totalWaitingTime/(float)numberOfProcesses;
}


void FCFS::FCFSfunction(){
    int mainCounter=0;
    int proNumber = processList.size();
    bool emptyQueue = true;
    while (!processList.isEmpty() || !readyQueue.isEmpty() ) {
        proNumber = processList.size();
        for(int i=0; i<proNumber;i++){
            if(mainCounter == processList.front().getArrivalTime())
            {
                if(mainCounter!=0 && emptyQueue && readyQueue.isEmpty())
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
            if(readyQueue.front().getBurstTime()>1)
            {
                for(int i =1;i<readyQueue.size();i++){
                    readyQueue[i].incrementWaitingTime();
                    totalWaitingTime++;
                }
                signal += "1";
                readyQueue.front().decrementBurstTime();
            }
            else if(readyQueue.front().getBurstTime()==1) {
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
            }
        }
        else {
            signal+="1";
            emptyQueue=true;
        }

        mainCounter++;
    }



}
