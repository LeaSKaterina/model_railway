//
// Created by HP on 23.11.2020.
//

#include "resource.h"
using namespace rw;

Resource::Resource() {
    type = UNKNOWN_TYPE;
    amount = 0;
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
    return this->getAmount() - right;
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

 int rw::operator-(int left, rw::Resource &right) {
    return left - right.getAmount();
}

 bool rw::operator<(int left, rw::Resource &right) {
    return left < right.getAmount();
}

 bool rw::operator>(int left, rw::Resource &right) {
    return left > right.getAmount();
}
