#include "railway.h"

#include <iostream>
using namespace std;

int main() {
    rw::Railway railwayModel;
    railwayModel.inputModelFromFile("Model1.txt");
    return 0;
}
