//
// Created by HP on 23.11.2020.
//

#include "map.h"
#include <iostream>
#include <fstream>
using namespace rw;

bool checkThatTheFileIsOpen(ifstream &F){
    if (!F.is_open()) {
        throw Exception("File not found. ");
    }
    return true;
}

void goToTheNextLine(ifstream &F) {
    string bufferString;
    getline(F,bufferString);
}

void Map::inputStationsFromFile(const char *path) {
    ifstream F;
    F.open(path);
    checkThatTheFileIsOpen(F);
    short int amountOfStations;
    F >> amountOfStations;
    for (int i = 0; i < amountOfStations; i++) {
        inputANewStationFromFile(F);
    }
    F.close();
}

void Map::inputPathsFromFile(const char *path) {
    ifstream F;
    F.open(path);
    checkThatTheFileIsOpen(F);
    short int numberOfStations;
    F >> numberOfStations;
    moveTheFileToPaths(F, numberOfStations);
    inputAdjacencyMatrix(F, numberOfStations);
    F.close();
}

void Map::inputMapFromFile(const char *path) {
    Map::inputStationsFromFile(path);
    Map::inputPathsFromFile(path);
}

Station *Map::getStation(string &nameOfStation) {
    for (auto &station : listOfStations) {
        if (station->getName() == nameOfStation) {
            return station;
        }
    }
    throw MapException("There's not such station. Check the name of station. ");
}

int Map::getPath(Station *station1, Station *station2) {
    return getElementOfTheAdjacencyMatrix(station1->getId(), station2->getId());
}

int Map::getElementOfTheAdjacencyMatrix(int i, int j) { //---------------------------------проверить корректность работы
    return listOfPaths[i][j];
}

void Map::moveTheFileToPaths(ifstream &F, int numberOfStations) {
    string bufferString;
    for (int i = 0; i <= numberOfStations; i++) {
        getline(F, bufferString);
    }
}

void Map::inputAdjacencyMatrix(ifstream &F, int numberOfStations) {
    int buffer{};
    for (int i = 0; i < numberOfStations; i++) {
        vector<int> bufferVector;
        for (int j = 0; j < numberOfStations; j++) {
            F >> buffer;
            bufferVector.push_back(buffer);
        }
        Map::listOfPaths.push_back(bufferVector);
    }
}

void Map::inputANewStationFromFile(ifstream &F) {
    short int bufferType;
    F>>bufferType;
    switch (bufferType) {
        case UNKNOWN_TYPE: {
            inputANewStationWithUnknownType(F);
            break;
        }
        case PASSENGER: {
            inputANewPassengerStation(F);
            break;
        }
        case FREIGHT: {
            inputANewFreightStation(F);
            break;
        }
        case PASSENGER_AND_FREIGHT: {
            inputANewPassengerAndFreightStation(F);
            break;
        }
        default: {
            throw MapException("Incorrect input: invalid type of new station. Check the data from file. ");
        }
    }
}

void Map::inputANewStationWithUnknownType(ifstream &F) {
    string bufferName;
    F >> bufferName;
    Map::listOfStations.push_back(new Station(bufferName));
}

void Map::inputANewPassengerStation(ifstream &F) {
    string bufferName;
    F >> bufferName;
    int bufferNumberOfPassengers;
    F >> bufferNumberOfPassengers;
    Map::listOfStations.push_back(new PassengerStation(bufferName, bufferNumberOfPassengers));
}

void Map::inputANewFreightStation(ifstream &F) {
    string bufferName;
    F >> bufferName;
    int bufferNumberOfGoods;
    F >> bufferNumberOfGoods;
    Map::listOfStations.push_back(new FreightStation(bufferName, bufferNumberOfGoods));
}

void Map::inputANewPassengerAndFreightStation(ifstream &F) {
    string bufferName;
    F >> bufferName;
    int bufferNumberOfPassengers;
    F >> bufferNumberOfPassengers;
    int bufferNumberOfGoods;
    F >> bufferNumberOfGoods;
    Map::listOfStations.push_back(
            new PassengerAndFreightStation(bufferName, bufferNumberOfPassengers, bufferNumberOfGoods));
}
