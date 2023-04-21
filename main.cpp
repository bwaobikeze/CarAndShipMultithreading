#include <iostream>
using namespace std;
#include <string>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <sstream>


struct vehicle{
    string vehicletype;
    string licensePlateNumber;
    int arrivalTime;// the time elapsed since the arrival of the prevous car or ship
    int TimeForCarToCross;// the tim it takes for the car to cross the bridge
};
int bridgeTimeToRaiseDrawbridge;
int bridgeTimeToLowerDrawbridge;



void* shipRoutine(void* arg){
    vehicle threadData = *(vehicle*)arg;
    string shipName=threadData.licensePlateNumber;
    cout<<"Ship "<<shipName<< " has entered the ship Routine"<<endl;
//    cout << "license Plate Number: " << threadData->licensePlateNumber << endl;
//    cout << "arrival Time: " << threadData-> arrivalTime << endl;
//    cout << "Time For Cross: " << threadData->TimeForCarToCross << endl;
//    cout<<"====================================="<<endl;

    return nullptr;
}
void* carRoutine(void* arg){
    vehicle threadData = *(vehicle*)arg;
    string carName=threadData.licensePlateNumber;
    cout<<"Car "<<carName<< " has entered the Car Routine"<<endl;
//    cout << "vehicle type: " << threadData-> vehicletype << endl;
//    cout << "license Plate Number: " << threadData->licensePlateNumber << endl;
//    cout << "arrival Time: " << threadData-> arrivalTime << endl;
//    cout << "Time For Cross: " << threadData->TimeForCarToCross << endl;
//    cout<<"====================================="<<endl;
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
    // print out the values in the struct
//    cout << "vehicle type: " << threadData-> vehicletype << endl;
//    cout << "license Plate Number: " << threadData->licensePlateNumber << endl;
//    cout << "arrival Time: " << threadData-> arrivalTime << endl;
//    cout << "Time For Cross: " << threadData->TimeForCarToCross << endl;
//    cout<<"=====================================";
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
    //cout << "Hello, World!" << endl;
    while(getline(cin,FileReadin)){
        //cout<<FileReadin<<endl;
        stringstream  ss(FileReadin);
        string type, name;
        int x,y;
        ss>>type>>name>>x>>y;
        if(type== "Bridge"){
            bridgeTimeToRaiseDrawbridge=x;
            bridgeTimeToLowerDrawbridge=y;
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
    vehicle temp;
    for(int i=0; i<transportation.size();i++){
        temp=transportation[i];
        if(pthread_create(&pid[i],NULL,&threadFunction,&temp) !=0){
            perror("Failed to create thread");
            return 1;
        }
    }
    for(int i=0; i<transportation.size();i++){
        if(pthread_join(pid[i],NULL)!=0){
            return 2;
        }
    }
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
//    }
    return 0;
}
