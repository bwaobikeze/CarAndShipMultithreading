#include <iostream>
using namespace std;
#include <string>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <sstream>
#include <fstream>

enum drawbridgeStatus {NOCARS, CARSCANGO};
drawbridgeStatus DrawbridgeStatus=CARSCANGO;
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
    vehicle threadData = *(vehicle*)arg;
    string shipName=threadData.licensePlateNumber;
    cout<<"Ship "<<shipName<< " arrives at the drawbridge"<<endl;
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

    DrawbridgeStatus=CARSCANGO;

    pthread_cond_broadcast(&carsCanGoCond);

    pthread_mutex_unlock(&drawBridgeLock);

    cout<<"Drawbridge is reopened to car traffic"<< endl;

    return nullptr;
}
void* carRoutine(void* arg){
    vehicle threadData = *(vehicle*)arg;
    string carName=threadData.licensePlateNumber;
    cout<<"Car "<<carName<< " arrives at the drawbridge"<<endl;
    pthread_mutex_lock(&drawBridgeLock);

    nCars++;

    if(DrawbridgeStatus==NOCARS){
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

    ifstream file("input30.txt");

    if (!file) {
        cerr << "Failed to open file" << std::endl;
        return 1;
    }
    while (getline(file, FileReadin)) {
        //cout<<FileReadin<<endl;
        stringstream ss(FileReadin);
        string type, name;
        int x, y;
        ss >> type >> name >> x >> y;
        if (type == "Bridge") {
            temp1 = x;
            temp2 = y;
        } else if (type == "Car" || type == "Ship") {
            vehicle Vehicle;
            Vehicle.vehicletype = type;
            Vehicle.licensePlateNumber = name;
            Vehicle.arrivalTime = x;
            Vehicle.TimeForCarToCross = y;
            transportation.push_back(Vehicle);
        }
    }
    cout<<"Bridge Rise Time: "<<temp1<<endl;
    cout<<"Bridge lower Time: "<<temp2<<endl;
//    pthread_t pid[transportation.size()];
//    for (int i = 0; i < transportation.size(); i++) {
//        vehicle *temp = new vehicle(transportation[i]);
//        sleep(temp->arrivalTime);
//        if (pthread_create(&pid[i], NULL, &threadFunction, temp) != 0) {
//            perror("Failed to create thread");
//            return 1;
//        }
//    }
//    for (int i = 0; i < transportation.size(); i++) {
//        if (pthread_join(pid[i], NULL) != 0) {
//            return 2;
//        }
//    }
//    cout << nCars << " Cars(s) crossed the bridge." << endl;
//    cout << nShips << " Ship(s) crossed the bridge." << endl;
    return 0;
}


