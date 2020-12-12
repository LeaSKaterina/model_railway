//
// Created by HP on 23.11.2020.
//

#ifndef RAILWAY_STATION_H
#define RAILWAY_STATION_H

#include "train.h"
#include <string>
#include <vector>
#include <iostream>

using namespace rw;
using namespace train;
using namespace std;

class StationException : public Exception {
public:
    explicit StationException(string error) : Exception(move(error)) {};
};

namespace rw {
    class Station {
    private:
        string name;
        int id;
        static int counter;
    protected:
        vector<Resource> resources{};
        short int type;

        static void checkTypeOfResource(int type);

        static bool thereAreNotPassengerVans(Train *train);

        static bool thereAreNotFreightVans(Train *train);

    public:
        explicit Station(string name);

        explicit Station(string name, Resource resource);

        void setName(string newName);

        string &getName();

        static void deleteTheStation(Station *station);

        int getId();

        int getAmountOfResource(int typeOfResource);

        void setTheFirstResource(Resource theFirstRecourse);

        void addTheResource(Resource newRecourse);

        void deleteTheResource(int typeOfDeletedResource);

        //void reduceTheNumberOfResource(int typeOfResource, int numberOfResource);
        //void restockTheNumberOfResource(int typeOfResource, int numberOfResource);
        virtual void temporaryStop(Train *train, int stopTime);

        virtual void transit(Train *train);

        virtual void loading(Train *train);

        virtual void unloading(Train *train);
    };

    class PassengerStation : public Station {
    public:
        explicit PassengerStation(string name, int numberOfPassengers);

        void loading(Train *train) override;

        void unloading(Train *train) override;

        void temporaryStop(Train *train, int stopTime) override;

        void transit(Train *train) override;
    };

    class FreightStation : public Station {
    public:
        explicit FreightStation(string name, int numberOfGoods);

        void loading(Train *train) override;

        void unloading(Train *train) override;

        void temporaryStop(Train *train, int stopTime) override;

        void transit(Train *train) override;
    };

    class PassengerAndFreightStation : public Station {
    public:
        explicit PassengerAndFreightStation(string name, int numberOfPassengers, int numberOfGoods);

        void loading(Train *train) override;

        void unloading(Train *train) override;

        void temporaryStop(Train *train, int stopTime) override;

        void transit(Train *train) override;
    };

}

#endif //RAILWAY_STATION_H
