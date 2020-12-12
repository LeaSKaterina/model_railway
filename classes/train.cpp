//
// Created by HP on 23.11.2020.
//

#include "station.h"
#include "van.h"
#include "train.h"


using namespace rw;
using namespace train;

void Route::addStation(Station *station) {
    listOfStops.push_back(station);
}

vector<Station *> *Route::getListOfStops() {
    return &listOfStops;
}

Station *Route::getThePointOfDeparture() {
    if (listOfStops.empty()) {
        throw TrainException(
                "List of stops is empty."); //-------------------------------------------------try catch (?)
    }
    return listOfStops[0];
}

Station *Route::getThePointOfArrival() {
    if (listOfStops.empty()) {
        throw TrainException(
                "List of stops is empty."); //-------------------------------------------------try catch (?)
    }
    return listOfStops[listOfStops.size() - 1];
}

Station *Route::getTheNextStop(Station *currentStop) {
    for (int i = 0; i < listOfStops.size(); i++) {
        if (listOfStops[i]->getId() == currentStop->getId()) {
            return listOfStops[i + 1];
        }
    }
    throw TrainException("There isn't such station in the route of this train.");
}

void Train::moveAlongTheRoute() {
    currentDepartureStation = route.getTheNextStop(currentDepartureStation);
}

void Train::isArrived() {
    moveAlongTheRoute();
    cout << "The \"" << name << "\" train is arrived at \"" << currentDepartureStation->getName() << "\" station."
         << endl;
    if (currentDepartureStation == route.getThePointOfArrival()) {
        Train::passTheRoute();
    }
}

void Train::departedFrom() {
    if (currentDepartureStation == route.getThePointOfArrival()) {
        Train::passTheRoute();
    } else {
        cout << "The \"" << name << "\" train departed from \"" << currentDepartureStation->getName() << "\" station."
             << endl;
        Train::status = ON_THE_WAY;
    }
}

void Train::simplyExist() {
    timeBeforeArrivalOrDeparture--;
}

Station *Train::getArrivalStation() {
    return route.getTheNextStop(currentDepartureStation);
}

Station *Train::getDepartureStation() {
    return currentDepartureStation;
}

void Train::removeFromRoute() {
    cout << "Train " << Train::name << " is removed from the route. " << endl;
    status = REMOVED_FROM_THE_ROUTE;
}

void Train::setTravelTime(int time) {
    timeBeforeArrivalOrDeparture = time;
}

int Train::getSpeed() {
    return speed;
}

void Train::passTheRoute() {
    cout << "Train " << Train::name << " has passed the route. " << endl;
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
        Locomotive::tractionForce += int((1 + van->getLoadCoefficient()) * force);
    }
}

void Locomotive::calculateInitialSpeed() {
    if (age != 0) {
        initialSpeed = 100 / age;
    } else {
        initialSpeed = 30;
    }
}

Locomotive::Locomotive() {
    age = 0;
    initialSpeed = 30;
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

int Train::loading(Resource resource) {
    while (resource.getAmount() != 0 && !Train::isFullyLoadedBy(resource.getType())) {
        for (auto &van : Train::listOfVans) {
            if (resource.getType() == van->getTypeOfVan()) {
                if ((resource.getAmount() / 2) % 2 == 1) {
                    resource -= van->loading((resource.getAmount() / 2) + 1);
                } else {
                    resource -= van->loading(resource.getAmount() / 2);
                }
            }
        }
    }
    return resource.getAmount();
}

int Train::unloading(Resource resource) {
    while (resource.getAmount() != 0 && !Train::isFullyUnloadedBy(resource.getType())) {
        for (auto &van : Train::listOfVans) {
            if (resource.getType() == van->getTypeOfVan()) {
                if ((resource.getAmount() / 2) % 2 == 1) {
                    resource.setAmount(resource.getAmount() / 2 + van->unloading((resource.getAmount() / 2) + 1));
                } else {
                    resource.setAmount(resource.getAmount() / 2 + van->unloading(resource.getAmount() / 2));
                }
            }
        }
    }
    return resource.getAmount();
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
                throw TrainException("Incorrect input: invalid type of van. Check the data from file. ");
            }
        }
        i++;
    }
}

Train::Train(string &name, int &locomotiveAge, string &listOfVans) {
    Train::name = name;
    Train::inputListOfVansFromString(listOfVans);
    Train::locomotive = Locomotive(locomotive);
    Train::updateTractionForceOfLocomotive();
    Train::calculateSpeed();
    Train::status = STANDING;
}

void Train::calculateSpeed() {
    const int minTrainSpeed = 10;
    updateTractionForceOfLocomotive();
    Train::speed = Train::locomotive.getInitialSpeed() - int(0.025 * Train::locomotive.getTractionForce());
    if (Train::speed <= 0) {
        Train::speed = minTrainSpeed;
    }
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
    if (action < 0 || action > 4) {
        throw TrainException(
                "Incorrect input: invalid train status value. "); //-------------------------------------------------try catch (?)
    }
    status = action;
}

string &Train::getName() {
    return name;
}

Station *Train::getCurrentStation() {
    return currentDepartureStation;
}

void Train::addStationToTheRoute(Station *newStation) {
    route.addStation(newStation);
}

void Train::putAtTheBeginningOfTheRoute() {
    currentDepartureStation = route.getThePointOfDeparture();
}

bool Train::theRouteIsNotPassed() {
    return status;
}

int Train::getAmountOfResource(int type) {
    int amountOfResource = 0;
    for (auto &van : listOfVans) {
        if (van->getTypeOfVan() == type)
            amountOfResource += van->getCurrentLoad();
    }
    return amountOfResource;
}

int Train::getAmountOfVans() {
    return listOfVans.size();
}

int Train::getAmountOfPassengerVans() {
    int amount = 0;
    for (auto &van : listOfVans) {
        if (van->getTypeOfVan() == PASSENGER) {
            amount++;
        }
    }
    return amount;
}

int Train::getAmountOfFreightVans() {
    int amount = 0;
    for (auto &van : listOfVans) {
        if (van->getTypeOfVan() == FREIGHT) {
            amount++;
        }
    }
    return amount;
}

bool Train::isFullyLoadedBy(int typeOfResource) {
    for (auto van : listOfVans) {
        if (van->getTypeOfVan() == typeOfResource && !van->isLoaded()) {
            return false;
        }
    }
    return true;
}

bool Train::isFullyUnloadedBy(int typeOfResource) {
    for (auto van : listOfVans) {
        if (van->getTypeOfVan() == typeOfResource && van->getCurrentLoad()) {
            return false;
        }
    }
    return true;
}

void Train::printParameters() {
    cout << getName() << ": ";
    cout << "passengers : " << getAmountOfResource(PASSENGER) << ", ";
    cout << "freight: " << getAmountOfResource(FREIGHT) << ". ";
    cout << "Current station: " << getCurrentStation()->getName() << endl;
}


