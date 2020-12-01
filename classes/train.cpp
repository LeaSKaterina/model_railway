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
    if (listOfStops.empty()){
        throw TrainException("List of stops is empty."); //-------------------------------------------------try catch (?)
    }
    return listOfStops[0];
}

Station *Route::getThePointOfArrival() {
    if (listOfStops.empty()){
        throw TrainException("List of stops is empty."); //-------------------------------------------------try catch (?)
    }
    return listOfStops[listOfStops.size()-1];
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
    if (currentDepartureStation == route.getThePointOfArrival()){
        Train::passTheRoute();
    } else {
        cout << "The \"" << name << "\" train departed from \"" << currentDepartureStation << "\" station."
             << endl;//-----------------------------------------------COUT?
        Train::status = ON_THE_WAY;
    }
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
    cout<<"Train "<<Train::name<<" is removed from the route. "<<endl;
    //Train::currentDepartureStation->unloading(this); //------------------------------------------------РАЗГРУЗИТЬ ВСЕ
    status = REMOVED_FROM_THE_ROUTE;
}

void Train::setTravelTime(int time) {
    timeBeforeArrivalOrDeparture = time;
}

int Train::getSpeed() {
    return speed;
}

void Train::passTheRoute() {
    cout<<"Train "<<Train::name<<" has passed the route. "<<endl;
    //Train::currentDepartureStation->unloading(this); //------------------------------------------------РАЗГРУЗИТЬ ВСЕ
    Train::status = PASSED_THE_ROUTE;
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

int Train::loading(Resource resource){
    while (resource.getAmount()!=0 && !Train::isFullyLoaded() ) {
        for (auto &van : Train::listOfVans) {
            try {
                if (resource.getType() == van->getTypeOfVan()) {
                    if ((resource.getAmount() / 2) % 2 == 1) {
                        van->loading((resource.getAmount() / 2) + 1);
                    } else {
                        van->loading(resource.getAmount() / 2);
                    }
                    resource.setAmount(resource.getAmount() / 2);
                }
            }
            catch(VanException& exception){
                cout<<"The van №"<<van->getNumber()<<": ";
                cout << exception.getError() << exception.getAmountOfExceptionalResource() << " aren't loaded." << endl;
                resource-=van->getMaximumLoad();
            }
        }
    }
    return resource.getAmount(); //-------------------------------------throw TrainException("There're not enough free space. The following amount of resource isn't loaded. ", resource.getAmount);
}

void Train::unloading(Resource resource) {
    while (resource.getAmount()!=0 && !Train::isFullyUnloaded() ) {
        for (auto &van : Train::listOfVans) {
            try {
                if (resource.getType() == van->getTypeOfVan()) {
                    if ((resource.getAmount() / 2) % 2 == 1) {
                        van->unloading((resource.getAmount() / 2) + 1);
                    } else {
                        van->unloading(resource.getAmount() / 2);
                    }
                    resource.setAmount(resource.getAmount() / 2);
                }
            }
            catch(VanException& exception){
                cout<<"The van №"<<van->getNumber()<<": ";
                cout << exception.getError() << exception.getAmountOfExceptionalResource() << " aren't unloaded." << endl;
                resource -= exception.getAmountOfExceptionalResource();
            }
        }
    }
    //throw TrainException("There're not enough resources. The following amount of resource isn't unloaded. ", resource.getAmount);
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
        i++;
    }
}

Train::Train(string &name, int &locomotiveAge, string &listOfVans, string &route) {
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

void Train::setStatus(int action) {
    if (action < 0 || action > 4){
        throw TrainException("Incorrect input: invalid train status value. "); //-------------------------------------------------try catch (?)
    }
    status = action;
}

string &Train::getName() {
    return name;
}

Station *Train::getCurrentStation() {
    return currentDepartureStation;
}

bool Train::isFullyLoaded() {
    for (auto van : listOfVans){
        if (!van->isLoaded()){
            return false;
        }
    }
    return true;
}

bool Train::isFullyUnloaded() {
    for (auto van : listOfVans){
        if (!van->isEmpty()){
            return false;
        }
    }
    return true;
}


