//
// Created by HP on 23.11.2020.
//

#include "station.h"

int Station::counter = 0;

Station::Station(string name) {
    Station::name = move(name);
    Station::type = UNKNOWN_TYPE;
    Station::id = Station::counter;
    Station::setTheFirstResource(Resource(UNKNOWN_TYPE,100));
    Station::counter++;
}

void Station::setName(string newName) {
    Station::name = move(newName);
}

string &Station::getName() {
    return Station::name;
}

Station::Station(string name, Resource resource) {
    Station::name = move(name);
    Station::type = UNKNOWN_TYPE;
    Station::resources.emplace_back(resource);
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
    cout<<"Train "<<train->getName()<<" - passed the station (transit)"<<Station::getName()<<"."<<endl;
    train->departedFrom();
}

void Station::temporaryStop(Train *train, int stopTime) {
    cout<<"The train"<<train->getName()<<" has a temporary stop on the station "<<Station::getName()<<"."<<endl;
    train->setTravelTime(stopTime);
}

int Station::getAmountOfResource(int typeOfResource) {
    checkTypeOfResource(typeOfResource);
    for (auto resource : resources){
        if (resource.getType() == typeOfResource){
            return resource.getAmount();
        }
    }
    throw StationException("There isn't such resource. Check the type of resource. ");
}

void Station::setTheFirstResource(Resource theFirstRecourse) {
    resources.emplace_back(theFirstRecourse);
}

void Station::addTheResource(Resource newRecourse) {
    for (auto resource : resources){
        if (resource.getType() == newRecourse.getType()){
            resource.restock(newRecourse.getAmount());
            return;
        }
    }
    resources.emplace_back(newRecourse);
}

void Station::deleteTheResource(int typeOfDeletedResource) {
    checkTypeOfResource(typeOfDeletedResource);
    for (int i = 0; i< resources.size();i++){
        if (resources[i].getType() == typeOfDeletedResource){
            resources.erase(resources.begin() + i);
            return;
        }
    }
    throw StationException("There isn't such resource. Check the type of deleted resource .");
}

/*void Station::reduceTheNumberOfResource(int typeOfResource, int numberOfResource) {
    checkTypeOfResource(typeOfResource);
    try {
        for (auto resource : resources) {
            if (resource.getType() == typeOfResource) {
                resource.reduce(numberOfResource);
                return;
            }
        }
    }
    catch (ResourceException exception){

    }
    throw StationException("There isn't such resource. Check the type of resource .");
}*/

void Station::checkTypeOfResource(int type) {
    if (type < 0 || type > 3){
        throw ResourceException("Incorrect input: invalid type value. "); //------------------------------------------------try catch(?)
    }
}

void Station::restockTheNumberOfResource(int typeOfResource, int numberOfResource) {
    checkTypeOfResource(typeOfResource);
    for (auto resource : resources){
        if (resource.getType() == typeOfResource){
            resource.restock(numberOfResource);
            return;
        }
    }
    throw StationException("There isn't such resource. Check the type of resource .");
}

void Station::loading(Train *train) {
    cout<<"It have to be loading of the train \""<< train->getName()<<"\", but it's impossible with the unknown-type station."<<endl;
}

void Station::unloading(Train *train) {
    cout<<"It have to be loading of the train \""<< train->getName()<<"\", but it's impossible with the unknown-type station."<<endl;
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

void PassengerStation::loading(Train *train) {
    bool flagOfException = false;
    for (auto resource : resources){
        if (resource.getType() == PASSENGER){
            try {
                train->loading(resource);
            }
            catch(TrainException& exception){
                resource.setAmount(exception.getAmountOfExceptionalResource());
                flagOfException = true;
            }
            if (!flagOfException){
                resource.setAmount(0);
            }
            cout<<"The train \""<<train->getName()<<"\" is loaded. "<<resource.getAmount()<<" passengers are stayed on the station \""<<getName()<<"\"."<<endl;
        }
    }
}

void PassengerStation::unloading(Train *train) {
    bool flagOfException = false;
    for (auto resource : resources){
        if (resource.getType() == PASSENGER){
            try {
                train->unloading(resource);
            }
            catch(TrainException& exception){
                resource.setAmount(exception.getAmountOfExceptionalResource());
                flagOfException = true;
            }
            if (!flagOfException){
                resource.setAmount(0);
            }
            cout<<"The train \""<<train->getName()<<"\" is unloaded. "<<resource.getAmount()<<" passengers aren't stayed on the station \""<<getName()<<"\"."<<endl;
        }
    }
}

FreightStation::FreightStation(string name, int numberOfGoods) : Station(move(name)) {
    type = FREIGHT;
    resources.emplace_back(Resource(FREIGHT, numberOfGoods));
}

void FreightStation::transit(Train *train) {
    cout<<"Train "<<train->getName()<<" - passed the freight station "<<Station::getName()<<"."<<endl;
    train->departedFrom();
}

void FreightStation::temporaryStop(Train *train, int stopTime) {
    cout<<"The train"<<train->getName()<<" has a temporary stop on the freight station "<<Station::getName()<<"."<<endl;
    train->setTravelTime(stopTime);
}

void FreightStation::loading(Train *train) {
    bool flagOfException = false;
    for (auto resource : resources){
        if (resource.getType() == FREIGHT){
            try {
                train->loading(resource);
            }
            catch(TrainException& exception){
                resource.setAmount(exception.getAmountOfExceptionalResource());
                flagOfException = true;
            }
            if (!flagOfException){
                resource.setAmount(0);
            }
            cout<<"The train \""<<train->getName()<<"\" is loaded. "<<resource.getAmount()<<" passengers are stayed on the station \""<<getName()<<"\"."<<endl;
        }
    }
}

void FreightStation::unloading(Train *train) {
    bool flagOfException = false;
    for (auto resource : resources){
        if (resource.getType() == FREIGHT){
            try {
                train->unloading(resource);
            }
            catch(TrainException& exception){
                resource.setAmount(exception.getAmountOfExceptionalResource());
                flagOfException = true;
            }
            if (!flagOfException){
                resource.setAmount(0);
            }
            cout<<"The train \""<<train->getName()<<"\" is unloaded. "<<resource.getAmount()<<" passengers aren't stayed on the station \""<<getName()<<"\"."<<endl;
        }
    }
}

PassengerAndFreightStation::PassengerAndFreightStation(string name, int numberOfPassengers, int numberOfGoods)
        : Station(move(name)) {
    type = PASSENGER_AND_FREIGHT;
    resources.emplace_back(Resource(PASSENGER, numberOfPassengers));
    resources.emplace_back(Resource(FREIGHT, numberOfGoods));
}

void PassengerAndFreightStation::transit(Train *train) {
    cout<<"Train "<<train->getName()<<" - passed the passenger and freight station "<<Station::getName()<<"."<<endl;
    train->departedFrom();
}

void PassengerAndFreightStation::temporaryStop(Train *train, int stopTime) {
    cout<<"The train"<<train->getName()<<" has a temporary stop on the passenger and freight station "<<Station::getName()<<"."<<endl;
    train->setTravelTime(stopTime);
}
