//
// Created by HP on 23.11.2020.
//

#include "station.h"

int Station::counter = 0;

Station::Station(string name) {
    Station::name = move(name);
    Station::type = UNKNOWN_TYPE;
    Station::id = Station::counter;
    Station::setTheFirstResource(Resource(UNKNOWN_TYPE, 100));
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
    cout << "Train " << train->getName() << " - passed the station (transit)" << Station::getName() << "." << endl;
    train->departedFrom();
}

void Station::temporaryStop(Train *train, int stopTime) {
    cout << "The train" << train->getName() << " has a temporary stop on the station " << Station::getName() << "."
         << endl;
    train->setTravelTime(stopTime);
}

int Station::getAmountOfResource(int typeOfResource) {
    checkTypeOfResource(typeOfResource);
    for (auto &resource : resources) {
        if (resource.getType() == typeOfResource) {
            return resource.getAmount();
        }
    }
    throw StationException("There isn't such resource. Check the type of resource. ");
}

void Station::setTheFirstResource(Resource theFirstRecourse) {
    resources.emplace_back(theFirstRecourse);
}

void Station::addTheResource(Resource newRecourse) {
    for (auto &resource : resources) {
        if (resource.getType() == newRecourse.getType()) {
            resource.restock(newRecourse.getAmount());
            return;
        }
    }
    resources.emplace_back(newRecourse);
}

void Station::deleteTheResource(int typeOfDeletedResource) {
    checkTypeOfResource(typeOfDeletedResource);
    for (int i = 0; i < resources.size(); i++) {
        if (resources[i].getType() == typeOfDeletedResource) {
            resources.erase(resources.begin() + i);
            return;
        }
    }
    throw StationException("There isn't such resource. Check the type of deleted resource .");
}

void Station::checkTypeOfResource(int type) {
    if (type < 0 || type > 3) {
        throw ResourceException(
                "Incorrect input: invalid type value. ");
    }
}

void Station::restockTheNumberOfResource(int typeOfResource, int numberOfResource) {
    checkTypeOfResource(typeOfResource);
    for (auto &resource : resources){
        if (resource.getType() == typeOfResource){
            resource.restock(numberOfResource);
            return;
        }
    }
    throw StationException("There isn't such resource. Check the type of resource.");
}

void Station::loading(Train *train) {
    for (auto &resource : resources) {
        resource.setAmount(train->loading(resource));
        cout << "Type of loaded resource: " << resource.getType() << ", " <<
             "The new amount of this resource: " << resource.getAmount() << ". " << endl;
    }
    cout << "\"" << train->getName() << "\" is being loaded on the \"" << Station::name << "\" station " << endl;
    train->setTravelTime(train->getAmountOfVans());
}

void Station::unloading(Train *train) {
    for (auto &resource : resources) {
        resource.setAmount(train->unloading(resource));
        cout << "Type of unloaded resource: " << resource.getType() << ", " <<
             "The new amount of this resource: " << resource.getAmount() << ". " << endl;
    }
    cout << "\"" << train->getName() << "\" is being unloaded on the \"" << Station::name << "\" station " << endl;
    train->setTravelTime(train->getAmountOfVans());
}

PassengerStation::PassengerStation(string name, int numberOfPassengers) : Station(move(name)) {
    type = PASSENGER;
    resources.clear();
    resources.emplace_back(Resource(PASSENGER, numberOfPassengers));
}

void PassengerStation::transit(Train *train) {
    cout << "Train " << train->getName() << " - passed the passenger station " << Station::getName() << "." << endl;
    train->departedFrom();
}

void PassengerStation::temporaryStop(Train *train, int stopTime) {
    cout << "The train \"" << train->getName() << "\" has a temporary stop on the passenger station "
         << Station::getName() << "." << endl;
    train->setTravelTime(stopTime);
}

void PassengerStation::loading(Train *train) {
    if (thereAreNotPassengerVans(train)) {
        return;
    }
    resources[0].setAmount(train->loading(resources[0]));
    cout << "The train \"" << train->getName() << "\" is being loaded. " << resources[0].getAmount()
         << " passengers will stayed on the passenger station \"" << getName() << "\"." << endl;
    train->setTravelTime(train->getAmountOfVans());
}

void PassengerStation::unloading(Train *train) {
    if (thereAreNotPassengerVans(train)) {
        return;
    }
    resources[0].setAmount(train->unloading(Resource(PASSENGER, train->getAmountOfResource(PASSENGER))));
    cout << "The train \"" << train->getName() << "\" is being unloaded on the passenger station \"" << getName()
         << "\". " << resources[0].getAmount() << " passengers will stayed in the train." << endl;
    train->setTravelTime(train->getAmountOfVans());
}

string PassengerStation::getTypeAsAString() {
    return "passenger";
}

void PassengerStation::printParameters() {
    cout << "\""<< getName() << "\" " << getTypeAsAString() <<" station, amount of passengers: "<<getAmountOfResource(PASSENGER) << endl;
}

bool Station::thereAreNotPassengerVans(Train *train) {
    return train->getAmountOfPassengerVans() == 0;
}

FreightStation::FreightStation(string name, int numberOfGoods) : Station(move(name)) {
    type = FREIGHT;
    resources.clear();
    resources.emplace_back(Resource(FREIGHT, numberOfGoods));
}

void FreightStation::transit(Train *train) {
    cout << "Train " << train->getName() << " - passed the freight station " << Station::getName() << "." << endl;
    train->departedFrom();
}

void FreightStation::temporaryStop(Train *train, int stopTime) {
    cout << "The train \"" << train->getName() << "\" has a temporary stop on the freight station "
         << Station::getName() << "." << endl;
    train->setTravelTime(stopTime);
}

void FreightStation::loading(Train *train) {
    if (thereAreNotFreightVans(train)) {
        return;
    }
    resources[0].setAmount(train->loading(resources[0]));
    cout << "The train \"" << train->getName() << "\" is being loaded. " << resources[0].getAmount()
         << " goods will stayed on the freight station \"" << getName() << "\"." << endl;
    train->setTravelTime(2 * train->getAmountOfVans());
}

void FreightStation::unloading(Train *train) {
    if (thereAreNotFreightVans(train)) {
        return;
    }
    resources[0].setAmount(train->unloading(Resource(FREIGHT, train->getAmountOfResource(FREIGHT))));
    cout << "The train \"" << train->getName() << "\" is being unloaded on the freight station \"" << getName()
         << "\". " << resources[0].getAmount() << " goods will stayed in the train." << endl;
    train->setTravelTime(2 * train->getAmountOfVans());
}

string FreightStation::getTypeAsAString() {
    return "freight";
}

void FreightStation::printParameters() {
    cout <<"\""<< getName() << "\" " <<getTypeAsAString()<<" station, amount of goods: "
         << getAmountOfResource(FREIGHT) << endl;
}

bool Station::thereAreNotFreightVans(Train *train) {
    return train->getAmountOfFreightVans() == 0;
}

void Station::printParameters() {
    cout <<"\""<< getName() << "\" station, type " << getTypeAsAString()<<endl;
}

string Station::getTypeAsAString() {
    return "unknown type";
}

PassengerAndFreightStation::PassengerAndFreightStation(string name, int numberOfPassengers, int numberOfGoods)
        : Station(move(name)) {
    type = PASSENGER_AND_FREIGHT;
    resources.clear();
    resources.emplace_back(Resource(PASSENGER, numberOfPassengers));
    resources.emplace_back(Resource(FREIGHT, numberOfGoods));
}

void PassengerAndFreightStation::transit(Train *train) {
    cout << "Train " << train->getName() << " - passed the passenger and freight station " << Station::getName() << "."
         << endl;
    train->departedFrom();
}

void PassengerAndFreightStation::temporaryStop(Train *train, int stopTime) {
    cout << "The train \"" << train->getName() << "\" has a temporary stop on the passenger and freight station "
         << Station::getName() << "." << endl;
    train->setTravelTime(stopTime);
}

void PassengerAndFreightStation::loading(Train *train) {
    cout << "The train \"" << train->getName() << "\" is being loaded. " << endl;
    loadingOfPassengerVans(train);
    loadingOfFreightVans(train);
    train->setTravelTime(train->getAmountOfPassengerVans() + 2 * train->getAmountOfFreightVans());
}

void PassengerAndFreightStation::unloading(Train *train) {
    cout << "The train \"" << train->getName() << "\" is being unloaded on the passenger and freight station \""
         << getName() << "\". ";
    unloadingOfPassengerVans(train);
    unloadingOfFreightVans(train);
    cout << "will stayed in the train." << endl;
    train->setTravelTime(train->getAmountOfPassengerVans() + 2 * train->getAmountOfFreightVans());
}

string PassengerAndFreightStation::getTypeAsAString() {
    return "passenger and freight";
}

void PassengerAndFreightStation::printParameters() {
    cout <<"\""<< getName() << "\" "<<getTypeAsAString()<<" station,\namount of passengers: "<<getAmountOfResource(PASSENGER) << ", amount of goods: "
         << getAmountOfResource(FREIGHT) << endl;
}

void PassengerAndFreightStation::loadingOfPassengerVans(Train *train) {
    for (auto &resource : resources) {
        if (resource.getType() == PASSENGER) {
            if (thereAreNotPassengerVans(train)) {
                break;
            }
            resource.setAmount(train->loading(resource));
            cout << resource.getAmount() << " passengers will stayed on the passenger and freight station \""
                 << getName() << "\"." << endl;
        }
    }
}

void PassengerAndFreightStation::loadingOfFreightVans(Train *train) {
    for (auto &resource : resources) {
        if (resource.getType() == FREIGHT) {
            if (thereAreNotFreightVans(train)) {
                break;
            }
            resource.setAmount(train->loading(resource));
            cout << resource.getAmount() << " goods will stayed on the passenger and freight station \"" << getName()
                 << "\"." << endl;
        }
    }
}

void PassengerAndFreightStation::unloadingOfPassengerVans(Train *train) {
    for (auto &resource : resources) {
        if (resource.getType() == PASSENGER) {
            if (thereAreNotPassengerVans(train)) {
                return;
            }
            resource.setAmount(train->unloading(Resource(PASSENGER, train->getAmountOfResource(PASSENGER))));
            cout << resource.getAmount() << " passengers ";
        }
    }
}

void PassengerAndFreightStation::unloadingOfFreightVans(Train *train) {
    for (auto &resource : resources) {
        if (resource.getType() == FREIGHT) {
            if (thereAreNotFreightVans(train)) {
                return;
            }
            resource.setAmount(train->unloading(Resource(FREIGHT, train->getAmountOfResource(FREIGHT))));
            cout << "and " << resource.getAmount() << " goods ";
        }
    }
}
