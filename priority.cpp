#include <deque>
#include <QList>
#include <iterator>
#include "priority.h"

using namespace std;
void Priority::setParameters(QVector<Process> process, bool preemptiveOrNot, int QuantumTime){
    if (preemptiveOrNot) // permpitive
    {
        int counter = 0;
        int flag_counter = 0;
        QQueue <Process>result;
        QVector<Process> temp =process;
        int my_count = 0;
        do {

            flag_counter = 0;

            QVector<Process> mine;  // temp to identify positoin of same process have same arrival time
            number_process = process.size();
            for (int i = 0; i < number_process; i++)
            {
                if (my_count == 0)
                    temp[i].setWaitingTime(0);

                if (temp[i].getArrivalTime() <= counter)
                {
                    mine.push_back(temp[i]);
                    flag_counter++;
                }

            }
            my_count++;

            if (flag_counter != 0) {
                Process res = check_piriority_perimitive(mine);


                // res.setWaitingTime(counter - res.getArrivalTime()); // 3ayzaa ttzaabt
                int temo = res.getBurstTime();
                temo--;
                res.setBurstTime(temo);  // setting remaining time

                temp = rearrange_perimative(res, temp,temo ,counter);
                result.enqueue(res); // continue


            }


            else {   // no procces arrived at this counter
                result_ones += "1/";
                result_names += "no process/";
            }

            counter++;

        } while (!check_remaining(temp));


        // seeting result here

            my_result_perimative(result);  // debugging

            for (int k = 0; k < temp.size(); k++)   //getting waiting time
            {
                waiting_time += temp[k].getWaitingTime();
            }

    }


    else {            // non permitive

        int counter = 0;
        int flag_counter = 0;


        QQueue<Process>result;
        int my_count = 0;
        while (process.size() != result.size()) {


            flag_counter = 0;
            QVector<int>temp;  // temp to identify positoin of same process have same arrival time
            number_process = process.size();
            for (int i = 0; i < number_process; i++)
            {
                if (my_count == 0)
                    process[i].setWaitingTime(0);
                if (process[i].getArrivalTime() <= counter)
                {
                    if (!check_existance(result, process[i]))
                    {

                        temp.insert(temp.begin()+flag_counter, i);
                        //temp[flag_counter]=i;
                        flag_counter++;

                    }

                }

            }
            my_count++;

            if (flag_counter != 0) {
                Process res = check_piriority(temp, process);
                //Process res =process[0];  // to check errors
                 res.setWaitingTime(counter - res.getArrivalTime()); // set waiting time

                result.enqueue(res); // continue
                counter += res.getBurstTime();

            }


            else {   // no procces arrived at this counter
                result_ones += "1/";
                counter++;
                result_names += "no process/";
            }

        }

        // seeting result here


        while (!result.empty())
        {
            Process temp = result.front();
            result.pop_front();
            my_result_nonperimative(temp);

        }


        result_ones.remove(result_ones.size() - 1, 1);  //happen fe non perimative bs
        result_names.remove(result_names.size() - 1, 1); //happen fe non perimative bs

    }




}

void Priority::getResults(QString *ToDrawSignal, QString *ToDrawProcess, float* averageWaitingTime) {


    *averageWaitingTime = (waiting_time / number_process);
    *ToDrawSignal = result_ones;
    *ToDrawProcess = result_names;


}







bool Priority::check_existance(QQueue<Process> a, Process b) {
    if (a.empty())
        return false;
    QQueue<Process> temp = a;

    while (! temp.empty())
    {
        if (b.getName() == temp.front().getName ())
            return true;
        temp.dequeue();
    }



    return false;
}



Process Priority::check_piriority(QVector<int> temp, QVector<Process> my_process) {
    int min = my_process[temp[0]].getPriority(); // initialise min piriority

    Process result = my_process[temp[0]]; // initialise min process piriority

    //if (temp.size() == 1)
        //return result;
    for (int i = 0; i < temp.size(); i++)
    {
        if (temp[i] > my_process.size() - 1)
            break;
        if (my_process[temp[i]].getPriority() < min)
        {
            min = my_process[temp[i]].getPriority();
            result = my_process[temp[i]];
        }

        else if (i != 0 && my_process[temp[i]].getPriority() == min ) {
            if (my_process[temp[i]].getArrivalTime() < result.getArrivalTime())
            {
                min = my_process[temp[i]].getPriority();
                result = my_process[temp[i]];
            }
        }

    }

    return result;

}

void Priority::my_result_nonperimative(Process a) {

    for (int i = 0; i < a.getBurstTime(); i++)
    {
        result_ones += "1";

    }
    result_names += (a.getName() + "/");
    waiting_time += a.getWaitingTime();
    result_ones += "/";

}




bool Priority::check_remaining(QVector<Process> a) {
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i].getBurstTime() > 0)
            return false;

    }

    return true;
}

void Priority::my_result_perimative(QQueue<Process> a) {
    //queue <Process> tempo = a;
    QString result = a.front().getName();
    QString name;
    while (!a.empty())
    {
        QString temp = a.front().getName();

        if (result == temp)
        {
            result_ones += "1";
            name = a.front().getName();
        }
        else {
            result = a.front().getName();
            result_ones += "/1";
            result_names += name + "/";
            name = a.front().getName();

        }


         a.dequeue();


    }

    result_names += name;
}


Process Priority::check_piriority_perimitive(QVector<Process> a) {
    int min =100000; // initialise min piriority

    Process result =a[0] ; // initialise min process piriority


    for (int i = 0; i < a.size(); i++)
    {
        int flag = 0;
        int c = a[i].getBurstTime();

        if (c == 0)
            flag++;

        if (flag ==0 && a[i].getPriority() < min  )
        {
            min = a[i].getPriority();
            result = a[i];
        }

        else if ( flag==0 && a[i].getPriority() == min ) {
            if (a[i].getArrivalTime() < result.getArrivalTime())
            {
                min = a[i].getPriority();
                result = a[i];
            }
        }

    }

        return result;
}



QVector<Process> Priority::rearrange_perimative( Process a , QVector<Process>temp ,int c ,int counter)
{
    QString resoo = a.getName();
    for (int i = 0; i < temp.size(); i++)
    {
        if (resoo == temp[i].getName())
        {
            temp[i].setBurstTime(c) ;

        }
        else if (temp[i].getArrivalTime() <=counter && temp[i].getBurstTime() > 0)
            temp[i].incrementWaitingTime();

    }
    return temp;

}
