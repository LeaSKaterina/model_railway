//
// Created by HP on 23.11.2020.
//

#include "van.h"
#include <iostream>
using namespace std;
using namespace rw;
using namespace train;

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
