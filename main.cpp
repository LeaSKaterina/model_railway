#include "classes/railway.h"

using namespace std;

int main() {
    rw::Railway railwayModel("Model2.txt");
    railwayModel.start();
    railwayModel.printParametersOfTrains();
    return 0;
}
