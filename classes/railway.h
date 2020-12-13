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

enum Option {
    EXIT,
    JUST_LOOK,
    RESTOCK_THE_RESOURCE_OF_THE_STATION,
    REMOVE_TRAIN_FROM_THE_ROUTE,
    CHOOSE_THE_ACTION
};

enum Mode {
    AUTOMATIC,
    USER
};

class ModeException : public Exception{
public:
    explicit ModeException(string error) : Exception(move(error)) {};
};

namespace rw {
    class Railway {
    private:
        Map map;
        vector<Train> listOfTrains;
        static int clock;

        static int getRandomAction();

        void calculateTravelTime(Train *train);

        static void moveTheFileToTrains(ifstream &F);

        void inputANewTrainFromFile(ifstream &F);

        void goToTheNextActionInAutomaticMode(Train &train);

        void goToTheNextActionInUserMode(Train &train);

        void performAnActionAtTheStation(Train &train);

        static void printCurrentTime();

        bool allTrainsFinishedMoving();

        void createRouteForTheTrain(Train *train, string inputString);

        void trainsDepart();

        static void printTheOptionsForUser();

        static void printTheActions();

        static int userDecidesWhatToDo();

        static int userDecidesWhatToDoOnTheStation();

        void gameOver();

        void startAutomaticMode();

        void startUserMode();

        void liveAUnitOfTimeInAutomaticMode();

        bool liveAUnitOfTimeInUserMode();

        static void userRestockTheResourceOnTheStation(Station *station);

        int doAnythingElseOnTheStation(Train &train);

        static int getInt();

        bool performTheUserOption(Train &train);

    public:
        explicit Railway(const char *path);

        void inputListOfTrainsFromFile(const char *path);

        void inputModelFromFile(const char *path);

        void printParametersOfTrains();

        void run(int modeOfWork);
    };

}

#endif //RAILWAY_RAILWAY_H
