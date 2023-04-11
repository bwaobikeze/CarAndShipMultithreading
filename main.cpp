#include <iostream>
using namespace std;
#include <string>
#include <pthread.h>
#include <vector>
#include <unistd.h>

struct ship{
    string BoatName;
    int arrivalTime;// the time a ship arrives at the bridge
    int TimeForShipToCross;// the tim it takes for the car to cross the bridge
};

struct car{
    string licensePlateNumber;
    int arrivalTime;// the time elapsed since the arrival of the prevous car or ship
    int TimeForCarToCross;// the tim it takes for the car to cross the bridge
};


int main() {
    cout << "Hello, World!" << endl;
    return 0;
}
