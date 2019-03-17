#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "view.h"
#include "process.h"
#include "fcfs.h"
#include "priority.h"
#include "rr.h"
#include "sjf.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Schedulermode_currentIndexChanged(const QString &arg1);

    void on_AddProcess_clicked();

    void on_Run_clicked();

    void on_Preemptive_clicked();

    void on_NonPreemptive_clicked();

    void drawGraph();

private:
    Ui::MainWindow *ui;
    QString SchedulerMode;
    QVector<Process> processList;
    bool PreemptiveORNone;
    int RRQuantimTime;
    int* averageWaitingTime;
    QString *ToDrawSignal;
    QString *ToDrawProcess;
    View *GanttChart;
    FCFS fcfs;
    Priority priority;
    RR rr;
    SJF sjf;
};

#endif // MAINWINDOW_H
