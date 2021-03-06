//
// Created by HP on 01.12.2020.
//

#ifndef RAILWAY_EXCEPTION_H
#define RAILWAY_EXCEPTION_H

#include <string>
#include <utility>

using namespace std;

class Exception : public exception {
private:
    string error;
public:
    explicit Exception(string error) {
        Exception::error = std::move(error);
    }

    string &getError() {
        return error;
    }
};

#endif //RAILWAY_EXCEPTION_H
