//
// Created by HP on 12.11.2020.
//

#include "railway.h"
using namespace rw;

#include <iostream>
#include <fstream>
//#include <utility>

int Station::counter = 0;

int Railway::clock = 0;

int Van::counter = 0;

Van::Van(int numberOfResource) {
    type = UNKNOWN_TYPE;
    maximumLoad = 100;
    if (numberOfResource < maximumLoad){
        currentLoad = numberOfResource;
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
                cout<<"Only "<<numberOfResource - maximumLoad <<" units are loaded into the van."<<endl;
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
        cout<<currentLoad<<" units are uploaded."<<endl;
        currentLoad = 0;
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
    currentLoad = 0;
    maximumLoad = 0;
    load = IS_NOT_LOADED;
    counter++;
    number = counter;
}

double Van::getLoadCoefficient() {
    return double(currentLoad)/maximumLoad;
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
        cout<<currentLoad<<" persons got off the train."<<endl;
        currentLoad = 0;
    } else{
        currentLoad -= numberOfPersons;
        cout<<numberOfPersons<<" persons got off the van number "<<getNumber()<<"."<<endl;
    }
    load = IS_NOT_LOADED;
}

PassengerVan::PassengerVan(int numberOfPersons){
    type = PASSENGER;
    maximumLoad = 40;
    Van::loading(numberOfPersons);
}

PassengerVan::PassengerVan() {
    type = PASSENGER;
    maximumLoad = 40;
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
        cout<<currentLoad<<" persons got off the train."<<endl;
        currentLoad = 0;
    } else{
        currentLoad -= numberOfGoods;
        cout<<numberOfGoods<<" goods are uploaded from the van number "<<getNumber()<<"."<<endl;
    }
    load = IS_NOT_LOADED;
}

FreightVan::FreightVan() {
    type = FREIGHT;
    maximumLoad = 100;
}

FreightVan::FreightVan(int numberOfGoods){
    type = FREIGHT;
    maximumLoad = 100;
    Van::loading(numberOfGoods);
}

int Train::loadingOfPassengerVans(int numberOfPersons) {
    for (auto & van : Train::listOfVans){
        if (van->getTypeOfVan() == PASSENGER) {
            numberOfPersons = van->loading(numberOfPersons);
            if (!numberOfPersons) {
                cout<<"All of people successfully got on the train."<<endl;
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
    Van *bufferVan;
    for (int i=0; i<inputString.length();i++){
        switch (int(inputString[i])-48){
            case PASSENGER:{
                bufferVan = new PassengerVan;
                Train::listOfVans.push_back(bufferVan);
                break;
            }
            case FREIGHT:{
                bufferVan = new FreightVan;
                Train::listOfVans.push_back(bufferVan);
                break;
            }
            default:{
                cout<<"Error."<<endl;
                return;
            }
        }
        bufferVan++;
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
}

void Train::calculateSpeed() {
    Train::speed = Train::locomotive.getInitialSpeed() - int(0.25*Train::locomotive.getTractionForce());
}

void Train::updateTractionForceOfLocomotive() {
    Train::locomotive.calculateTractionForce(listOfVans);
}

void Train::updateTheParametersOfTheTrain() {
    //--------------------------------------------------------------------------------------------------------------------------ПЕРЕДЕЛАТЬ
    updateTractionForceOfLocomotive();
    calculateSpeed();
    //updateSchedule  ??????
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

string Station::getName() {
    return Station::name;
}

Station::Station(string name, int numberOfResource) {
    Station::name = move(name);
    Station::type = UNKNOWN_TYPE;
    Station::resources.emplace_back(0,numberOfResource);
    Station::id = Station::counter;
    Station::counter++;
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
    Station *bufferStation{};
        for (int i=0; i<amountOfStations; i++){
            F>>bufferName>>bufferType;
            switch (bufferType){
            case UNKNOWN_TYPE:{
                bufferStation = new Station(bufferName);
                Map::listOfStations.push_back(bufferStation);
                break;
            }
            case PASSENGER:{
                int bufferNumberOfPassengers;
                F >> bufferNumberOfPassengers;
                bufferStation = new PassengerStation(bufferName, bufferNumberOfPassengers);
                Map::listOfStations.push_back(bufferStation);
                break;
            }
            case FREIGHT:{
                int bufferNumberOfGoods;
                F>>bufferNumberOfGoods;
                bufferStation = new FreightStation(bufferName, bufferNumberOfGoods);
                Map::listOfStations.push_back(bufferStation);
                break;
            }
            case PASSENGER_AND_FREIGHT:{
                int bufferNumberOfPassengers;
                F >> bufferNumberOfPassengers;
                int bufferNumberOfGoods;
                F>>bufferNumberOfGoods;
                bufferStation = new PassengerAndFreightStation(bufferName, bufferNumberOfPassengers, bufferNumberOfGoods);
                Map::listOfStations.push_back(bufferStation);
                break;
            }
            default:{
                cout<<"Error."<<endl;
            }
        }
        bufferName.clear();
        bufferType = UNKNOWN_TYPE;
        bufferStation++;
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

/*

//НУЖНА ПРОВЕРКА НА КОРРЕКТНОСТЬ ВВЕДЕННОЙ СТРОКИ
void Schedule::inputFromString( string &inputString) {
    string bufferName;
    for (int i=0; i < inputString.size();i++) {
        while (inputString[i] != ' ') {
            bufferName.push_back(inputString[i]);
            if (i == inputString.size() - 1){ //ПЕРЕДЕЛАТЬ ОШИБКИ
                cout<<"Error."<<endl;
                return;
            } else{
                i++;
            }
        }
        i++; //проходим пробел
        Schedule::stops.emplace_back(bufferName,int(inputString[i]) - 48); //создает объект непосредственно в конце вектора, т.е. без лишнего копирования (или перемещения)
        i++; //еще один пробел, если это не конец
        bufferName.clear();
    }
}

Stop::Stop(string nameOfStation, short int action) {
    Stop::nameOfStation = move(nameOfStation);
    Stop::action = action;
}

short int Stop::getAction() {
    return action;
}

void Stop::setAction(short int newAction) {
    action = newAction;
}

string *Stop::getNameOfStation() {
    return &nameOfStation;
}

void Stop::setNameOfStation(string newNameOfStation) {
    nameOfStation = move(newNameOfStation);
}
*/

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
        //string bufferString;
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

void Route::addStation(Station *station) {
    listOfStations.push_back(station);
}


/*
    string bufferName;
    for (int i=0; i < inputString.size();i++) {
        while (inputString[i] != ' ') {
            bufferName.push_back(inputString[i]);
            if (i == inputString.size() - 1){ //ПЕРЕДЕЛАТЬ ОШИБКИ
                cout<<"Error."<<endl;
                return;
            } else{
                i++;
            }
        }
        i++; //проходим пробел
        addStation(Station(bufferName)); //создает объект непосредственно в конце вектора, т.е. без лишнего копирования (или перемещения)
        i++; //еще один пробел, если это не конец
        bufferName.clear();
}*/
void Route::inputFromString(string &inputString) {
    //--------------------------------------------------------------------------------------------------------------------------РЕАЛИЗОВАТЬ
}
