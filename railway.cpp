//
// Created by HP on 12.11.2020.
//

#include "railway.h"
using namespace rw;

#include <iostream>
#include <fstream>
#include <ctime>
//#include <utility>

int Station::counter = 0;

int Railway::clock = 0;

int Van::counter = 0;

Van::Van(int numberOfResource) {
    type = UNKNOWN_TYPE;
    maximumLoad.setAmount(100);
    if (numberOfResource < maximumLoad){
        currentLoad.setAmount(numberOfResource);
        load = IS_NOT_LOADED;
    } else{
        currentLoad = maximumLoad;
        load = IS_LOADED;
        if (numberOfResource > maximumLoad){
            cout<<"Error."<<endl;
            //выбросить ошибку(?), мол вагон загружен, но количество ресурса утеряно
        }
    }
    counter++;
    number = counter;
}

int Van::loading(int numberOfResource) {
    if (vanIsLoaded()) {
        cout<<"Error. Van is overloaded."<<endl;
        return  numberOfResource;
        //throw что-то
    } else {
        if (numberOfResource < maximumLoad - currentLoad) {
            currentLoad += numberOfResource;
            load = IS_NOT_LOADED;
            cout<<numberOfResource<<" units are loaded into the van. There is still free space."<<endl;
        } else {
            currentLoad = maximumLoad;
            load = IS_LOADED;
            if (numberOfResource > maximumLoad - currentLoad) {
                cout<<"Error. Van is overloaded.";
                cout<<"Only "<<numberOfResource - maximumLoad <<" units are loaded into the van."<<endl; //-----------------------------------------ПЕРЕДЕЛАТЬ
                return numberOfResource - maximumLoad;
                //выбросить ошибку(?),
            }
            cout<<numberOfResource<<" units are loaded into the van. Van is loaded."<<endl;
        }
        return 0;
    }
}

void Van::uploading(int numberOfResource) {
    if (currentLoad < numberOfResource){
        //throw 123;
        cout<<"Error: There are not enough resources. ";
        cout<<currentLoad.getAmount()<<" units are uploaded."<<endl;
        currentLoad.setAmount(0);
    } else{
        currentLoad -= numberOfResource;
        cout<<numberOfResource<<" units are uploaded from the van number "<<getNumber()<<"."<<endl;
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
    return double(currentLoad.getAmount())/maximumLoad.getAmount();
}

int PassengerVan::loading(int numberOfPersons) {
    if (vanIsLoaded()) {
        cout<<"Error. Van is overloaded."<<endl;
        return  numberOfPersons;
        //throw что-то
    } else {
        if (numberOfPersons < maximumLoad - currentLoad) {
            currentLoad += numberOfPersons;
            load = IS_NOT_LOADED;
            cout<<numberOfPersons<<" persons got on the van number" <<getNumber() <<". There is still available seats."<<endl;
        } else {
            currentLoad = maximumLoad;
            load = IS_LOADED;
            if (numberOfPersons > maximumLoad - currentLoad) {
                cout<<"Error. Van is overloaded.";
                cout<<"Only "<<numberOfPersons - maximumLoad <<" persons got on the van number" <<getNumber() <<"."<<endl;
                return numberOfPersons - maximumLoad;
                //выбросить ошибку(?),
            }
        }
        return 0;
    }
}

void PassengerVan::uploading(int numberOfPersons) {
    if (currentLoad < numberOfPersons){
        //throw 123;
        cout<<"Error: There are not enough persons at the van number "<<getNumber()<<"."<<endl;
        cout<<currentLoad.getAmount()<<" persons got off the train."<<endl;
        currentLoad.setAmount(0);
    } else{
        currentLoad -= numberOfPersons;
        cout<<numberOfPersons<<" persons got off the van number "<<getNumber()<<"."<<endl;
    }
    load = IS_NOT_LOADED;
}

PassengerVan::PassengerVan(int numberOfPersons){
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
        cout<<"Error. Van is overloaded."<<endl;
        return  numberOfGoods;
        //throw что-то
    } else {
        if (numberOfGoods < maximumLoad - currentLoad) {
            currentLoad += numberOfGoods;
            load = numberOfGoods;
            cout<<numberOfGoods<<" goods are loaded into the van number" <<getNumber() <<". There is still free space."<<endl;
        } else {
            currentLoad = maximumLoad;
            load = IS_LOADED;
            if (numberOfGoods > maximumLoad - currentLoad) {
                cout<<"Error. Van is overloaded.";
                cout<<"Only "<<numberOfGoods - maximumLoad <<" goods are loaded into the van  number"<<getNumber()<<"."<<endl;
                return numberOfGoods - maximumLoad;
                //выбросить ошибку(?),
            }
        }
        return 0;
    }
}

void FreightVan::uploading(int numberOfGoods) {
    if (currentLoad < numberOfGoods){
        //throw 123;
        cout<<"Error: There are not enough goods at the van number "<<getNumber()<<"."<<endl;
        cout<<currentLoad.getAmount()<<" persons got off the train."<<endl;
        currentLoad.setAmount(0);
    } else{
        currentLoad -= numberOfGoods;
        cout<<numberOfGoods<<" goods are uploaded from the van number "<<getNumber()<<"."<<endl;
    }
    load = IS_NOT_LOADED;
}

FreightVan::FreightVan() {
    type = FREIGHT;
    maximumLoad.setAmount(100);
}

FreightVan::FreightVan(int numberOfGoods){
    type = FREIGHT;
    maximumLoad.setAmount(100);
    Van::loading(numberOfGoods);
}

int Train::loadingOfPassengerVans(int numberOfPersons) {
    for (auto & van : Train::listOfVans){
        if (van->getTypeOfVan() == PASSENGER) {
            numberOfPersons = van->loading(numberOfPersons);
            if (!numberOfPersons) {
                cout<<"All of people successfully got on the train."<<endl; //--------------------------------------------------------эта надпись должна быть по окончанию загрузки, а не в начале
                return 0;
            }
        }
    }
    cout <<"There weren't enough seats for everybody, so "<<numberOfPersons<<" persons had to stay."<<endl;
    //throw
    return numberOfPersons;
}

int Train::loadingOfFreightVans(int numberOfGoods) {
    for (auto & van : Train::listOfVans){
        if (van->getTypeOfVan() == FREIGHT) {
            numberOfGoods = van->loading(numberOfGoods);
            if (!numberOfGoods){
                cout<<"All of goods are loaded on the train successfully."<<endl;
                return 0;
            }
        }
    }
    cout <<"There wasn't enough space for all, so "<<numberOfGoods<<" goods is left at the station."<<endl;
    //throw
    return numberOfGoods;
}


void Train::uploadingOfPassengerVans(int numberOfPersons) {
    //--------------------------------------------------------------------------------------------------------------------------ПЕРЕДЕЛАТЬ
    for (auto & van : Train::listOfVans){
        if (van->getTypeOfVan() == PASSENGER)
            van->uploading(numberOfPersons);
    }
}

void Train::uploadingOfFreightVans(int numberOfGoods) {
    //--------------------------------------------------------------------------------------------------------------------------ПЕРЕДЕЛАТЬ
    for (auto & van : Train::listOfVans){
        if (van->getTypeOfVan() == FREIGHT)
            van->uploading(numberOfGoods);
    }
}

void Train::inputListOfVansFromString(string &inputString) {
    for (int i=0; i<inputString.length();i++){
        switch (int(inputString[i])-48){
            case PASSENGER:{
                Train::listOfVans.push_back(new PassengerVan);
                break;
            }
            case FREIGHT:{
                Train::listOfVans.push_back(new FreightVan);
                break;
            }
            default:{
                cout<<"Error."<<endl;
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
    Train::speed = Train::locomotive.getInitialSpeed() - int(0.25*Train::locomotive.getTractionForce());
}

void Train::updateTractionForceOfLocomotive() {
    Train::locomotive.calculateTractionForce(listOfVans);
}

vector<Station*>* Train::getRoute() {
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
    Station::resources.emplace_back(0,numberOfResource);
    Station::id = Station::counter;
    Station::counter++;
}

void Station::deleteTheStation(Station *station) {
    delete station;
    counter--;
}

PassengerStation::PassengerStation(string name, int numberOfPassengers) : Station(move(name)) {
    type = PASSENGER;
    resources.emplace_back(Resource(PASSENGER,numberOfPassengers));
}

FreightStation::FreightStation(string name, int numberOfGoods) : Station(move(name)) {
    type = FREIGHT;
    resources.emplace_back(Resource(FREIGHT,numberOfGoods));
}

PassengerAndFreightStation::PassengerAndFreightStation(string name, int numberOfPassengers, int numberOfGoods) : Station(move(name)) {
    type = PASSENGER_AND_FREIGHT;
    resources.emplace_back(Resource(PASSENGER, numberOfPassengers));
    resources.emplace_back(Resource(FREIGHT,numberOfGoods));
}

void Map::inputStationsFromFile(const char *path) {
    ifstream F;
    F.open(path);
    if (!F.is_open()) {
        cout<<"Error"<<endl;
        return;
    }
    short int amountOfStations;
    F>>amountOfStations;
    short int bufferType;
    string bufferName;
        for (int i=0; i<amountOfStations; i++){
            F>>bufferName>>bufferType;
            switch (bufferType){
            case UNKNOWN_TYPE:{
                Map::listOfStations.push_back(new Station(bufferName));
                break;
            }
            case PASSENGER:{
                int bufferNumberOfPassengers;
                F >> bufferNumberOfPassengers;
                Map::listOfStations.push_back(new PassengerStation(bufferName, bufferNumberOfPassengers));
                break;
            }
            case FREIGHT:{
                int bufferNumberOfGoods;
                F>>bufferNumberOfGoods;
                Map::listOfStations.push_back(new FreightStation(bufferName, bufferNumberOfGoods));
                break;
            }
            case PASSENGER_AND_FREIGHT:{
                int bufferNumberOfPassengers;
                F >> bufferNumberOfPassengers;
                int bufferNumberOfGoods;
                F>>bufferNumberOfGoods;
                Map::listOfStations.push_back(new PassengerAndFreightStation(bufferName, bufferNumberOfPassengers, bufferNumberOfGoods));
                break;
            }
            default:{
                cout<<"Error."<<endl;
            }
        }
        bufferName.clear();
        bufferType = UNKNOWN_TYPE;
    }
    F.close();
}

void Map::inputPathsFromFile(const char *path) {
    ifstream F;
    F.open(path);
    if (!F.is_open()) {
        cout<<"Error"<<endl;
        return;
    }
    short int amountOfStations;
    F>>amountOfStations;
    string bufferString;
    int buffer;
    for (int i=0; i<=amountOfStations; i++){
        getline(F,bufferString);
    }
    for (int i=0; i<amountOfStations; i++){
        vector<int> bufferVector;
        for (int j=0; j<amountOfStations; j++){
            F>>buffer;
            bufferVector.push_back(buffer);
        }
        Map::listOfPaths.push_back(bufferVector);
    }
    F.close();
}

void Map::inputMapFromFile(const char *path) {
    Map::inputStationsFromFile(path);
    Map::inputPathsFromFile(path);
}

Station *Map::getStation(string &nameOfStation) {
    for (auto & station : listOfStations){
        if (station->getName() == nameOfStation){
            return station;
        }
    }
    //throw 13; нет такой станции
    return nullptr;
}


void Locomotive::calculateTractionForce(vector<Van*> &vans) {
    Locomotive::tractionForce = 0;
    for (auto & van : vans){
        int force;
        if (van->getTypeOfVan() == 0 || van->getTypeOfVan() == 1){
            force = 10;
        } else{
            force = 10*van->getTypeOfVan();
        }
        Locomotive::tractionForce += int(van->getLoadCoefficient()*force);
    }
}

void Locomotive::calculateInitialSpeed() {
    if (age != 0) {
        initialSpeed = 100 / age;
    } else{
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
    if (!F.is_open()){
        cout<<"Error"<<endl;
        return;
    }
    string bufferString;
    {
        short int bufferInt;
        F >> bufferInt;
        for (int i = 0; i <= bufferInt * 2; i++) {
            getline(F, bufferString);
        }
    }
    int amountOfTrains;
    F>>amountOfTrains;
    getline(F,bufferString);//спускаемся со строки с количеством поездов
    string bufferName;
    int bufferLocomotiveAge;
    string bufferVans;
    string bufferRoute;
    for (int i=0; i<amountOfTrains; i++){
        getline(F, bufferName);
        F >> bufferLocomotiveAge;
        getline(F,bufferString);//спускаемся со строки с возрастом
        getline(F, bufferVans);
        getline(F, bufferRoute);
        Railway::listOfTrains.emplace_back(bufferName, bufferLocomotiveAge, bufferVans, bufferRoute);
    }
}

void Railway::putTrainsOnTheMap() {//--------------------------------------------------------------------------------------ПРОВЕРИТЬ, МЕНЯЕТСЯ ЛИ МАРШРУТ (РАБОТА С ССЫЛКАМИ)
    vector<Station*> *route{};
    Station* buffer{};
    for ( auto &train : listOfTrains){
        route = train.getRoute();
        for (auto &stop : *route){
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
    for (auto &train : listOfTrains){
        if (train.getStatus() != REMOVED_FROM_THE_ROUTE && train.getStatus() != PASSED_THE_ROUTE) {
            if (train.getTimeBeforeArrivalOrDeparture() == 0) {
                switch (train.getStatus()) {
                    case ON_THE_WAY:{
                        train.isArrived();
                        //каким-то образом определяется действие, которое нужно совершить на станции
                        /* Смена статуса:
                         * при транзите не меняем cтатус. не забыть, что он как бы прибыл. но проехал мимо. расчитываем время на путь до следующей станции
                         * в остальных случаях: статус, выбранный пользователем || статус, который генерируется рандомайзером
                         */
                            //транзит - меняем станцию отправления, высчитываем "время до прибытия" (не меняя скорость), меняем статус на "в пути"
                            //стоянка - статус меняем на "стоит", по истечению соответствующего количества времени высчитываем новое "время до прибытия" и статус меняем на "в пути"
                            //разгрузка/загрузка - статус меняем, по истечению соответствующего количества времени высчитываем новую скорость,

                        //в зависимости от действия присваивается новое значение для timeBeforeArrivalOrDeparture (либо здесь, либо внутри функций загрузки/разгрузки)
                        //пассажирские вагоны загружаются быстро за фиксированный срок
                        //товарные вагоны загружаются/разгружаются дольше
                        //загрузка и тех, и тех - сумма по времени
                        break;
                    }
                    case STANDING:{
                        train.departedFrom();
                        break;
                    }
                    case IS_BEING_LOADED | IS_BEING_UNLOADED:{
                        train.calculateSpeed();
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

int Railway::getRandomAction() { //--------------------------------------------------------------------------------------------------ПОДУМАТЬ НАД ЭТИМ, МОЖЕТ КАК-ТО ПО-ДРУГОМУ РЕАЛИЗОВАТЬ
    srand ( time(NULL) );
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
    return this->amount > right.amount ;
}

bool Resource::operator<(Resource &right) {
    /*if (this->type != resource.type){
        throw 123;
    }*/
    return this->amount < right.amount ;
}

bool Resource::operator<(int right) {
    return this->getAmount()<right;
}

bool Resource::operator>(int right) {
    return this->getAmount()>right;
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
    currentDepartureStation++; //---------------------------------------------------------------------------------------------------------------------ПРОВЕРИТЬ РАБОТУ
}

void Train::isArrived() {
    moveAlongTheRoute();
    cout<<"The \""<<name<<"\" train is arrived at \""<<currentDepartureStation<<"\" station."<<endl;//-----------------------------------------------COUT?
}

void Train::departedFrom() {
    //здесь нужен расчет времени на дорогу до след.станции (расстояние/скорость) --------------------------------------------------------------------СДЕЛАТЬ
    cout<<"The \""<<name<<"\" train departed from \""<<currentDepartureStation<<"\" station."<<endl;//-----------------------------------------------COUT?
    Train::status = ON_THE_WAY;
    //если в расписании закончились станции, поезд благополучно доехал до последней, то статус меняется на "маршрут пройден" ------------------------ПРОВЕРКА НА ВОТ ЭТО ВОТ
}

void Train::simplyExist() {
    timeBeforeArrivalOrDeparture++;
}
