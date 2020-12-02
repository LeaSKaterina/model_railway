#include "classes/railway.h"

#include <iostream>
using namespace std;

int main() {
    rw::Railway railwayModel("Model1.txt");
    railwayModel.start();
    return 0;
}
