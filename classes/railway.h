//
// Created by HP on 12.11.2020.
//

#ifndef RAILWAY_RAILWAY_H
#define RAILWAY_RAILWAY_H

#include <string>
#include <vector>
#include "map.h"

using namespace std;

enum Action {
    TEMPORARY_STOP = 1,
    TRANSIT,
    LOADING,
    UNLOADING
};

enum Mode {
    AUTOMATIC,
    USER
};

namespace rw {
    //режим работы - поле класса Railway, не читается из файла, а вводится вместе с названием файла входных данных
    class Railway {
    private:
        Map map;
        vector<Train> listOfTrains;
        static int clock;

        static int getRandomAction();

        void calculateTravelTime(Train *train);

        static void moveTheFileToTrains(ifstream &F);

        void inputANewTrainFromFile(ifstream &F);

        static int getRandomNumberOfResource();

        void goToTheNextAction(Train &train);

        void performAnActionAtTheStation(Train &train);

        static void printCurrentTime();

        bool allTrainsFinishedMoving();

        void createRouteForTheTrain(Train *train, string inputString);

        void trainsDepart();

    public:
        explicit Railway(const char *path);

        void inputListOfTrainsFromFile(const char *path);

        void inputModelFromFile(const char *path);

        void start();

        void liveAUnitOfTime();

        void printParametersOfTrains();
    };

}

#endif //RAILWAY_RAILWAY_H
