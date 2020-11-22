//
// Created by HP on 12.11.2020.
//

#include "railway.h"

using namespace rw;

#include <iostream>
#include <fstream>
#include <ctime>
//#include <utility>

bool checkThatTheFileIsOpen(ifstream &F){
    if (!F.is_open()) {
        cout << "Error" << endl;
        //throw 123;
        return false;
    }
    return true;
}

void goToTheNextLine(ifstream &F) {
    string bufferString;
    getline(F,bufferString);
}

int Station::counter = 0;

int Railway::clock = 0;

int Van::counter = 0;

Van::Van(int numberOfResource) {
    type = UNKNOWN_TYPE;
    maximumLoad.setAmount(100);
    if (numberOfResource < maximumLoad) {
        currentLoad.setAmount(numberOfResource);
        load = IS_NOT_LOADED;
    } else {
        currentLoad = maximumLoad;
        load = IS_LOADED;
        if (numberOfResource > maximumLoad) {
            cout << "Error." << endl;
            //выбросить ошибку(?), мол вагон загружен, но количество ресурса утеряно
        }
    }
    counter++;
    number = counter;
}

int Van::loading(int numberOfResource) {
    if (vanIsLoaded()) {
        cout << "Error. Van is overloaded." << endl;
        return numberOfResource;
        //throw что-то
    } else {
        if (numberOfResource < maximumLoad - currentLoad) {
            currentLoad += numberOfResource;
            load = IS_NOT_LOADED;
            cout << numberOfResource << " units are loaded into the van. There is still free space." << endl;
        } else {
            currentLoad = maximumLoad;
            load = IS_LOADED;
            if (numberOfResource > maximumLoad - currentLoad) {
                cout << "Error. Van is overloaded.";
                cout << "Only " << numberOfResource - maximumLoad << " units are loaded into the van."
                     << endl; //-----------------------------------------ПЕРЕДЕЛАТЬ
                return numberOfResource - maximumLoad;
                //выбросить ошибку(?),
            }
            cout << numberOfResource << " units are loaded into the van. Van is loaded." << endl;
        }
        return 0;
    }
}

void Van::unloading(int numberOfResource) {
    if (currentLoad < numberOfResource) {
        //throw 123;
        cout << "Error: There are not enough resources. ";
        cout << currentLoad.getAmount() << " units are uploaded." << endl;
        currentLoad.setAmount(0);
    } else {
        currentLoad -= numberOfResource;
        cout << numberOfResource << " units are uploaded from the van number " << getNumber() << "." << endl;
    }
    load = IS_NOT_LOADED;
}

bool Van::vanIsLoaded() {
    return load;
}

short int Van::getTypeOfVan() {
    return type;
}

int Van::getNumber() {
    return number;
}

Van::Van() {
    type = UNKNOWN_TYPE;
    currentLoad.setAmount(0);
    maximumLoad.setAmount(0);
    load = IS_NOT_LOADED;
    counter++;
    number = counter;
}

double Van::getLoadCoefficient() {
    return double(currentLoad.getAmount()) / maximumLoad.getAmount();
}

int PassengerVan::loading(int numberOfPersons) {
    if (vanIsLoaded()) {
        cout << "Error. Van is overloaded." << endl;
        return numberOfPersons;
        //throw что-то
    } else {
        if (numberOfPersons < maximumLoad - currentLoad) {
            currentLoad += numberOfPersons;
            load = IS_NOT_LOADED;
            cout << numberOfPersons << " persons got on the van number" << getNumber()
                 << ". There is still available seats." << endl;
        } else {
            currentLoad = maximumLoad;
            load = IS_LOADED;
            if (numberOfPersons > maximumLoad - currentLoad) {
                cout << "Error. Van is overloaded.";
                cout << "Only " << numberOfPersons - maximumLoad << " persons got on the van number" << getNumber()
                     << "." << endl;
                return numberOfPersons - maximumLoad;
                //выбросить ошибку(?),
            }
        }
        return 0;
    }
}

void PassengerVan::unloading(int numberOfPersons) {
    if (currentLoad < numberOfPersons) {
        //throw 123;
        cout << "Error: There are not enough persons at the van number " << getNumber() << "." << endl;
        cout << currentLoad.getAmount() << " persons got off the train." << endl;
        currentLoad.setAmount(0);
    } else {
        currentLoad -= numberOfPersons;
        cout << numberOfPersons << " persons got off the van number " << getNumber() << "." << endl;
    }
    load = IS_NOT_LOADED;
}

PassengerVan::PassengerVan(int numberOfPersons) {
    type = PASSENGER;
    maximumLoad.setAmount(40);
    Van::loading(numberOfPersons);
}

PassengerVan::PassengerVan() {
    type = PASSENGER;
    maximumLoad.setAmount(40);
}

int FreightVan::loading(int numberOfGoods) {
    if (vanIsLoaded()) {
        cout << "Error. Van is overloaded." << endl;
        return numberOfGoods;
        //throw что-то
    } else {
        if (numberOfGoods < maximumLoad - currentLoad) {
            currentLoad += numberOfGoods;
            load = numberOfGoods;
            cout << numberOfGoods << " goods are loaded into the van number" << getNumber()
                 << ". There is still free space." << endl;
        } else {
            currentLoad = maximumLoad;
            load = IS_LOADED;
            if (numberOfGoods > maximumLoad - currentLoad) {
                cout << "Error. Van is overloaded.";
                cout << "Only " << numberOfGoods - maximumLoad << " goods are loaded into the van  number"
                     << getNumber() << "." << endl;
                return numberOfGoods - maximumLoad;
                //выбросить ошибку(?),
            }
        }
        return 0;
    }
}

void FreightVan::unloading(int numberOfGoods) {
    if (currentLoad < numberOfGoods) {
        //throw 123;
        cout << "Error: There are not enough goods at the van number " << getNumber() << "." << endl;
        cout << currentLoad.getAmount() << " persons got off the train." << endl;
        currentLoad.setAmount(0);
    } else {
        currentLoad -= numberOfGoods;
        cout << numberOfGoods << " goods are uploaded from the van number " << getNumber() << "." << endl;
    }
    load = IS_NOT_LOADED;
}

FreightVan::FreightVan() {
    type = FREIGHT;
    maximumLoad.setAmount(100);
}

FreightVan::FreightVan(int numberOfGoods) {
    type = FREIGHT;
    maximumLoad.setAmount(100);
    Van::loading(numberOfGoods);
}

int Train::loadingOfPassengerVan(int number, int numberOfPersons) {
    for (auto &van : Train::listOfVans) {
        if (van->getTypeOfVan() == PASSENGER && van->getNumber() == number) {
            return van->loading(numberOfPersons);
        }
    }
    //throw 123; //нет такого вагона
    return numberOfPersons;
}

int Train::loadingOfFreightVan(int number, int numberOfGoods) {
    for (auto &van : Train::listOfVans) {
        if (van->getTypeOfVan() == FREIGHT && van->getNumber() == number) {
            return van->loading(numberOfGoods);
        }
    }
    //throw 123; //нет такого вагона
    return numberOfGoods;
}


void Train::unloadingOfPassengerVan(int number, int numberOfPersons) {
    for (auto &van : Train::listOfVans) {
        if (van->getTypeOfVan() == PASSENGER && van->getNumber() == number) {
            van->unloading(numberOfPersons);
        }
    }
    //throw 123; //нет такого вагона
}

void Train::unloadingOfFreightVan(int number, int numberOfGoods) {
    for (auto &van : Train::listOfVans) {
        if (van->getTypeOfVan() == FREIGHT && van->getNumber() == number) {
            van->unloading(numberOfGoods);
        }
    }
    //throw 123; //нет такого вагона
}

void Train::inputListOfVansFromString(string &inputString) {
    for (int i = 0; i < inputString.length(); i++) {
        switch (int(inputString[i]) - 48) {
            case PASSENGER: {
                Train::listOfVans.push_back(new PassengerVan);
                break;
            }
            case FREIGHT: {
                Train::listOfVans.push_back(new FreightVan);
                break;
            }
            default: {
                cout << "Error." << endl;
                return;
            }
        }
        i++; //пробел, если это не конец
    }
}

Train::Train(string &name, int &locomotive, string &listOfVans, string &route) {
    Train::name = name;
    Train::inputListOfVansFromString(listOfVans);
    Train::route.inputFromString(route);
    Train::locomotive = Locomotive(locomotive);
    Train::updateTractionForceOfLocomotive();
    Train::calculateSpeed();
    Train::status = STANDING;
    currentDepartureStation = Train::route.getThePointOfDeparture();
}

void Train::calculateSpeed() {
    updateTractionForceOfLocomotive();
    Train::speed = Train::locomotive.getInitialSpeed() - int(0.25 * Train::locomotive.getTractionForce());
}

void Train::updateTractionForceOfLocomotive() {
    Train::locomotive.calculateTractionForce(listOfVans);
}

vector<Station *> *Train::getRoute() {
    return route.getListOfStops();
}

int Train::getTimeBeforeArrivalOrDeparture() {
    return timeBeforeArrivalOrDeparture;
}

int Train::getStatus() {
    return status;
}

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
    //throw 13; нет такой станции
    return nullptr;
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
            cout << "Error." << endl;
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

void Locomotive::calculateTractionForce(vector<Van *> &vans) {
    Locomotive::tractionForce = 0;
    for (auto &van : vans) {
        int force;
        if (van->getTypeOfVan() == 0 || van->getTypeOfVan() == 1) {
            force = 10;
        } else {
            force = 10 * van->getTypeOfVan();
        }
        Locomotive::tractionForce += int(van->getLoadCoefficient() * force);
    }
}

void Locomotive::calculateInitialSpeed() {
    if (age != 0) {
        initialSpeed = 100 / age;
    } else {
        //trow
        initialSpeed = 10;
    }
}

Locomotive::Locomotive() {
    age = 0;
    initialSpeed = 10;
    tractionForce = 1;
}

Locomotive::Locomotive(int age) {
    Locomotive::age = age;
    Locomotive::tractionForce = 1;
    Locomotive::calculateInitialSpeed();
}

int Locomotive::getInitialSpeed() {
    return initialSpeed;
}

int Locomotive::getTractionForce() {
    return tractionForce;
}

void Locomotive::setAge(int newAge) {
    Locomotive::age = newAge;
    Locomotive::calculateInitialSpeed();
}

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
    inputListOfTrainsFromFile(path);
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

Resource::Resource(int type, int amount) {
    Resource::type = type;
    Resource::amount = amount;
}

int Resource::getType() {
    return type;
}

int Resource::getAmount() {
    return amount;
}

void Resource::setAmount(int newAmount) {
    Resource::amount = newAmount;
}

Resource::Resource() {
    type = UNKNOWN_TYPE;
    amount = 0;
}

Resource &Resource::operator+=(int right) {
    this->setAmount(this->getAmount() + right);
    return *this;
}

Resource &Resource::operator-=(int right) {
    this->setAmount(this->getAmount() - right);
    return *this;
}

int Resource::operator-(Resource &right) {
    /*if (this->type != resource.type){
        throw 123;
    }*/
    return this->getAmount() - right.getAmount();
}

int Resource::operator-(int right) {
    return right - this->getAmount();
}

bool Resource::operator==(Resource &right) {
    return (this->amount == right.amount && this->type == right.type);
}

bool Resource::operator!=(Resource &right) {
    return (this->amount != right.amount || this->type != right.type);
}

bool Resource::operator>(Resource &right) {
    /*if (this->type != resource.type){
        throw 123;
    }*/
    return this->amount > right.amount;
}

bool Resource::operator<(Resource &right) {
    /*if (this->type != resource.type){
        throw 123;
    }*/
    return this->amount < right.amount;
}

bool Resource::operator<(int right) {
    return this->getAmount() < right;
}

bool Resource::operator>(int right) {
    return this->getAmount() > right;
}

int operator-(int left, Resource &right) {
    return left - right.getAmount();
}

bool operator<(int left, Resource &right) {
    return left < right.getAmount();
}

bool operator>(int left, Resource &right) {
    return left > right.getAmount();
}

void Route::addStation(Station *station) {
    listOfStops.push_back(station);
}

void Route::inputFromString(string &inputString) {
    string bufferName;
    for (int i = 0; i < inputString.size(); i++) {
        while (inputString[i] != ' ' && i < inputString.size()) {
            bufferName.push_back(inputString[i]);
            i++;
        }
        addStation(new Station(bufferName));
        bufferName.clear();
    }
}

vector<Station *> *Route::getListOfStops() {
    return &listOfStops;
}

Station *Route::getThePointOfDeparture() {
    return listOfStops[0];
}

void Train::moveAlongTheRoute() {
    currentDepartureStation++; //-----------------------------------------------------------------------ПРОВЕРИТЬ РАБОТУ
}

void Train::isArrived() {
    moveAlongTheRoute();
    cout << "The \"" << name << "\" train is arrived at \"" << currentDepartureStation << "\" station."
         << endl;//-----------------------------------------------COUT?
}

void Train::departedFrom() {
    cout << "The \"" << name << "\" train departed from \"" << currentDepartureStation << "\" station."
         << endl;//-----------------------------------------------COUT?
    Train::status = ON_THE_WAY;
    //если в расписании закончились станции, поезд благополучно доехал до последней, то статус меняется на "маршрут пройден" ------------------------ПРОВЕРКА НА ВОТ ЭТО ВОТ
}

void Train::simplyExist() {
    timeBeforeArrivalOrDeparture--;
}

Station *Train::getArrivalStation() {
    Station *nextDepartureStation = currentDepartureStation;
    return nextDepartureStation++; //-----------------------------------------ПРОВЕРИТЬ, НЕ СМЕЩАЕТСЯ ЛИ ТЕКУЩАЯ СТАНЦИЯ
}

Station *Train::getDepartureStation() {
    return currentDepartureStation;
}

void Train::removeFromRoute() {
    status = REMOVED_FROM_THE_ROUTE;
}

void Train::setTravelTime(int time) {
    timeBeforeArrivalOrDeparture = time;
}

int Train::getSpeed() {
    return speed;
}

void Train::passTheRoute() {
    status = PASSED_THE_ROUTE;
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
