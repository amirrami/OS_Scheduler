#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    RRQuantimTime = ui->RRQuantumTime->value();
    int burstTime = ui->burstTime->value();
    int arrivalTime = ui->ArrivalTime->value();
    int priority;
    if(ui->ProcessPriority->isEnabled())
        priority = ui->ProcessPriority->value();
    else
        priority = -1;
    Process p;
    p.setBurstTime(burstTime);
    p.setArrivalTime(arrivalTime);
    p.setPriority(priority);
    processList.append(p);
}






void MainWindow::on_Run_clicked()
{
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

void MainWindow::drawGraph(){

    if(SchedulerMode == "FCFS"){
        fcfs.getResults(ToDrawSignal,ToDrawProcess,averageWaitingTime);
    }
    else if (SchedulerMode == "SJF") {
       sjf.getResults(ToDrawSignal,ToDrawProcess,averageWaitingTime);
    }
    else if (SchedulerMode == "RR") {
        rr.getResults(ToDrawSignal,ToDrawProcess,averageWaitingTime);
    }
    else if (SchedulerMode == "Priority") {
        priority.getResults(ToDrawSignal,ToDrawProcess,averageWaitingTime);
    }
    QVector<QString> Signals;
    QVector<QString> SignalsNames;
    Signals.append(*ToDrawSignal);
    SignalsNames.append(*ToDrawProcess);
    GanttChart = new View(0,Signals,SignalsNames,20);
    GanttChart->show();

}

void MainWindow::on_Preemptive_clicked()
{
    PreemptiveORNone = true;
}

void MainWindow::on_NonPreemptive_clicked()
{
    PreemptiveORNone = false;
}
