//
// Created by HP on 23.11.2020.
//

#include "van.h"
#include <iostream>
#include <utility>
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
            throw VanException("The number of resource is too large. Try to load the following number of resource to another van. ",numberOfResource-maximumLoad);
        }
    }
    counter++;
    number = counter;
}

void Van::loading(int numberOfResource) {
    if (Van::isLoaded()) {
        throw VanException("Van is fully loaded, so there's no opportunity to load this amount of resource. ", numberOfResource);
    } else {
        if (numberOfResource < maximumLoad - currentLoad) {
            currentLoad += numberOfResource;
            load = IS_NOT_LOADED;
        } else {
            currentLoad = maximumLoad;
            load = IS_LOADED;
            if (numberOfResource > maximumLoad - currentLoad) {
                throw VanException("Van is fully loaded, so there's no opportunity to load this amount of resource. ", numberOfResource - maximumLoad);
            }
        }
    }
}

void Van::unloading(int numberOfResource) {
    if (currentLoad < numberOfResource) {
        int amountOfUnloadedResource = currentLoad.getAmount();
        currentLoad.setAmount(0);
        throw VanException("There are not enough resources. ", amountOfUnloadedResource);
    } else {
        currentLoad -= numberOfResource;
    }
    load = IS_NOT_LOADED;
}

bool Van::isLoaded() {
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

bool Van::isEmpty() {
    return !currentLoad.getAmount();
}

int Van::getMaximumLoad() {
    return maximumLoad.getAmount();
}

int Van::getCurrentLoad() {
    return currentLoad.getAmount();
}

void PassengerVan::loading(int numberOfPersons) {
    if (Van::isLoaded()) {
        throw VanException("Passenger van is fully loaded, so there's no opportunity to load this amount of people. ", numberOfPersons);
    } else {
        if (numberOfPersons < maximumLoad - currentLoad) {
            currentLoad += numberOfPersons;
            load = IS_NOT_LOADED;
        } else {
            currentLoad = maximumLoad;
            load = IS_LOADED;
            if (numberOfPersons > maximumLoad - currentLoad) {
                throw VanException("Passenger van is fully loaded, so there's no opportunity to load this amount of people. ", numberOfPersons - maximumLoad);
            }
        }
    }
}

void PassengerVan::unloading(int numberOfPersons) {
    if (currentLoad < numberOfPersons) {
        int amountOfUnloadedPeople = currentLoad.getAmount();
        currentLoad.setAmount(0);
        throw VanException("There are not enough persons at this passenger van. ", amountOfUnloadedPeople);
    } else {
        currentLoad -= numberOfPersons;
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

void FreightVan::loading(int numberOfGoods) {
    if (Van::isLoaded()) {
        throw VanException("Freight van is fully loaded, so there's no opportunity to load this amount of goods. ", numberOfGoods);
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
                throw VanException("Freight van is fully loaded, so there's no opportunity to load this amount of goods. ", numberOfGoods - maximumLoad);
            }
        }
    }
}

void FreightVan::unloading(int numberOfGoods) {
    if (currentLoad < numberOfGoods) {
        int amountOfUnloadedGoods = currentLoad.getAmount();
        currentLoad.setAmount(0);
        throw VanException("There are not enough goods at this freight van. ", amountOfUnloadedGoods);
    } else {
        currentLoad -= numberOfGoods;
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

VanException::VanException(string error, int amountOfResource) : Exception(move(error)){
    VanException::amountOfExceptionalResource = amountOfResource;
}

int VanException::getAmountOfExceptionalResource() {
    return amountOfExceptionalResource;
}
