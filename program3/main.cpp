/***************************************************************************
* File: angFour.cpp
* Author: Toby
* Modification History:
* 	Toby - 8 November 2020
* Procedures:
* 	   findWaitingTimeFCFS - procedure to calculate the waiting times for the First-Come-First-Serve algorithm
*      findTurnAroundTime - procedure to calculate the turn around times
*      findAvgTimeFCFS - procedure to calculate the average time for the First-Come-First-Serve algorithm
*      findWaitingTimeRR - procedure to calculate the waiting times for the Round Robin algorithm
*      findAvgTimeRR - procedure to calculate the average time for the Round Robin algorithm
*      findAvgTimeHRRN - procedure to calculate the average time for the Highest Response Ratio Next algorithm
*
*      main - Creates the data set of burst/service times, print out the average waiting and turn around times
***************************************************************************/
#include <iostream>
#include <random>

using namespace std;

//process struct for ease of use in the Highest Response Ration Next algorithm
struct process {
    int at, bt, wt, tt, rt, ct;// arrival time, burst time, wait time, and turn around time
    int completed;
    float ntt;//normalized turn around times
    int total_time =0;
}Q1[1000],Q2[1000],Q3[1000];
//output structure to save average wait times and turn around times
struct output{
    int FCFSwait, FCFSturn, RRwait, RRturn, Hwait, Hturn, Fwait, Fturn;
};

/***************************************************************************
* void findWaitingTimeFCFS()
* Author: Toby
* Date: 8 November 2020
* Modification History:
* 	Toby - 8 November 2020
* Description: calculates the waiting times for the First-Come-First-Serve algorithm
* Parameters:
*   length  I/P  int        total number of processes
*   burst   I/P  int[]      array of service/burst times
*   wait    I/P  int[] 	    array of waiting times
**************************************************************************/
void findWaitingTimeFCFS(int length, int burst[], int wait[]){
    wait[0] = 0;//first wait is zero because we do not have to wait to start this process
    for (int i = 1; i < length; i++)wait[i] = burst[i-1] + wait[i-1];//calculate the waiting time
}
/***************************************************************************
* void findTurnAroundTimeFCFS()
* Author: Toby
* Date: 8 November 2020
* Modification History:
* 	Toby - 8 November 2020
* Description: calculates the turn around times
* Parameters:
*   length      I/P  int        total number of processes
*   burst       I/P  int[]      array of service/burst times
*   wait        I/P  int[] 	    array of waiting times
*   turnAround  I/P  int[]      array of turn around times
**************************************************************************/
void findTurnAroundTime(int length, int burst[], int wait[], int turnAround[]){
    for (int i = 0; i < length; i++)turnAround[i] = burst[i] + wait[i];//calculate the turn around times
}
/***************************************************************************
* void findAvgTimeFCFS()
* Author: Toby
* Date: 8 November 2020
* Modification History:
* 	Toby - 8 November 2020
* Description: calculates the average times for the First-Come-First-Serve algorithm
* Parameters:
*   length      I/P  int        total number of processes
*   burst       I/P  int[]      array of service/burst times
*   output      I/P  output     output entry
**************************************************************************/
void findAvgTimeFCFS(int length, int burst[], output o) {
    int wait[length], turn[length], total_wait = 0, total_turn = 0;//initialize/declare waiting time array,
    //turn around array, total wait time, and the total turn around time
    findWaitingTimeFCFS(length, burst, wait); //calculate the waiting times
    findTurnAroundTime(length, burst, wait, turn);//calculate the turn around times
    for (int  i=0; i<length; i++){
        total_wait = total_wait + wait[i];//add to the total waiting time
        total_turn = total_turn + turn[i];//add to the total turn around time
    }
    //output
    o.FCFSwait =total_wait / length;//calculate and output average waiting time
    o.FCFSturn =total_turn / length;//calculate and output average turn
}
/***************************************************************************
* void findWaitingTimeRR()
* Author: Toby
* Date: 8 November 2020
* Modification History:
* 	Toby - 8 November 2020
* Description: calculates the waiting times for the Round Robin algorithm
* Parameters:
*   length  I/P  int        total number of processes
*   burst   I/P  int[]      array of service/burst times
*   wait    I/P  int[] 	    array of waiting times
*   quantum I/P  int        size of each time slot in the round robin
**************************************************************************/
void findWaitingTimeRR(int length, int burst[], int wait[], int quantum) {
    int remaining[length];//save the remaining burst times
    for (int i = 0 ; i < length ; i++)remaining[i] = burst[i];//set the values in remaining to the burst/service times
    int time = 0; // Current time
    while (true){
        bool done = true;//value to break out of the loop
        for (int i = 0 ; i < length; i++){
            if (remaining[i] > 0){//if there are burst/service times left
                done = false; // There is a pending process
                if (remaining[i] > quantum){//if the service time is greater than quantum then add 1 to time and remove
                    //1 quantum from remaining
                    time += quantum;
                    remaining[i] -= quantum;
                }else{
                    time = time + remaining[i];//add remaining to current time
                    wait[i] = time - burst[i];//update waiting times
                    remaining[i] = 0;//set remaining for this iteration to 0
                }
            }
        }
        if (done)break;//if the all the service burst times have been serviced then the waiting time has been calculated
    }
}
/***************************************************************************
* void findAvgTimeRR()
* Author: Toby
* Date: 8 November 2020
* Modification History:
* 	Toby - 8 November 2020
* Description: calculates the average times for the Round Robin algorithm
* Parameters:
*   length      I/P  int        total number of processes
*   burst       I/P  int[]      array of service/burst times
*   quantum     I/P  int        size of each time slot in the round robin
*   output      I/P  output     output entry
**************************************************************************/
void findAvgTimeRR(int length, int burst[], int quantum, output o) {
    int wait[length], turn[length], total_wait = 0, total_turn = 0;//initialize/declare waiting time array,
    //turn around array, total wait time, and the total turn around time
    findWaitingTimeRR(length, burst, wait,quantum); //calculate the waiting times
    findTurnAroundTime(length, burst, wait, turn);//calculate the turn around times
    for (int  i=0; i<length; i++){
        total_wait = total_wait + wait[i];//add to the total waiting time
        total_turn = total_turn + turn[i];//add to the total turn around time
    }
    //output

    o.RRwait =total_wait / length;//calculate and output average waiting time
    o.RRturn =total_turn / length;//calculate and output average turn
    //around time
}
/***************************************************************************
* void findAvgTimeRR()
* Author: Toby
* Date: 8 November 2020
* Modification History:
* 	Toby - 8 November 2020
* Description: calculates the average times for the Highest Response Ratio algorithm
* Parameters:
*   processes   I/P  process[]  array of all the processes
*   length      I/P  int        total number of processes
*   sum         I/P  int        sum of all the burst/service times
*   output      I/P  output     output entry
**************************************************************************/
void findAvgTimeHRRN(process processes[], int n, int sum, output o){
    float avgwait = 0, avgturn = 0;
    for (int t = processes[0].at; t < sum;) {//start at the first arrival time and make sure that we do not go higher than the
        //sum of the service times
        float hrr = -9999;//highest response ratio, set to unreachable number
        float temp;//temporary value for HRR
        int location;//save the location among the processes
        for (int i = 0; i < n; i++) {
            if (processes[i].at <= t && processes[i].completed != 1) {//given that the arrival time is lower and the process has not
                //been completed
                temp = (float)(processes[i].bt + (t - processes[i].at)) / (float)processes[i].bt;//calculate the response ratio
                if (hrr < temp) {//if the current highest response ratio is lower than set the new HRR and move there
                    hrr = temp;
                    location = i;
                }
            }
        }
        t += processes[location].bt;//add the current service/burst time to the total time
        processes[location].wt = t - processes[location].at - processes[location].bt;//update wait time for this instance
        processes[location].tt = t - processes[location].at;//update turn around time for this instance
        avgturn += (float)processes[location].tt;//add total turn around time so that we can calculate the average at the end
        processes[location].ntt = ((float)processes[location].tt / (float)processes[location].bt);//calculated normalized
        //turn around time
        processes[location].completed = 1;//now that the process is completed, we update its status
        avgwait += (float)processes[location].wt;//add total wait time so that we can calculate the average at the end
    }
    //output
    o.Hwait = (int)avgwait / n;//calculate and output average waiting time
    o.Hturn = (int)avgturn / n;//calculate and output average turn around time
}

/***************************************************************************
* int main()
* Author: Toby
* Date: 8 November 2020
* Modification History:
* 	Toby - 8 November 2020
* Description: calculates the average times for the Highest Response Ratio algorithm
* Parameters:
*   main  O/P  string   outputs both the average waiting time and the average turn around time for each of the four
*                       scheduling algorithms: First-Come-First-Serve, Round Robin, Highest Response Ratio Next, and
*                       Feedback
**************************************************************************/
int main(){
    output o[1000];
    for (int m = 0; m < 1000; m++) {


        default_random_engine generator;
        normal_distribution<double> distribution(10.0, 5.0);
        int burst_time[1000];
        for (int n = 0; n < 1000; ++n) {
            burst_time[n] = distribution(generator);
        }
        int quantum = 1;
        int n = sizeof burst_time / sizeof burst_time[0];
        //arrival time of all processes
        int arrivalTime[1000];
        int timeStep = 0;
        for (int i = 0; i < 1000; i++) {
            arrivalTime[i] = timeStep;
            timeStep += 2;
        }
        //process array for HRRN
        process p[n];
        int burstSum = 0;
        for (int i = 0; i < n; i++) {
            p[i].at = arrivalTime[i];
            p[i].bt = burst_time[i];
            p[i].completed = 0;
            burstSum += burst_time[i];
            p[i].wt = 0;
            p[i].tt = 0;
        }
        findAvgTimeFCFS(n, burst_time,o[m]);
        findAvgTimeRR(n, burst_time, quantum,o[m]);
        findAvgTimeHRRN(p, n, burstSum, o[m]);
        for (int i = 0; i < n; i++) {
            p[i].at = arrivalTime[i];
            p[i].bt = burst_time[i];
            p[i].completed = 0;
            burstSum += burst_time[i];
            p[i].wt = 0;
            p[i].tt = 0;
        }
        //feedback
        int i, k = 0, r = 0, time = 0;
        int averageWait, averageTAT = 0;
        for (i = 0; i < n; i++) {
            Q1[i].bt = burst_time[i];
            Q1[i].at = arrivalTime[i];
            Q1[i].rt = Q1[i].bt;/*save burst time in remaining time for each process*/
        }
        time = Q1[0].at;
        for (i = 0; i < n; i++) {
            if (Q1[i].rt <= 1) {
                time += Q1[i].rt;/*from arrival time of first process to completion of this process*/
                Q1[i].rt = 0;
                Q1[i].wt = time - Q1[i].at - Q1[i].bt;/*amount of time process has been waiting in the first queue*/
                Q1[i].tt = time - Q1[i].at;/*amount of time to execute the process*/
                averageTAT += Q1[i].tt;
                averageWait += Q1[i].tt;
            } else/*process moves to queue 2 with qt=1*/{
                Q2[k].wt = time;
                time += 1;
                Q1[i].rt -= 1;
                Q2[k].bt = Q1[i].rt;
                Q2[k].rt = Q2[k].bt;
                k = k + 1;
            }
        }
        for (i = 0; i < k; i++) {
            if (Q2[i].rt <= 1) {
                time += Q2[i].rt;/*from arrival time of first process +bt of this process*/
                Q2[i].rt = 0;
                Q2[i].wt = time - 1 - Q2[i].bt;/*amount of time process has been waiting in the ready queue*/
                averageWait += Q2[i].tt;
                Q2[i].tt = time - Q2[i].at;/*amount of time to execute the process*/
                averageTAT += Q2[i].tt;
            } else/*process moves to queue 3 with FCFS*/{
                Q3[r].at = time;
                time += 1;
                Q2[i].rt -= 1;
                Q3[r].bt = Q2[i].rt;
                Q3[r].rt = Q3[r].bt;
                r = r + 1;
            }
        }
        for (i = 0; i < r; i++) {
            if (i == 0)
                Q3[i].ct = Q3[i].bt + time - 1 - 1;
            else
                Q3[i].ct = Q3[i - 1].ct + Q3[i].bt;
        }
        for (i = 0; i < r; i++) {
            Q3[i].tt = Q3[i].ct;
            averageTAT += Q3[i].tt;
            Q3[i].wt = Q3[i].tt - Q3[i].bt;
            averageWait += Q3[i].tt;
        }
        o[m].Fturn = averageTAT/n;
        o[m].Fwait = averageWait/n;
    }
    float FCFSwaitS, FCFSturnS, RRwaitS, RRturnS, HwaitS, HturnS, FwaitS, FturnS=0;
    for (int i = 0; i < 1000; i++) {
        FCFSwaitS +=(float)o[i].FCFSwait/1000;
        FCFSturnS+=(float)o[i].FCFSturn/1000;
        RRwaitS+=(float)o[i].RRwait/1000;
        RRturnS+=(float)o[i].RRturn/1000;
        HwaitS +=(float)o[i].Hwait/1000;
        HturnS+=(float)o[i].Hturn/1000;
        FwaitS +=(float)o[i].Fwait/1000;
        FturnS+=(float)o[i].Fturn/1000;
    }
    cout<<"\nFirst-Come-First_Serve:\n";
    cout<<"Average wait time: "<<FCFSwaitS<<endl;
    cout<<"Average turn around time: "<<FCFSturnS<<endl;
    cout<<"Round Robin:\n";
    cout<<"Average wait time: "<<RRwaitS<<endl;
    cout<<"Average turn around time: "<<RRturnS<<endl;
    cout<<"Highest Response Ratio:\n";
    cout<<"Average wait time: "<<HwaitS<<endl;
    cout<<"Average turn around time: "<<HturnS<<endl;
    cout<<"Feedback:\n";
    cout<<"Average wait time: "<<FwaitS<<endl;
    cout<<"Average turn around time: "<<FturnS<<endl;
    return 0;
}
 