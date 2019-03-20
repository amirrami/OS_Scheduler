#include "mainwindow.h"
#include "ui_mainwindow.h"
int const MainWindow::EXIT_CODE_REBOOT = -123456789;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Preemptive->setEnabled(false);
    ui->NonPreemptive->setEnabled(false);
    ui->ProcessPriority->setEnabled(false);
    ui->RRQuantumTime->setEnabled(false);
    SchedulerMode = "FCFS";
    PreemptiveORNone = true;
    processCounter = 0;
    ToDrawSignal = "";
    ToDrawProcess = "";
    averageWaitingTime = 0;
    ToDrawSignalPointer = &ToDrawSignal;
    ToDrawProcessPointer = &ToDrawProcess;
    averageWaitingTimePointer = &averageWaitingTime;


    actionReboot = new QAction( this );
    actionReboot->setText( tr("Restart") );
    actionReboot->setStatusTip( tr("Restarts the application") );
    connect( actionReboot, SIGNAL (triggered()),
                     this, SLOT ()
           );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Schedulermode_currentIndexChanged(const QString &arg1)
{
    SchedulerMode = arg1;
    ui->Preemptive->setEnabled(true);
    ui->NonPreemptive->setEnabled(true);
    ui->ProcessPriority->setEnabled(true);
    ui->RRQuantumTime->setEnabled(true);

    if(arg1 == "FCFS"){
        ui->Preemptive->setEnabled(false);
        ui->NonPreemptive->setEnabled(false);
        ui->ProcessPriority->setEnabled(false);
        ui->RRQuantumTime->setEnabled(false);
    }
    else if (arg1 == "SJF") {
        ui->ProcessPriority->setEnabled(false);
        ui->RRQuantumTime->setEnabled(false);
    }
    else if (arg1 == "RR") {
        ui->Preemptive->setEnabled(false);
        ui->NonPreemptive->setEnabled(false);
        ui->ProcessPriority->setEnabled(false);
    }
    else if (arg1 == "Priority") {

    }
}

void MainWindow::on_AddProcess_clicked()
{
    if(ui->burstTime->value() >0){
        processCounter++;
        int burstTime = ui->burstTime->value();
        int arrivalTime = ui->ArrivalTime->value();
        int priority;
        if(ui->ProcessPriority->isEnabled())
            priority = ui->ProcessPriority->value();
        else
            priority = -1;
        Process p;
        p.setWaitingTime(0);
        p.setBurstTime(burstTime);
        p.setArrivalTime(arrivalTime);
        p.setPriority(priority);
        p.setName("P"+QString::number(processCounter));
        processList.append(p);
        ui->burstTime->setValue(0);
        ui->ArrivalTime->setValue(0);
        ui->ProcessPriority->setValue(0);
    }
    else {
        QMessageBox::warning(this,"BurstTime Error","Process BurstTime must be Greater than 0");

    }

}






void MainWindow::on_Run_clicked()
{
    MainWindow::SortArrivalTime();
    if(ui->RRQuantumTime->isEnabled() && ui->RRQuantumTime->value() >0)
        RRQuantimTime = ui->RRQuantumTime->value();
    else if(ui->RRQuantumTime->isEnabled() && ui->RRQuantumTime->value() <=0) {
        QMessageBox::warning(this,"RR QuantumTime Erorr","You Did not Write RR QuantumTime");
        return;
    }
    if(processList.size()>=1){
        if(SchedulerMode == "FCFS"){
            fcfs.setParameters(processList);
        }
        else if (SchedulerMode == "SJF") {
           sjf.setParameters(processList,PreemptiveORNone);
        }
        else if (SchedulerMode == "RR") {
            rr.setParameters(processList,RRQuantimTime);
        }
        else if (SchedulerMode == "Priority") {
            priority.setParameters(processList,PreemptiveORNone,RRQuantimTime);
        }
        drawGraph();
    }
    else {
        QMessageBox::warning(this,"Process Error","You Did not Add Any Process");
    }


}

void MainWindow::drawGraph(){

    if(SchedulerMode == "FCFS"){
        fcfs.getResults(ToDrawSignalPointer,ToDrawProcessPointer,averageWaitingTimePointer);
    }
    else if (SchedulerMode == "SJF") {
       sjf.getResults(ToDrawSignalPointer,ToDrawProcessPointer,averageWaitingTimePointer);
    }
    else if (SchedulerMode == "RR") {
        rr.getResults(ToDrawSignalPointer,ToDrawProcessPointer,averageWaitingTimePointer);
    }
    else if (SchedulerMode == "Priority") {
        priority.getResults(ToDrawSignalPointer,ToDrawProcessPointer,averageWaitingTimePointer);
    }
    QVector<QString> Signals;
    QVector<QString> SignalsNames;
    Signals.append(ToDrawSignal);
    Signals.append(ToDrawProcess);
    SignalsNames.append("Processor");
    GanttChart = new View(0,Signals,SignalsNames,numberOfCycles());
    GanttChart->show();
    ui->WaintingTime->display((double)averageWaitingTime);
}

void MainWindow::on_Preemptive_clicked()
{
    PreemptiveORNone = true;
}

void MainWindow::on_NonPreemptive_clicked()
{
    PreemptiveORNone = false;
}

void MainWindow::SortArrivalTime(){
    int numberOfProcess = processList.size();
    int min_idx;
    for (int i = 0; i < numberOfProcess-1; i++)
        {
            // Find the minimum element in unsorted array
            min_idx = i;
            for (int j = i+1; j < numberOfProcess; j++)
              if (processList[j].getArrivalTime() < processList[min_idx].getArrivalTime())
                min_idx = j;

            // Swap the found minimum element with the first element
            std::swap(processList[min_idx], processList[i]);
        }
}

int MainWindow::numberOfCycles(){
    int cycles=0;
    for(int i=0;i<ToDrawSignal.length();i++){
        if(ToDrawSignal[i]=='1')
            cycles++;
    }
    return cycles;
}




void MainWindow::on_Restart_clicked()
{
    qApp->exit( MainWindow::EXIT_CODE_REBOOT );
}
