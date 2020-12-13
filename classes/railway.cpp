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
    F.close();
}

Railway::Railway(const char *path) {
    inputModelFromFile(path);
}

void Railway::liveAUnitOfTimeInAutomaticMode() {
    for (auto &train : listOfTrains) {
        if (train.getStatus() != REMOVED_FROM_THE_ROUTE && train.getStatus() != PASSED_THE_ROUTE) {
            if (train.getTimeBeforeArrivalOrDeparture() == 0) {
                goToTheNextActionInAutomaticMode(train);
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

void Railway::goToTheNextActionInAutomaticMode(Train &train) {
    try {
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
                calculateTravelTime(&train);
                train.departedFrom();
                break;
            }
            case IS_BEING_LOADED:
            case IS_BEING_UNLOADED: {
                train.calculateSpeed();
                calculateTravelTime(&train);
                train.departedFrom();
                break;
            }
            default: {
                throw TrainException("Incorrect input: invalid status of train. ");
            }
        }
    }
    catch (MapException &exception) {
        cout << "There is a problem with the train \"" << train.getName() << "\" on the station \""
             << train.getCurrentStation()->getName() << "\":\n" << exception.getError() << endl;
        train.removeFromRoute();
    }
}

void Railway::performAnActionAtTheStation(Train &train) {
    switch (train.getStatus()) {
        case STANDING: {
            train.getCurrentStation()->temporaryStop(&train, 10);
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
            throw TrainException("Incorrect input: invalid type of action on the station.");
        }
    }
}

void Railway::startAutomaticMode() {
    trainsDepart();
    clock++;
    srand(time(nullptr));
    while (!allTrainsFinishedMoving()) {
        printCurrentTime();
        liveAUnitOfTimeInAutomaticMode();
        clock++;
    }
    gameOver();
}

void Railway::printCurrentTime() {
    cout << "Time: " << clock << "." << endl;
}

bool Railway::allTrainsFinishedMoving() {
    for (auto &train : listOfTrains) {
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

void Railway::run(int modeOfWork) {
    switch (modeOfWork) {
        case AUTOMATIC: {
            startAutomaticMode();
            break;
        }
        case USER: {
            startUserMode();
            break;
        }
        default: {
            throw ModeException("Incorrect input: check the entered mode of work.");
        }
    }
}


void Railway::startUserMode() {
    trainsDepart();
    clock++;
    srand(time(nullptr));
    while (!allTrainsFinishedMoving()) {
        printCurrentTime();
        if (!liveAUnitOfTimeInUserMode()) {
            gameOver();
            return;
        }
        clock++;
    }
    cout << "All trains finished their moving." << endl;
    gameOver();
}

void Railway::printTheOptionsForUser() {
    cout << "1 - just look, what will happen next\n"
            "2 - restock the amount of resource on the station\n"
            "3 - remove the train from the route\n"
            "4 - choose the action at the station\n"
            "0 - exit" << endl;
}

bool Railway::liveAUnitOfTimeInUserMode() {
    for (auto &train : listOfTrains) {
        if (train.getStatus() != REMOVED_FROM_THE_ROUTE && train.getStatus() != PASSED_THE_ROUTE) {
            if (train.getTimeBeforeArrivalOrDeparture() == 0) {
                bool finishWithThisTrain = false;
                while (!finishWithThisTrain) {
                    finishWithThisTrain = performTheUserOption(train);
                }
            } else {
                train.simplyExist();
            }
        }
    }
    return true;
}

int Railway::userDecidesWhatToDo() {
    printTheOptionsForUser();
    int option;
    cin >> option;
    return option;
}

int Railway::userDecidesWhatToDoOnTheStation() {
    printTheActions();
    int action;
    cin >> action;
    return action;
}

void Railway::gameOver() {
    printParametersOfTrains();
    cout << "GAME OVER." << endl;
    clock = 0;
}

void Railway::printTheActions() {
    cout << "1 - temporary stop\n"
            "2 - transit (if the train is already at the station, it just will start moving)\n"
            "3 - loading\n"
            "4 - unloading\n";
}

void Railway::goToTheNextActionInUserMode(Train &train) {
    try {
        switch (train.getStatus()) {
            case ON_THE_WAY: {
                train.isArrived();
                if (train.theRouteIsNotPassed()) {
                    train.setStatus(userDecidesWhatToDoOnTheStation());
                    performAnActionAtTheStation(train);
                }
                break;
            }
            case STANDING: {
                if (!doAnythingElseOnTheStation(train)) {
                    calculateTravelTime(&train);
                    train.departedFrom();
                }
                break;
            }
            case IS_BEING_LOADED:
            case IS_BEING_UNLOADED: {
                if (!doAnythingElseOnTheStation(train)) {
                    train.calculateSpeed();
                    calculateTravelTime(&train);
                    train.departedFrom();
                }
                break;
            }
            default: {
                throw TrainException("Incorrect input: invalid status of train. ");
            }
        }
    }
    catch (MapException &exception) {
        cout << "There is a problem with the train \"" << train.getName() << "\" on the station \""
             << train.getCurrentStation()->getName() << "\":\n" << exception.getError() << endl;
        train.removeFromRoute();
    }
}

void Railway::userRestockTheResourceOnTheStation(Station *station) {
    cout << "Enter the type of resource:\n"
            "1 - passenger\n"
            "2 - freight" << endl;
    int type = getInt();
    cout << "Enter the amount of resource: " << endl;
    int amount = getInt();
    try {
        station->restockTheNumberOfResource(type, amount);
    }
    catch (ResourceException &exception) {
        cout << exception.getError() << endl;
    }
    catch (StationException &exception) {
        cout << exception.getError() << endl;
        cout << "Check the type of station." << endl;
    }
}

int Railway::getInt() {
    int buffer;
    cin >> buffer;
    return buffer;
}

int Railway::doAnythingElseOnTheStation(Train &train) {
    cout << "Train \"" << train.getName() << "\" stands at the station.\n"
                                             "0 - let him go on\n"
                                             "1 - choose the other action at the station (temporary stop/loading/unloading)"
         << endl;
    int choice = getInt();
    if (choice) {
        train.setStatus(userDecidesWhatToDoOnTheStation());
        performAnActionAtTheStation(train);
    }
    return choice;
}

bool Railway::performTheUserOption(Train &train) {
    train.printParameters();
    switch (userDecidesWhatToDo()) {
        case JUST_LOOK: {
            goToTheNextActionInAutomaticMode(train);
            return true;
        }
        case RESTOCK_THE_RESOURCE_OF_THE_STATION: {
            userRestockTheResourceOnTheStation(train.getCurrentStation());
            return false;
        }
        case REMOVE_TRAIN_FROM_THE_ROUTE: {
            train.removeFromRoute();
            return true;
        }
        case CHOOSE_THE_ACTION: {
            goToTheNextActionInUserMode(train);
            return true;
        }
        case EXIT: {
            return false;
        }
        default:{
            cout<<"Incorrect input. Try again."<<endl;
            return false;
        }
    }
}

