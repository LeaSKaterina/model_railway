//
// Created by HP on 23.11.2020.
//

#include "train.h"
#include "van.h"

using namespace rw;
using namespace train;

void Route::addStation(Station *station) {
    listOfStops.push_back(station);
}

void Route::inputFromString(string &inputString) {
    string bufferName;
    for (int i = 0; i < inputString.size(); i++) {
        while (inputString[i] != ' ' && i < inputString.size()) {
            bufferName.push_back(inputString[i]);
            i++;
        }
        addStation(new Station(bufferName));
        bufferName.clear();
    }
}

vector<Station *> *Route::getListOfStops() {
    return &listOfStops;
}

Station *Route::getThePointOfDeparture() {
    return listOfStops[0];
}

void Train::moveAlongTheRoute() {
    currentDepartureStation++; //-----------------------------------------------------------------------ПРОВЕРИТЬ РАБОТУ
}

void Train::isArrived() {
    moveAlongTheRoute();
    cout << "The \"" << name << "\" train is arrived at \"" << currentDepartureStation << "\" station."
         << endl;//-----------------------------------------------COUT?
}

void Train::departedFrom() {
    cout << "The \"" << name << "\" train departed from \"" << currentDepartureStation << "\" station."
         << endl;//-----------------------------------------------COUT?
    Train::status = ON_THE_WAY;
    //если в расписании закончились станции, поезд благополучно доехал до последней, то статус меняется на "маршрут пройден" ------------------------ПРОВЕРКА НА ВОТ ЭТО ВОТ
}

void Train::simplyExist() {
    timeBeforeArrivalOrDeparture--;
}

Station *Train::getArrivalStation() {
    Station *nextDepartureStation = currentDepartureStation;
    return nextDepartureStation++; //-----------------------------------------ПРОВЕРИТЬ, НЕ СМЕЩАЕТСЯ ЛИ ТЕКУЩАЯ СТАНЦИЯ
}

Station *Train::getDepartureStation() {
    return currentDepartureStation;
}

void Train::removeFromRoute() {
    status = REMOVED_FROM_THE_ROUTE;
}

void Train::setTravelTime(int time) {
    timeBeforeArrivalOrDeparture = time;
}

int Train::getSpeed() {
    return speed;
}

void Train::passTheRoute() {
    status = PASSED_THE_ROUTE;
}

void Locomotive::calculateTractionForce(vector<Van *> &vans) {
    Locomotive::tractionForce = 0;
    for (auto &van : vans) {
        int force;
        if (van->getTypeOfVan() == 0 || van->getTypeOfVan() == 1) {
            force = 10;
        } else {
            force = 10 * van->getTypeOfVan();
        }
        Locomotive::tractionForce += int(van->getLoadCoefficient() * force);
    }
}

void Locomotive::calculateInitialSpeed() {
    if (age != 0) {
        initialSpeed = 100 / age;
    } else {
        //trow
        initialSpeed = 10;
    }
}

Locomotive::Locomotive() {
    age = 0;
    initialSpeed = 10;
    tractionForce = 1;
}

Locomotive::Locomotive(int age) {
    Locomotive::age = age;
    Locomotive::tractionForce = 1;
    Locomotive::calculateInitialSpeed();
}

int Locomotive::getInitialSpeed() {
    return initialSpeed;
}

int Locomotive::getTractionForce() {
    return tractionForce;
}

void Locomotive::setAge(int newAge) {
    Locomotive::age = newAge;
    Locomotive::calculateInitialSpeed();
}

int Train::loadingOfPassengerVan(int number, int numberOfPersons) {
    for (auto &van : Train::listOfVans) {
        if (van->getTypeOfVan() == PASSENGER && van->getNumber() == number) {
            return van->loading(numberOfPersons);
        }
    }
    //throw 123; //нет такого вагона
    return numberOfPersons;
}

int Train::loadingOfFreightVan(int number, int numberOfGoods) {
    for (auto &van : Train::listOfVans) {
        if (van->getTypeOfVan() == FREIGHT && van->getNumber() == number) {
            return van->loading(numberOfGoods);
        }
    }
    //throw 123; //нет такого вагона
    return numberOfGoods;
}


void Train::unloadingOfPassengerVan(int number, int numberOfPersons) {
    for (auto &van : Train::listOfVans) {
        if (van->getTypeOfVan() == PASSENGER && van->getNumber() == number) {
            van->unloading(numberOfPersons);
        }
    }
    //throw 123; //нет такого вагона
}

void Train::unloadingOfFreightVan(int number, int numberOfGoods) {
    for (auto &van : Train::listOfVans) {
        if (van->getTypeOfVan() == FREIGHT && van->getNumber() == number) {
            van->unloading(numberOfGoods);
        }
    }
    //throw 123; //нет такого вагона
}

void Train::inputListOfVansFromString(string &inputString) {
    for (int i = 0; i < inputString.length(); i++) {
        switch (int(inputString[i]) - 48) {
            case PASSENGER: {
                Train::listOfVans.push_back(new PassengerVan);
                break;
            }
            case FREIGHT: {
                Train::listOfVans.push_back(new FreightVan);
                break;
            }
            default: {
                cout << "Error." << endl;
                return;
            }
        }
        i++; //пробел, если это не конец
    }
}

Train::Train(string &name, int &locomotive, string &listOfVans, string &route) {
    Train::name = name;
    Train::inputListOfVansFromString(listOfVans);
    Train::route.inputFromString(route);
    Train::locomotive = Locomotive(locomotive);
    Train::updateTractionForceOfLocomotive();
    Train::calculateSpeed();
    Train::status = STANDING;
    currentDepartureStation = Train::route.getThePointOfDeparture();
}

void Train::calculateSpeed() {
    updateTractionForceOfLocomotive();
    Train::speed = Train::locomotive.getInitialSpeed() - int(0.25 * Train::locomotive.getTractionForce());
}

void Train::updateTractionForceOfLocomotive() {
    Train::locomotive.calculateTractionForce(listOfVans);
}

vector<Station *> *Train::getRoute() {
    return route.getListOfStops();
}

int Train::getTimeBeforeArrivalOrDeparture() {
    return timeBeforeArrivalOrDeparture;
}

int Train::getStatus() {
    return status;
}
