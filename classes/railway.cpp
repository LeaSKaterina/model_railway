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
    Railway::map.inputMapFromFile(path);
    Railway::inputListOfTrainsFromFile(path);
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

void Railway::putTrainsOnTheMap() {//---------------------------------ПРОВЕРИТЬ, МЕНЯЕТСЯ ЛИ МАРШРУТ (РАБОТА С ССЫЛКАМИ)
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
}

Railway::Railway(const char *path) {
    inputModelFromFile(path);
    putTrainsOnTheMap();
}

void Railway::liveAUnitOfTime() {
    for (auto &train : listOfTrains) {
        if (train.getStatus() != REMOVED_FROM_THE_ROUTE && train.getStatus() != PASSED_THE_ROUTE) {
            if (train.getTimeBeforeArrivalOrDeparture() == 0) {
                switch (train.getStatus()) {
                    case ON_THE_WAY: {
                        train.isArrived(); //смена станции отправления
                        /*определяется действие, которое нужно совершить на станции
                         *действие определяется рандомно || действие выбирает пользователь
                            транзит - calculateTravelTime(&train); train.departedFrom();
                            стоянка - "время до отправления" = 10 || "время до отправления" задает пользователь, статус меняем на "стоит"
                            разгрузка/загрузка - "время до отправления" задает пользователь || "время до отправления" = t,
                                                    t = const, разная для различный вагонов (сделать пассажирские, грузовые и миксованные поезда?)
                                                    - пассажирские вагоны загружаются быстро за фиксированный срок
                                                    - товарные вагоны загружаются/разгружаются дольше
                                                    - загрузка и тех, и тех - сумма по времени
                                                статус меняем на "загружается"/"разгружается"
                         *если поезд прибыл на конечную станцию, то uploadAll(station, train); train.passThaRoute();
                         */
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
                }
            } else {
                train.simplyExist();
            }
        }
    }
}

int
Railway::getRandomAction() { //-----------------------------------ПОДУМАТЬ НАД ЭТИМ, МОЖЕТ КАК-ТО ПО-ДРУГОМУ РЕАЛИЗОВАТЬ
    srand(time(nullptr));
    return rand() % 4 + 1;
}

void Railway::calculateTravelTime(Train *train) {
    if (!map.getPath(train->getDepartureStation(), train->getArrivalStation())) {
        //throw 123; //нет пути между этими станциями.
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
    string bufferRoute;
    getline(F, bufferRoute);
    Railway::listOfTrains.emplace_back(bufferName, bufferLocomotiveAge, bufferVans, bufferRoute);
}
