//
// Created by HP on 23.11.2020.
//

#include "station.h"

int Station::counter = 0;

Station::Station(string name) {
    Station::name = move(name);
    Station::type = UNKNOWN_TYPE;
    Station::id = Station::counter;
    Station::counter++;
}

void Station::setName(string newName) {
    Station::name = move(newName);
}

string &Station::getName() {
    return Station::name;
}

Station::Station(string name, int numberOfResource) {
    Station::name = move(name);
    Station::type = UNKNOWN_TYPE;
    Station::resources.emplace_back(0, numberOfResource);
    Station::id = Station::counter;
    Station::counter++;
}

void Station::deleteTheStation(Station *station) {
    delete station;
    counter--;
}

int Station::getId() {
    return id;
}

PassengerStation::PassengerStation(string name, int numberOfPassengers) : Station(move(name)) {
    type = PASSENGER;
    resources.emplace_back(Resource(PASSENGER, numberOfPassengers));
}

FreightStation::FreightStation(string name, int numberOfGoods) : Station(move(name)) {
    type = FREIGHT;
    resources.emplace_back(Resource(FREIGHT, numberOfGoods));
}

PassengerAndFreightStation::PassengerAndFreightStation(string name, int numberOfPassengers, int numberOfGoods)
        : Station(move(name)) {
    type = PASSENGER_AND_FREIGHT;
    resources.emplace_back(Resource(PASSENGER, numberOfPassengers));
    resources.emplace_back(Resource(FREIGHT, numberOfGoods));
}
