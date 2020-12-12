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
        try {
            currentLoad = maximumLoad;
            load = IS_LOADED;
            if (numberOfResource > maximumLoad) {
                throw ResourceException(
                        "The number of resource is too large. The maximum possible amount is loaded.");
            }
        }
        catch (ResourceException &exception) {
            cout << exception.getError() << endl;
        }
    }
    counter++;
    number = counter;
}

int Van::loading(int numberOfResource) {
    if (Van::isLoaded()) {
        return 0;
    }
    if (numberOfResource > maximumLoad - currentLoad) {
        load = IS_LOADED;
        currentLoad = maximumLoad;
        return maximumLoad.getAmount();
    }
    if (numberOfResource == maximumLoad - currentLoad) {
        load = IS_LOADED;
        currentLoad = maximumLoad;
    } else {
        load = IS_NOT_LOADED;
        currentLoad += numberOfResource;
    }
    return numberOfResource;
}

int Van::unloading(int numberOfUnloadedResource) {
    load = IS_NOT_LOADED;
    if (currentLoad < numberOfUnloadedResource) {
        int remainder = numberOfUnloadedResource - currentLoad;
        currentLoad.setAmount(0);
        return remainder;
    } else {
        currentLoad -= numberOfUnloadedResource;
        return 0;
    }
}

bool Van::isLoaded() {
    return load;
}

int Van::getTypeOfVan() {
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

bool Van::isEmpty() {
    return !currentLoad.getAmount();
}

int Van::getMaximumLoad() {
    return maximumLoad.getAmount();
}

int Van::getCurrentLoad() {
    return currentLoad.getAmount();
}

void Van::setTypeOfResources(int newType) {
    currentLoad.setType(newType);
    maximumLoad.setType(newType);
}

int PassengerVan::loading(int numberOfPersons) {
    numberOfPersons = Van::loading(numberOfPersons);
    cout << numberOfPersons << " passengers are loading in the passenger van number " << number << "." << endl;
    return numberOfPersons;
}

int PassengerVan::unloading(int numberOfPersons) {
    int remainder = Van::unloading(numberOfPersons);
    cout << numberOfPersons - remainder << " passengers are unloading from the passenger van number " << number << "."
         << endl;
    return remainder;
}

PassengerVan::PassengerVan(int numberOfPersons) {
    type = PASSENGER;
    setTypeOfResources(PASSENGER);
    maximumLoad.setAmount(40);
    Van::loading(numberOfPersons);
}

PassengerVan::PassengerVan() {
    type = PASSENGER;
    setTypeOfResources(PASSENGER);
    maximumLoad.setAmount(40);
}

int FreightVan::loading(int numberOfGoods) {
    numberOfGoods = Van::loading(numberOfGoods);
    cout << numberOfGoods << " goods are being loaded in the freight van number " << number << "." << endl;
    return numberOfGoods;
}

int FreightVan::unloading(int numberOfGoods) {
    int remainder = Van::unloading(numberOfGoods);
    cout << numberOfGoods - remainder << " goods are being unloaded from the freight van number " << number << "."
         << endl;
    return remainder;
}

FreightVan::FreightVan() {
    type = FREIGHT;
    setTypeOfResources(FREIGHT);
    maximumLoad.setAmount(100);
}

FreightVan::FreightVan(int numberOfGoods) {
    type = FREIGHT;
    setTypeOfResources(FREIGHT);
    maximumLoad.setAmount(100);
    Van::loading(numberOfGoods);
}

