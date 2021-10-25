/***************************************************************************
* File: bank.cpp
* Author: Toby
* Modification History:
* 	Toby - 17 October 2020
* Procedures:
* 	main - main procedure that reads in a file and check for deadlock, outputs safe or unsafe
***************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
/***************************************************************************
* int main()
* Author: Toby
* Date: 17 October 2020
* Modification History:
* 	Toby - 17 October 2020
* Description: reads in a file that details the scenario and uses the banker's algorithm to check for
*              the safety status of the scenario
* Parameters:
*   argv  I/P  char**   The arguments on the command line
*   argc  I/P  int      The number of arguments on the command line
*   main  O/P  int 	    Outputs safe or unsafe
**************************************************************************/
int main() {
    //setup
    bool safe= false;
    //grab the input file
    const string filename = "input.txt";
    ifstream file(filename);
    //Number of resource types, number of process
    int r, p;
    //grab the number of resources per process
    string line;
    getline(file,line);
    r = stoi(line);
    //grab the resource vector
    getline(file, line);
    stringstream ss(line);
    string tmp;
    //set the resource vector into an array
    int resources[r];
    for(int i = 0; i<r; i++){
        getline(ss, tmp, '\t');//grab the line and split it by tabs
        resources[i] = stoi(tmp);//set the resource vector entry
    }
    //grab the number of processes
    getline(file, line);
    p = stoi(line);
    //grab the claim matrix
    int claim[p][r];
    int allo[p][r];
    for(int i = 0; i<p; i++){//loop through to add each line and entry to the claim matrix
        getline(file, line);
        string temp;
        stringstream t(line);
        for(int j = 0; j<r; j++){
            getline(t, tmp, '\t');//grab the line and split it by tabs
            claim[i][j] = stoi(tmp);//set the claim entry
        }
    }
    //grab the allocation matrix
    for(int i = 0; i<p; i++){//loop through to add each line and entry to the allocation matrix
        getline(file, line);
        string temp;
        stringstream t(line);
        for(int j = 0; j<r; j++){
            getline(t, tmp, '\t');//grab the line and split it by tabs
            allo[i][j] = stoi(tmp);//set the allocation entry
        }
    }
    //set the need matrix
    int need[p][r];//need matrix
    for (int i = 0; i < p; i++) {//calculate the need matrix
        for (int j = 0; j < r; j++)
            need[i][j] = claim[i][j] - allo[i][j];//need for that slot is claim minus allocation
    }
    //set the availability array
    int avail[r];//availability array
    for (int i = 0; i<r; i++){
        int sum=0;// reset sum
        for(int j = 0; j<p; j++){
            sum += allo[j][i];//collect allocation data and store in sum
        }
        avail[i] = resources[i] - sum;//availability for that resource type is maximum resources minus the current sum
    }
    //now that we are finished with setup, we can test for a safe sequence
    for(int i =0; i< r;i++){//loop through to check that none of the claims exceed the maximum number of resources
        for (int j = 0; j < p; j++){
            if(claim[j][i] > resources[i]){
                cout<<"error ";//print error and end the program
                return 0;
            }
        }
    }
    int count=0;
    int curP=-1;//current process, used to save a possible process
    int add = 0;
    for(int k = 0; k< p; k++) {//loop for every process
        for (int i = 0; i < p; i++) {//loop through the need matrix to check for a suitable process(correct need) to execute
            count = 0;//new process so we have to reset count and add for this line
            add = 0;
            for (int j = 0; j < r; j++) {
                if (need[i][j] <= avail[j]) {//if the available array entry can satisfy the need matrix entry
                    count++;//count to make sure that each resource type can fit the available amount of resources
                    add += need[i][j];//add to make sure that this process is nonzero or still needs resources
                }
            }
            if (count == r && add != 0) {//a process is found and is possible
                curP = i;//set the current process to the line entry
                break;//now that the process is found, break out of the loop
            }
        }
        if (curP == -1 && add != 0) safe = false;//if it isn't safe, don't update
        else {
            for (int i = 0; i < r; i++) {//loop through and update for next cycle
                avail[i] = avail[i] + allo[curP][i];//calculate new availability
                allo[curP][i] = 0;//update allocation matrix
                need[curP][i] = 0;//update need matrix
            }
        }
    }
    int final =0;//declare and initialize final
    for (int i = 0; i < p; i++) {//calculate final need
        for (int j = 0; j < r; j++) {
            final += need[i][j];//add to make sure that no process still needs resources
        }
    }
    if(final == 0) cout<< "safe";//if the sum of the need matrix is 0, then all the processes have been satisfied and will therefore return safe
    else cout<<"unsafe";//if some processes still need resources, then it is in an unsafe state
    cout<<"\n";
    return 0;
}



