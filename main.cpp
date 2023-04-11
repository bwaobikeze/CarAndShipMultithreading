#include <iostream>
using namespace std;
#include <string>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <sstream>

struct bridge{
    int liftingBridge;// the time elapsed since the arrival of the prevous car or ship
    int loweringBridge;// the tim it takes for the car to cross the bridge
};
struct vehicle{
    string vehicletype;
    string licensePlateNumber;
    int arrivalTime;// the time elapsed since the arrival of the prevous car or ship
    int TimeForCarToCross;// the tim it takes for the car to cross the bridge
};

void* threadFunction(void* arg) {
    // cast the argument to a pointer to the ThreadData struct
    vehicle* threadData = static_cast<vehicle*>(arg);

    // print out the values in the struct
    cout << "vehicle type: " << threadData-> vehicletype << endl;
    cout << "license Plate Number: " << threadData->licensePlateNumber << endl;
    cout << "arrival Time: " << threadData-> arrivalTime << endl;
    cout << "Time For Cross: " << threadData->TimeForCarToCross << endl;
    cout<<"=====================================";
    return nullptr;
}
int main() {
    int numThreads=4;
    struct vehicle transportation[numThreads];
    string FileReadin;
    //cout << "Hello, World!" << endl;
    while(getline(cin,FileReadin)){
        cout<<FileReadin<<endl;
        stringstream  ss(FileReadin);
        string word;
        while(ss>>word){

        }
    }
    return 0;
}
