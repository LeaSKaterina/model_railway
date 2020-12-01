//
// Created by HP on 23.11.2020.
//

#include "station.h"

int Station::counter = 0;

Station::Station(string name) {
    Station::name = move(name);
    Station::type = UNKNOWN_TYPE;
    Station::id = Station::counter;
    Station::counter++;
}

void Station::setName(string newName) {
    Station::name = move(newName);
}

string &Station::getName() {
    return Station::name;
}

Station::Station(string name, int numberOfResource) {
    Station::name = move(name);
    Station::type = UNKNOWN_TYPE;
    Station::resources.emplace_back(0, numberOfResource);
    Station::id = Station::counter;
    Station::counter++;
}

void Station::deleteTheStation(Station *station) {
    delete station;
    counter--;
}

int Station::getId() {
    return id;
}

void Station::transit(Train *train) {
    cout<<"Train "<<train->getName()<<" - passed the station "<<Station::getName()<<"."<<endl;
}

void Station::temporaryStop(Train *train, int stopTime) {
    cout<<"The train"<<train->getName()<<" has a temporary stop on the station "<<Station::getName()<<"."<<endl;
    train->setTravelTime(stopTime);
}

int Station::getAmountOfResources() {
    int amountOfResources = 0;
    for (auto resource : resources){
        amountOfResources+=resource.getAmount();
    }
    return amountOfResources;
}

void Station::setTheFirstResource(int type, int amount) {
    resources.emplace_back(Resource(type, amount));
}

void Station::addTheResource(int type, int amount) {
    for (auto resource : resources){
        if (resource.getType() == type){
            resource.restock(amount);
            return;
        }
    }
    resources.emplace_back(Resource(type, amount));
}

void Station::deleteTheResource(int type) {
    for (int i = 0; i< resources.size();i++){
        if (resources[i].getType() == type){
            resources.erase(resources.begin() + i);
        }
    }
    throw 123; //there aren't such resources
}

PassengerStation::PassengerStation(string name, int numberOfPassengers) : Station(move(name)) {
    type = PASSENGER;
    resources.emplace_back(Resource(PASSENGER, numberOfPassengers));
}

void PassengerStation::transit(Train *train) {
    cout<<"Train "<<train->getName()<<" - passed the passenger station "<<Station::getName()<<"."<<endl;
    train->departedFrom();
}

void PassengerStation::temporaryStop(Train *train, int stopTime) {
    cout<<"The train"<<train->getName()<<" has a temporary stop on the passenger station "<<Station::getName()<<"."<<endl;
    train->setTravelTime(stopTime);
}

FreightStation::FreightStation(string name, int numberOfGoods) : Station(move(name)) {
    type = FREIGHT;
    resources.emplace_back(Resource(FREIGHT, numberOfGoods));
}

void FreightStation::transit(Train *train) {
    cout<<"Train "<<train->getName()<<" - passed the freight station "<<Station::getName()<<"."<<endl;
}

void FreightStation::temporaryStop(Train *train, int stopTime) {
    cout<<"The train"<<train->getName()<<" has a temporary stop on the freight station "<<Station::getName()<<"."<<endl;
    train->setTravelTime(stopTime);
}

PassengerAndFreightStation::PassengerAndFreightStation(string name, int numberOfPassengers, int numberOfGoods)
        : Station(move(name)) {
    type = PASSENGER_AND_FREIGHT;
    resources.emplace_back(Resource(PASSENGER, numberOfPassengers));
    resources.emplace_back(Resource(FREIGHT, numberOfGoods));
}

void PassengerAndFreightStation::transit(Train *train) {
    cout<<"Train "<<train->getName()<<" - passed the passenger and freight station "<<Station::getName()<<"."<<endl;
}

void PassengerAndFreightStation::temporaryStop(Train *train, int stopTime) {
    cout<<"The train"<<train->getName()<<" has a temporary stop on the passenger and freight station "<<Station::getName()<<"."<<endl;
    train->setTravelTime(stopTime);
}
