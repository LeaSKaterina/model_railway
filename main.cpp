#include "classes/railway.h"

using namespace std;

int main() {
    rw::Railway railwayModel("Model2.txt");
    railwayModel.run(USER);
    return 0;
}
