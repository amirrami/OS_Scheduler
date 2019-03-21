#include "sjf.h"

SJF::SJF()
{

}

void SJF::setParameters(QVector<Process> process, bool preemptiveOrNot){    
    processList = process ;
    signal ="";
    processesNames ="";
    totalWaitingTime = 0;
    preemative = preemptiveOrNot;
    numberOfProcesses = process.size();
}

void SJF::getResults(QString *ToDrawSignal, QString *ToDrawProcess, float *averageWaitingTime){
    if(preemative)
        SJF_Preemative();
    else {
        SJF_NonPreemative();
    }
    *ToDrawSignal = signal;
    *ToDrawProcess = processesNames;
    *averageWaitingTime = (float)totalWaitingTime/(float)numberOfProcesses;
}
void SJF:: SJF_NonPreemative(){

    //to sort processList accordnig to burst time
    for(int i=0 ; i<processList.size()-1; i++){
        for(int j=i+1 ; j<processList.size() ; j++){
            if(processList[i].getArrivalTime() == processList[j].getArrivalTime()){
                if(processList[i].getBurstTime()>processList[j].getBurstTime()){
                       std::swap(processList[i], processList[j]);
                }
            }
        }
    }

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
                /////////////////////////////////////////////

                /////////////////////////////////////////////
                for(int i=0; i<readyQueue.size()-1 ; i++){
                    for(int j=i+1 ; j<readyQueue.size();j++){
                        if(readyQueue[i].getBurstTime() > readyQueue[j].getBurstTime())
                            std::swap(readyQueue[i],readyQueue[j]);
                    }
                }

                ////////////////////////////////////////////
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
void SJF:: SJF_Preemative(){

    //to sort processList accordnig to burst time
    for(int i=0 ; i<processList.size()-1; i++){
        for(int j=i+1 ; j<processList.size() ; j++){
            if(processList[i].getArrivalTime() == processList[j].getArrivalTime()){
                if(processList[i].getBurstTime()>processList[j].getBurstTime()){
                       std::swap(processList[i], processList[j]);
                }
            }
        }
    }

    int mainCounter=0;
    int proNumber = processList.size();
    bool emptyQueue = true;
    bool swap = false;
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
                /////////////////////////////////////////////
                Process pro = readyQueue.front();
                for(int i=0; i<readyQueue.size()-1 ; i++){
                    for(int j=i+1 ; j<readyQueue.size();j++){
                        if(readyQueue[i].getBurstTime() > readyQueue[j].getBurstTime()){
                            std::swap(readyQueue[i],readyQueue[j]);
                            swap = true ;
                        }
                    }
                }
                if(swap){
                    if(signal[signal.size()-1] !='/'){
                        signal +="/";
                        processesNames+=pro.getName()+"/";
                    }
                    swap = false;
                }

                ////////////////////////////////////////////

                for(int i =1;i<readyQueue.size();i++){
                    readyQueue[i].incrementWaitingTime();
                    totalWaitingTime++;
                }
                signal += "1";

                readyQueue.front().decrementBurstTime();
                /////////////////////////////////////////////


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
            else if(readyQueue.front().getBurstTime()==0) {
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
