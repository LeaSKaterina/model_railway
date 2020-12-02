//
// Created by HP on 12.11.2020.
//

#include "railway.h"

using namespace rw;

#include <iostream>
#include <fstream>
#include <ctime>
//#include <utility>

int Railway::clock = 0;

void Railway::inputModelFromFile(const char *path) {
    try {
        Railway::map.inputMapFromFile(path);
        Railway::inputListOfTrainsFromFile(path);
    }
    catch (Exception &exception){
        cout<<exception.getError()<<endl;
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

/*void Railway::putTrainsOnTheMap() {//---------------------------------ПРОВЕРИТЬ, МЕНЯЕТСЯ ЛИ МАРШРУТ (РАБОТА С ССЫЛКАМИ)
    vector<Station *> *route{};
    Station *buffer{};
    for (auto &train : listOfTrains) {
        route = train.getRoute();
        for (auto &stop : *route) {
            buffer = stop;
            stop = map.getStation(stop->getName());
            Station::deleteTheStation(buffer);
        }
    }
}*/

Railway::Railway(const char *path) {
    inputModelFromFile(path);
    //putTrainsOnTheMap();
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
    //srand(time(nullptr)); //-----------------------------------------------------------------------------randomize()??
    return rand() % 4 + 1;
}

int Railway::getRandomNumberOfResource() {
    return rand() % 1000 + 1;
}

void Railway::calculateTravelTime(Train *train) {
    if (!map.getPath(train->getDepartureStation(), train->getArrivalStation())) {
        throw MapException("There's no way between these stations. Check the map. Train is removed form the route. ");
        train->removeFromRoute(); //внутри catch
    }
    train->setTravelTime(
            int(map.getPath(train->getDepartureStation(), train->getArrivalStation()) / train->getSpeed()));
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
            train.isArrived(); //смена станции отправления
            train.setStatus(getRandomAction()); //смена статуса в автоматическом режиме
            performAnActionAtTheStation(train);
            break;
        }
        case STANDING: {
            calculateTravelTime(&train);
            train.departedFrom();
            break;
        }
        case IS_BEING_LOADED | IS_BEING_UNLOADED: {
            train.calculateSpeed();
            calculateTravelTime(&train);
            train.departedFrom();
            break;
        }
        default:{
            throw TrainException("Incorrect input: invalid status of train. ");
        }
    }
}

void Railway::performAnActionAtTheStation(Train &train) {
    switch (train.getStatus()){
        case STANDING:{
            train.getCurrentStation()->temporaryStop(&train, 10); //автоматический режим (в пользовательнском stopTime вводит пользователь?)
            break;
        }
        case ON_THE_WAY:{
            train.getCurrentStation()->transit(&train);
            calculateTravelTime(&train);
            break;
        }
        case IS_BEING_LOADED:{
            //train.getCurrentStation()->loading(&train);
            break;
        }
        case IS_BEING_UNLOADED:{
            //train.getCurrentStation()->unloading(&train);
            break;
        }
        default:{
            throw TrainException("Incorrect input: invalid type of action on the station. ");
        }
    }
}

void Railway::start() {
    while (!allTrainsFinishedMoving()){
        printCurrentTime();
        liveAUnitOfTime();
        clock++;
    }
}

void Railway::printCurrentTime() {
    cout <<"Time: "<<clock<<"."<<endl;
}

bool Railway::allTrainsFinishedMoving() {
    for (auto train : listOfTrains){
        if (train.getStatus()){
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
