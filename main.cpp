#include <iostream>
using namespace std;
#include <string>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <sstream>
#include <fstream>

enum drawbridgeStatus {NOCARS, CARSCANGO};
drawbridgeStatus DrawbridgeStatus=NOCARS;
pthread_cond_t carsCanGoCond=PTHREAD_COND_INITIALIZER;
int bridgeTimeToRaiseDrawbridge=1;
int bridgeTimeToLowerDrawbridge=1;
int temp1;
int temp2;
int nCars;
int nShips;
static pthread_mutex_t drawBridgeLock=PTHREAD_MUTEX_INITIALIZER;

struct vehicle{
    string vehicletype;
    string licensePlateNumber;
    int arrivalTime;// the time elapsed since the arrival of the prevous car or ship
    int TimeForCarToCross;// the tim it takes for the car to cross the bridge
};


void* shipRoutine(void* arg){
    /***********STEPS FOR SHIP ROUTINE***********************
   * STEP 1:Close the drawbridge for car trafic
   * STEP 2: Wait until they can lower the drawbridge(No other ship & No car on the drawbridge)
   * STEP 3:Sleep for drawbridge raising time
   * STEP 4: Sleep for bridge crossing time
   * STEP 5: Sleep for drawbridge lowering time
   * STEP 6: Reopen drawbridge to car traffic
   * ============IMPLEMENTATION STEPS==============================
   * implement these instructions:
   * STEP 1: Print a message when they are created: "Ship [Name] arrives"
   * STEP 2: Set drawbridgeStatus to NOCARS(Before doing anything else)
   * STEP 3: Print second message: "Drawbridge is closed to car traffic"
   * STEP 4: Request drawbridge mutex( No cars on bridge)
   * STEP 5:Update nShips
   * STEP 6: Print a third messsage: "Drawbidge can be saftely raised
   * STEP 7: Print a fourth message: "Ship [Name] goes"
   * STEP 8: Sleep for drawbridge crossing time
   * STEP 9: Print a fifth message: "Ship [Name] leaves"
   * STEP 10: Sleep for draw bridge lowering time
   * STEP 11: print a sixth message: "Drawbridge is reopended to car traffic"
   * STEP 12: Set drawbridgeStatus to CARSCANGO
   * STEP 13: Signal CARSCANGO condition
   * STEP 14: Release drawbridge mutex (Reopens drawbridge to care traffic)
   * ****************************************************/
    vehicle threadData = *(vehicle*)arg;
    string shipName=threadData.licensePlateNumber;
    cout<<"Ship "<<shipName<< " arrives"<<endl;
    DrawbridgeStatus=NOCARS;
    cout<<"Drawbridge is closed to car traffic"<<endl;
    pthread_mutex_lock(&drawBridgeLock);

    nShips++;

    cout<<"Drawbridge can be saftely raised"<<endl;
    sleep(bridgeTimeToRaiseDrawbridge);
    cout<<"Ship "<< shipName <<" goes"<<endl;
    sleep(threadData.TimeForCarToCross);
    cout<<"Ship "<<shipName << " Leaves"<<endl;
    sleep(bridgeTimeToLowerDrawbridge);

    cout<<"Drawbridge is reopened to car traffic"<< endl;

    DrawbridgeStatus=CARSCANGO;

    pthread_cond_signal(&carsCanGoCond);

    pthread_mutex_unlock(&drawBridgeLock);

    return nullptr;
}
void* carRoutine(void* arg){
    /**************STEPS FOR CAR ROUTINE********************************
     * STEP 1: print message: car i has arrived
     * STEP 2: Request drawbridge mutex
     * STEP 3: Update nCars(From inside a critical section)
     * STEP 4: if drawbridgeStatus == NoCARS( Wait for a carCanGo signal from a ship thread)
     * STEP 5: Print second message: " Car i Goes on Bridge"
     * STEP 6: Sleep for the time it takes to cross the drawbridge
     * STEP 7: Print a third message: "Car i Leaves the drawbridge"
     * STEP 8: drawbridge mutex
 * ********************************************************/
    vehicle threadData = *(vehicle*)arg;
    string carName=threadData.licensePlateNumber;
    cout<<"Car "<<carName<< " has Arrived"<<endl;
    pthread_mutex_lock(&drawBridgeLock);

    nCars++;

    while(DrawbridgeStatus==NOCARS){
        pthread_cond_wait(&carsCanGoCond,&drawBridgeLock);
    }
cout<<"Car "<<carName<<" goes on the drawbridge"<<endl;
sleep(threadData.TimeForCarToCross);

cout<<"Car "<<carName<<" Leaves the drawbridge"<<endl;

    pthread_mutex_unlock(&drawBridgeLock);
    return nullptr;
}

void* threadFunction(void* arg) {
    // cast the argument to a pointer to the ThreadData struct
    vehicle threadData = *(vehicle*)arg;
    string type=threadData.vehicletype;
        if(type=="Ship"){
            shipRoutine(&threadData);
        }else{
            carRoutine(&threadData);
        }
   return nullptr;
}
/***************
Bridge 1 1
Car STOL3N 0 2
Car HIOFCR 1 2
Ship Alicia 1 2
Car 2ZBEACH 2 2
 ***************/
int main() {
    vector<vehicle> transportation;
    string FileReadin;

//    ifstream file("input30.txt");
//
//    if (!file) {
//        cerr << "Failed to open file" << std::endl;
//        return 1;
//    }
    while(getline(cin,FileReadin)){
        //cout<<FileReadin<<endl;
        stringstream  ss(FileReadin);
        string type, name;
        int x,y;
        ss>>type>>name>>x>>y;
        if(type== "Bridge"){
            temp1=x;
            temp2=y;
        }else if(type=="Car"||type=="Ship"){
            vehicle Vehicle;
            Vehicle.vehicletype=type;
            Vehicle.licensePlateNumber=name;
            Vehicle.arrivalTime=x;
            Vehicle.TimeForCarToCross=y;
            transportation.push_back(Vehicle);
        }
    }
    pthread_t pid[transportation.size()];
    for(int i=0; i<transportation.size();i++){
        vehicle* temp= new vehicle(transportation[i]);
        sleep(temp->arrivalTime);
        if(pthread_create(&pid[i],NULL,&threadFunction,temp) !=0){
            perror("Failed to create thread");
            return 1;
        }
    }
    for(int i=0; i<transportation.size();i++){
        if(pthread_join(pid[i],NULL)!=0){
            return 2;
        }
        //cout<<" Thread "<<i<<" has ended execution"<<endl;
    }
    cout<<nCars<<" Cars(s) crossed the bridge."<<endl;
    cout<<nShips<<" Ship(s) crossed the bridge."<<endl;
//    cout<<"==============Bridge======================"<<endl;
//    cout << "Bridge Time To Raise Drawbridge: " << bridgeTimeToRaiseDrawbridge<< endl;
//    cout << "Bridge Time To Lower Drawbridge: " << bridgeTimeToLowerDrawbridge << endl;
//    cout<<"====================================="<<endl;
//    for(int i= 0; i< transportation.size();i++){
//        cout << "vehicle type: " << transportation[i].vehicletype << endl;
//        cout << "license Plate Number: " << transportation[i].licensePlateNumber << endl;
//        cout << "arrival Time: " << transportation[i].arrivalTime << endl;
//        cout << "Time For Cross: " << transportation[i].TimeForCarToCross << endl;
//        cout<<"====================================="<<endl;
    return 0;
}
