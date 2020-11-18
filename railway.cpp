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

Van::Van() {
    type = UNKNOWN_TYPE;
    load = IS_NOT_LOADED;
    counter++;
    number = counter;
}

void Van::loading() {
    load = IS_LOADED;
}

void Van::uploading() {
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

void PassengerVan::loading() {
    if (vanIsLoaded()){
        cout<<"Passenger van number "<< number <<" is already loaded."<<endl;
    } else{
        load = IS_LOADED;
        cout<<"Passenger van number "<< number <<" is loaded successfully."<<endl;
    }
}

void PassengerVan::uploading() {
    if (!vanIsLoaded()){
        cout<<"Passenger van number "<< number <<" is already uploaded."<<endl;
    } else{
        load = IS_NOT_LOADED;
        cout<<"Passenger van number "<< number <<" is uploaded successfully."<<endl;
    }
}

PassengerVan::PassengerVan() {
    type = PASSENGER;
}

void FreightVan::loading() {
    if (vanIsLoaded()){
        cout<<"Freight van number "<< number <<" is already loaded."<<endl;
    } else{
        load = IS_LOADED;
        cout<<"Freight van number "<< number <<" is loaded successfully."<<endl;
    }
}

void FreightVan::uploading() {
    if (!vanIsLoaded()){
        cout<<"Freight van number "<< number <<" is already uploaded."<<endl;
    } else{
        load = IS_NOT_LOADED;
        cout<<"Freight van number "<< number <<" is uploaded successfully."<<endl;
    }
}

FreightVan::FreightVan() {
    type = FREIGHT;
}

void Train::loadingOfPassengerVans() {
    for (auto & listOfVan : Train::listOfVans){
        if (listOfVan->getTypeOfVan() == PASSENGER)
            listOfVan->loading();
    }
}

void Train::loadingOfFreightVans() {
    for (auto & listOfVan : Train::listOfVans){
        if (listOfVan->getTypeOfVan() == FREIGHT)
            listOfVan->loading();
    }
}

void Train::loadingOfAllVans() {
    for (auto & listOfVan : Train::listOfVans){
            listOfVan->loading();
    }
}

void Train::uploadingOfPassengerVans() {
    for (auto & listOfVan : Train::listOfVans){
        if (listOfVan->getTypeOfVan() == PASSENGER)
            listOfVan->uploading();
    }
}

void Train::uploadingOfFreightVans() {
    for (auto & listOfVan : Train::listOfVans){
        if (listOfVan->getTypeOfVan() == FREIGHT)
            listOfVan->uploading();
    }
}

void Train::uploadingOfAllVans() {
    for (auto & listOfVan : Train::listOfVans){
        listOfVan->uploading();
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

Train::Train(string &name, int &locomotive, string &listOfVans, string &schedule) {
    Train::name = name;
    Train::inputListOfVansFromString(listOfVans);
    Train::schedule.inputFromString(schedule);
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

void Station::loading(Train train) {
    train.loadingOfAllVans();
}

void Station::uploading(Train train) {
    train.uploadingOfAllVans();
}

PassengerStation::PassengerStation(string name) : Station(move(name)) {
    type = PASSENGER;
}

void PassengerStation::loading(Train train) {
    train.loadingOfPassengerVans();
}

void PassengerStation::uploading(Train train) {
    train.uploadingOfPassengerVans();
}

FreightStation::FreightStation(string name) : Station(move(name)) {
    type = FREIGHT;
}

void FreightStation::loading(Train train) {
    train.loadingOfFreightVans();
}

void FreightStation::uploading(Train train) {
    train.uploadingOfFreightVans();
}

PassengerAndFreightStation::PassengerAndFreightStation(string name) : Station(move(name)) {
    type = PASSENGER_AND_FREIGHT;
}

void PassengerAndFreightStation::loading(Train train) {
    train.loadingOfAllVans();
}

void PassengerAndFreightStation::uploading(Train train) {
    train.uploadingOfAllVans();
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
    Station *bufferStation = nullptr;
        for (int i=0; i<amountOfStations; i++){
            F>>bufferName>>bufferType;
            switch (bufferType){
            case UNKNOWN_TYPE:{
                bufferStation = new Station(bufferName);
                Map::listOfStations.push_back(bufferStation);
                break;
            }
            case PASSENGER:{
                bufferStation = new PassengerStation(bufferName);
                Map::listOfStations.push_back(bufferStation);
                break;
            }
            case FREIGHT:{
                bufferStation = new FreightStation(bufferName);
                Map::listOfStations.push_back(bufferStation);
                break;
            }
            case PASSENGER_AND_FREIGHT:{
                bufferStation = new PassengerAndFreightStation(bufferName);
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

void Locomotive::calculateTractionForce(vector<Van*> &vans) {
    Locomotive::tractionForce = 0;
    for (auto & van : vans){
        int coefficient;
        if (van->getTypeOfVan() == 0 || van->getTypeOfVan() == 1){
            coefficient = 1;
        } else{
            coefficient = van->getTypeOfVan();
        }
        if (van->vanIsLoaded()) coefficient*=2;
        Locomotive::tractionForce+=coefficient;
    }
}

void Locomotive::calculateInitialSpeed() {
    if (age != 0) {
        initialSpeed = 100 / age;
    } else{
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

/*void Locomotive::attacheTheVans(vector<Van *> &vans) {
    Locomotive::calculateTractionForce(vans);
}*/

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
    /*вместо отдельной переменной количества поездов просто посчитать строки в файле (?)
     while (getline(in, line))
    {
        n++;
    }*/
    int amountOfTrains;
    F>>amountOfTrains;
    getline(F,bufferString);//спускаемся со строки с количеством поездов
    string bufferName;
    int bufferLocomotiveAge;
    string bufferVans;
    string bufferSchedule;
    for (int i=0; i<amountOfTrains; i++){
        getline(F, bufferName);
        F >> bufferLocomotiveAge;
        getline(F,bufferString);//спускаемся со строки с возрастом
        getline(F, bufferVans);
        getline(F, bufferSchedule);
        Railway::listOfTrains.emplace_back(bufferName, bufferLocomotiveAge, bufferVans, bufferSchedule);
    }
}
