//
// Created by HP on 23.11.2020.
//

#ifndef RAILWAY_TRAIN_H
#define RAILWAY_TRAIN_H

#include "van.h"
#include <string>
#include <vector>

using namespace std;

class TrainException : public Exception {
public:
    explicit TrainException(string error) : Exception(move(error)) {};
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

            vector<Station *> *getListOfStops();

            Station *getThePointOfDeparture();

            Station *getThePointOfArrival();

            Station *getTheNextStop(Station *currentStop);
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
            Route route{};
            Station *currentDepartureStation{};
            Locomotive locomotive;
            vector<Van *> listOfVans;
            int timeBeforeArrivalOrDeparture = 0;
            int status;
            int speed{};

            void inputListOfVansFromString(string &inputString);

            void moveAlongTheRoute();

            void updateTractionForceOfLocomotive();

            bool isFullyLoadedBy(int typeOfResource);

            bool isFullyUnloadedBy(int typeOfResource);

            string getStatusAsAString();

        public:
            Train(string &name, int &locomotiveAge, string &listOfVans);

            vector<Station *> *getRoute();

            int getTimeBeforeArrivalOrDeparture();

            int getStatus();

            int getSpeed();

            int getAmountOfPassengerVans();

            int getAmountOfFreightVans();

            int getAmountOfVans();

            Station *getDepartureStation();

            Station *getCurrentStation();

            Station *getArrivalStation();

            string &getName();

            int getAmountOfResource(int type);

            void putAtTheBeginningOfTheRoute();

            void calculateSpeed();

            void setStatus(int action);

            void addStationToTheRoute(Station *newStation);

            int loading(Resource resource);

            int unloading(Resource resource);

            void isArrived();

            void departedFrom();

            void simplyExist();

            void setTravelTime(int time);

            void removeFromRoute();

            void passTheRoute();

            bool theRouteIsNotPassed();

            void printParameters();
        };

    }

}

#endif //RAILWAY_TRAIN_H
