//
// Created by HP on 23.11.2020.
//

#ifndef RAILWAY_MAP_H
#define RAILWAY_MAP_H


#include "station.h"
#include <string>
#include <vector>

class MapException : public Exception{
public:
    explicit MapException(string error) : Exception(move(error)){};
};

bool checkThatTheFileIsOpen(ifstream &F);
void goToTheNextLine(ifstream &F);

namespace rw{

    class Map {
    private:
        vector<Station *> listOfStations;
        vector<vector<int>> listOfPaths;

        static void moveTheFileToPaths(ifstream &F, int numberOfStations);

        void inputAdjacencyMatrix(ifstream &F, int numberOfStations);

        int getElementOfTheAdjacencyMatrix(int i, int j);

        void inputANewStationFromFile(ifstream& F);

        void inputANewStationWithUnknownType(ifstream &F);

        void inputANewPassengerStation(ifstream &F);

        void inputANewFreightStation(ifstream &F);

        void inputANewPassengerAndFreightStation(ifstream &F);

    public:

        void inputStationsFromFile(const char *path);

        void inputPathsFromFile(const char *path);

        void inputMapFromFile(const char *path);

        Station *getStation(string &nameOfStation);

        int getPath(Station *station1, Station *station2);
    };
}

#endif //RAILWAY_MAP_H
