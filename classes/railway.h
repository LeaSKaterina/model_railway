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
    LOADING,
    UNLOADING,
    TRANSIT,
    TEMPORARY_STOP
};

namespace rw {

    //на выходе должен быть список событий в хронологическом порядке
    //автоматическое моделирование - вывод от t=0 до t=момент, когда последний поезд доедет (станет неактивным)
    //режим работы - поле класса Railway, не читается из файла, а вводится вместе с названием файла входных данных

    class Railway {
    private:
        Map map;
        vector<Train> listOfTrains;
        static int clock;

        static int getRandomAction();

        void calculateTravelTime(Train *train);

        void moveTheFileToTrains(ifstream &F);

        void inputANewTrainFromFile(ifstream& F);

    public:
        explicit Railway(const char *path);

        void inputListOfTrainsFromFile(const char *path);

        void inputModelFromFile(const char *path);

        void putTrainsOnTheMap();

        void liveAUnitOfTime(); //-------------------------------------------------------------------------------------------------------ДОДЕЛАТЬ
    };

}

#endif //RAILWAY_RAILWAY_H
