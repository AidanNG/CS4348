#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>
#define SIM 1000
#define START 50
#define FINAL 150
#define INCREMENT 10
#define SIZE 1+((FINAL-START)/INCREMENT)

using namespace std;

struct head{
    int track =100;
    int sector =0;
};head h;

struct IO{
    int track, sector;
    float seek, rotationalD, transfer;
    bool done;
};IO* requests;

float FIFO(IO* request, int length){
    float sectorsTraversed, seekTime, rotationalDelay, requestTime =0;
    float transferTime = .04;
    h.track = 100;
    h.sector = 0;
    for (int i = 0; i < length; i++) {
        sectorsTraversed = abs(((request[i].track * 360) + request[i].sector) -((h.track * 360) + h.sector));
        seekTime = 2.5 * sectorsTraversed;
        rotationalDelay = 2.5 * sectorsTraversed;
        requestTime += seekTime + rotationalDelay + transferTime;
        h.track = request[i].track;
        h.sector = request[i].sector;
    }
    return requestTime/(float)length;
}
float LIFO(IO* request, int length){
    float sectorsTraversed, seekTime, rotationalDelay, requestTime =0;
    float transferTime = .04;
    h.track = 100;
    h.sector = 0;
    for (int i = length-1; i >= 0; i--) {
        sectorsTraversed = abs(((request[i].track * 360) + request[i].sector) -((h.track * 360) + h.sector));
        seekTime = 2.5 * sectorsTraversed;
        rotationalDelay = 2.5 * sectorsTraversed;
        requestTime += seekTime + rotationalDelay + transferTime;
        h.track = request[i].track;
        h.sector = request[i].sector;
    }
    return requestTime/(float)length;
}
float SSTF(IO* request, int length){
    float sectorsTraversed, seekTime, rotationalDelay, requestTime, temp =0;
    float transferTime = .04;
    int location;
    h.track = 100;
    h.sector = 0;
    float currMin =99999999;
    for (int i = 0; i < length; i++)request[i].done = false;
    for (int i = 0; i < length; i++) {
        currMin =99999999;
        for (int j = 0; j < length; j++) {
            if(!request[j].done){
                temp = abs(((request[j].track * 360) + request[j].sector) - ((h.track * 360) + h.sector));
                if (currMin > temp) {
                    currMin = temp;
                   location =j;
                }
            }
        }
        sectorsTraversed = abs(((request[location].track * 360) + request[location].sector) -((h.track * 360) + h.sector));
        seekTime = 2.5 * sectorsTraversed;
        rotationalDelay = 2.5 * sectorsTraversed;
        //cout<<location<<" ";
        requestTime += seekTime + rotationalDelay + transferTime;
        h.track = request[location].track;
        h.sector = request[location].sector;
        request[location].done = true;
    }
    //cout<<"\n";
    return requestTime/(float)length;
}
bool compare(int a, int b){
    float one = abs(((requests[a].track * 360) + requests[a].sector) - ((h.track * 360) + h.sector));
    float two = abs(((requests[b].track * 360) + requests[b].sector) - ((h.track * 360) + h.sector));
    return one > two;
}
float SCAN(IO* request, int length){
    float sectorsTraversed, seekTime, rotationalDelay, requestTime =0;
    float transferTime = .04;
    h.track = 100;
    h.sector = 0;
    vector<int> left, right;
    for (int i = 0; i < length; i++) {
        sectorsTraversed = ((request[i].track * 360) + request[i].sector) - ((h.track * 360) + h.sector);
        if(sectorsTraversed < 0){
            left.push_back(i);
        }else{
            right.push_back(i);
        }
    }
    int current = 0;
    sort(right.begin(),right.end(), compare);
    sort(left.begin(),left.end(), compare);
    for (int i = 0; i < right.size(); i++) {
        current = right[i];
        sectorsTraversed = abs(((request[current].track * 360) + request[current].sector) - ((h.track * 360) + h.sector));
        seekTime = 2.5 * sectorsTraversed;
        rotationalDelay = 2.5 * sectorsTraversed;
        requestTime += seekTime + rotationalDelay + transferTime;
        h.track = request[current].track;
        h.sector = request[current].sector;
    }
    for (int i = left.size()-1; i >=0; i--) {
        current = left[i];
        sectorsTraversed = abs(((request[current].track * 360) + request[current].sector) - ((h.track * 360) + h.sector));
        seekTime = 2.5 * sectorsTraversed;
        rotationalDelay = 2.5 * sectorsTraversed;
        requestTime += seekTime + rotationalDelay + transferTime;
        h.track = request[current].track;
        h.sector = request[current].sector;
    }
    return requestTime/(float)length;
}

int main() {
    default_random_engine gen;
    uniform_int_distribution<int> trackDis(0,201);
    uniform_int_distribution<int> sectorDis(0, 360);

    float* FIFOarr = new float[SIZE];
    float* LIFOarr = new float[SIZE];
    float* SSTFarr = new float[SIZE];
    float* SCANarr = new float[SIZE];

    for(int count =START; count<= FINAL; count += 10){
        requests = new IO[count];
        for (int i = 0; i < count; i++) {
            requests[i].track = trackDis(gen);
            requests[i].sector = sectorDis(gen);
        }
        for (int i = 0; i < SIM; i++) {
            int current = (count-START)/INCREMENT;
            if (current ==0){
                FIFOarr[current] = FIFO(requests,count);
                LIFOarr[current] = LIFO(requests,count);
                SSTFarr[current] = SSTF(requests,count);
                SCANarr[current] = SCAN(requests,count);
            }else{
                FIFOarr[current] = ((FIFOarr[current]+FIFO(requests,count))/2);
                LIFOarr[current] = ((LIFOarr[current]+LIFO(requests,count))/2);
                SSTFarr[current] = ((SSTFarr[current]+SSTF(requests,count))/2);
                SCANarr[current] = ((SCANarr[current]+SCAN(requests,count))/2);
            }
        }
    }
    cout<<"Algo\tFIFO\t\tLIFO\t\tSSTF\t\tSCAN\n";
    for (int i = 0; i < SIZE; i++) {
        cout<<"\t\t"<< FIFOarr[i]/1000<<"\t\t"<< LIFOarr[i]/1000 <<"\t\t"<<SSTFarr[i]/1000<<"\t\t"<< SCANarr[i]/1000;
        cout<<"\n";
    }

    return 0;
}
