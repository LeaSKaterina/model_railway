//
// Created by HP on 23.11.2020.
//

#ifndef RAILWAY_RESOURCE_H
#define RAILWAY_RESOURCE_H

#include "exception.h"

class RecourseException : public Exception{
public:
    explicit RecourseException(string error) : Exception(move(error)){};
};

enum KindOfStationOrVanOrResources {
    UNKNOWN_TYPE,
    PASSENGER,
    FREIGHT,
    PASSENGER_AND_FREIGHT
};

namespace rw {

    class Resource {
    private:
        int type;
        int amount;
    public:
        explicit Resource();

        explicit Resource(int type, int amount);

        int getType();

        int getAmount();

        void setAmount(int amount);

        void restock(int additionalAmount);

        void reduce(int quantity);

        Resource &operator+=(int right);

        Resource &operator-=(int right);

        bool operator!=(Resource &right);

        bool operator==(Resource &right);

        bool operator>(Resource &right);

        bool operator<(Resource &right);

        bool operator<(int right);

        bool operator>(int right);

        int operator-(Resource &right);

        int operator-(int right);

    };

    int operator-(int left, Resource &right);

    bool operator<(int left, Resource &right);

    bool operator>(int left, Resource &right);

}


#endif //RAILWAY_RESOURCE_H
