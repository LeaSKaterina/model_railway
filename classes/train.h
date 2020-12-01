//
// Created by HP on 23.11.2020.
//

#ifndef RAILWAY_TRAIN_H
#define RAILWAY_TRAIN_H

#include "van.h"
#include "station.h"
#include <string>
#include <vector>
using namespace std;

class TrainException : public Exception{
public:
    TrainException(string error) : Exception(move(error)){};
};

enum StatusOfTheTrain {
    REMOVED_FROM_THE_ROUTE = 0,
    PASSED_THE_ROUTE = 0,
    STANDING,
    ON_THE_WAY,
    IS_BEING_LOADED,
    IS_BEING_UNLOADED
};

namespace rw {

    class Station;

    namespace train {

        class Route {
        private:
            vector<Station *> listOfStops;
        public:
            void addStation(Station *station);

            void inputFromString(string &inputString);

            vector<Station *> *getListOfStops();

            Station *getThePointOfDeparture();

            Station *getThePointOfArrival();
        };

        class Locomotive {
        private:
            int tractionForce{};
            int initialSpeed{};
            int age;

            void calculateInitialSpeed();

        public:
            Locomotive();

            explicit Locomotive(int age);

            void calculateTractionForce(vector<Van *> &vans);

            int getInitialSpeed();

            int getTractionForce();

            void setAge(int newAge);

        };

        class Train {
        private:
            string name;
            Route route;
            Station *currentDepartureStation;
            Locomotive locomotive;
            vector<Van *> listOfVans;
            int timeBeforeArrivalOrDeparture = 0;
            int status;
            int speed{};

            void inputListOfVansFromString(string &inputString);

            void moveAlongTheRoute();

            void updateTractionForceOfLocomotive();

            bool isFullyLoaded();

            bool isFullyUnloaded();

        public:
            Train(string &name, int &locomotiveAge, string &listOfVans, string &route);

            vector<Station *> *getRoute();

            int getTimeBeforeArrivalOrDeparture();

            int getStatus();

            int getSpeed();

            Station *getDepartureStation();

            Station *getCurrentStation();

            Station *getArrivalStation();

            string &getName();

            void calculateSpeed();

            void setStatus(int action);

            //--------------------------------------------------------------------------------------------------------------------------ДОДЕЛАТЬ-----------
            int loading(Resource resource);

            void unloading(Resource resource);
            //----------------------------------------------------------------------------------------------------------------------------------------------

            void isArrived();

            void departedFrom();

            void simplyExist();

            void setTravelTime(int time);

            void removeFromRoute();

            void passTheRoute();
        };

    }

}

#endif //RAILWAY_TRAIN_H
