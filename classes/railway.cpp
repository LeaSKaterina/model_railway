//
// Created by HP on 12.11.2020.
//

#include "railway.h"

using namespace rw;

#include <iostream>
#include <fstream>
#include <ctime>

int Railway::clock = 0;

void Railway::inputModelFromFile(const char *path) {
    try {
        Railway::map.inputMapFromFile(path);
        Railway::inputListOfTrainsFromFile(path);
    }
    catch (MapException &exception) {
        cout << exception.getError() << endl;
    }
}

void Railway::inputListOfTrainsFromFile(const char *path) {
    ifstream F;
    F.open(path);
    checkThatTheFileIsOpen(F);
    moveTheFileToTrains(F);
    int amountOfTrains;
    F >> amountOfTrains;
    goToTheNextLine(F);
    for (int i = 0; i < amountOfTrains; i++) {
        inputANewTrainFromFile(F);
    }
}

Railway::Railway(const char *path) {
    inputModelFromFile(path);
}

void Railway::liveAUnitOfTime() {
    for (auto &train : listOfTrains) {
        if (train.getStatus() != REMOVED_FROM_THE_ROUTE && train.getStatus() != PASSED_THE_ROUTE) {
            if (train.getTimeBeforeArrivalOrDeparture() == 0) {
                goToTheNextAction(train);
            } else {
                train.simplyExist();
            }
        }
    }
}

int
Railway::getRandomAction() {
    return rand() % 4 + 1;
}

int Railway::getRandomNumberOfResource() {
    return rand() % 1000 + 1;
}

void Railway::calculateTravelTime(Train *train) {
    if (!map.getPath(train->getDepartureStation(), train->getArrivalStation())) {
        throw MapException("There's no way between stations. Check the map. Train is removed form the route. ");
    }
    train->setTravelTime(
            int(10 * map.getPath(train->getDepartureStation(), train->getArrivalStation()) / train->getSpeed()));
}

void Railway::moveTheFileToTrains(ifstream &F) {
    short int numberOfStations;
    F >> numberOfStations;
    string bufferString;
    for (int i = 0; i <= numberOfStations * 2; i++) {
        getline(F, bufferString);
    }
}

void Railway::inputANewTrainFromFile(ifstream &F) {
    string bufferName;
    getline(F, bufferName);
    int bufferLocomotiveAge;
    F >> bufferLocomotiveAge;
    goToTheNextLine(F);
    string bufferVans;
    getline(F, bufferVans);
    Railway::listOfTrains.emplace_back(bufferName, bufferLocomotiveAge, bufferVans);
    string bufferRoute;
    getline(F, bufferRoute);
    createRouteForTheTrain(&listOfTrains[listOfTrains.size() - 1], bufferRoute);
    listOfTrains[listOfTrains.size() - 1].putAtTheBeginningOfTheRoute();
}

void Railway::goToTheNextAction(Train &train) {
    switch (train.getStatus()) {
        case ON_THE_WAY: {
            train.isArrived();
            if (train.theRouteIsNotPassed()) {
                train.setStatus(getRandomAction());
                performAnActionAtTheStation(train);
            }
            break;
        }
        case STANDING: {
            try {
                calculateTravelTime(&train);
                train.departedFrom();
            }
            catch (MapException &exception) {
                cout << "There is a problem with the train \"" << train.getName() << "\" on the station \""
                     << train.getCurrentStation()->getName() << "\":\n" << exception.getError() << endl;
                train.removeFromRoute();
            }
            break;
        }
        case IS_BEING_LOADED:
        case IS_BEING_UNLOADED: {
            try {
                train.calculateSpeed();
                calculateTravelTime(&train);
                train.departedFrom();
            }
            catch (MapException &exception) {
                cout << "There is a problem with the train \"" << train.getName() << "\" on the station \""
                     << train.getCurrentStation()->getName() << "\":\n" << exception.getError() << endl;
                train.removeFromRoute();
            }
            break;
        }
        default: {
            throw TrainException("Incorrect input: invalid status of train. ");
        }
    }
}

void Railway::performAnActionAtTheStation(Train &train) {
    switch (train.getStatus()) {
        case STANDING: {
            train.getCurrentStation()->temporaryStop(&train,
                                                     10);
            break;
        }
        case ON_THE_WAY: {
            train.getCurrentStation()->transit(&train);
            if (train.theRouteIsNotPassed()) {
                calculateTravelTime(&train);
            }
            break;
        }
        case IS_BEING_LOADED: {
            train.getCurrentStation()->loading(&train);
            break;
        }
        case IS_BEING_UNLOADED: {
            train.getCurrentStation()->unloading(&train);
            break;
        }
        default: {
            throw TrainException("Incorrect input: invalid type of action on the station. ");
        }
    }
}

void Railway::start() {
    trainsDepart();
    clock++;
    srand(time(nullptr));
    while (!allTrainsFinishedMoving()) {
        printCurrentTime();
        liveAUnitOfTime();
        clock++;
    }

}

void Railway::printCurrentTime() {
    cout << "Time: " << clock << "." << endl;
}

bool Railway::allTrainsFinishedMoving() {
    for (auto train : listOfTrains) {
        if (train.getStatus()) {
            return false;
        }
    }
    return true;
}

void Railway::createRouteForTheTrain(Train *train, string inputString) {
    string bufferName;
    for (int i = 0; i < inputString.size(); i++) {
        while (inputString[i] != ' ' && i < inputString.size()) {
            bufferName.push_back(inputString[i]);
            i++;
        }

        train->addStationToTheRoute(map.getStation(bufferName));
        bufferName.clear();
    }
}

void Railway::trainsDepart() {
    for (auto &train : listOfTrains) {
        calculateTravelTime(&train);
        train.departedFrom();
    }
}

void Railway::printParametersOfTrains() {
    for (auto &train : listOfTrains) {
        train.printParameters();
    }
}
