//
// Created by HP on 23.11.2020.
//

#ifndef RAILWAY_STATION_H
#define RAILWAY_STATION_H

#include "train.h"
#include <string>
#include <vector>
#include <iostream>
using namespace rw;
using namespace train;
using namespace std;

namespace rw {
    class Station {
    private:
        string name;
        int id;
        static int counter;
    protected:
        vector <Resource> resources{};
        short int type;
    public:
        explicit Station(string name);

        explicit Station(string name, int resource);

        void setName(string newName);

        string &getName();

        static void deleteTheStation(Station *station);

        int getId();

        //virtual void loading(Train *train);
        //virtual void unloading(Train *train);
        //void temporaryStop(Train *train);
        //void transit(Train *train);
    };

    class PassengerStation : public Station {
    public:
        explicit PassengerStation(string name, int numberOfPassengers);
        /* explicit - предотвращает неявное преобразование типов
         (выполняется всякий раз, когда требуется один фундаментальный тип данных,
         но предоставляется другой, и пользователь не указывает компилятору, как выполнить конвертацию )
         рекомендуется явно объявлять конструкторы explicit всегда, кроме случаев, когда неявное преобразование семантически оправдано.
        */
        //void loading(Train *train) override;

        //void unloading(Train *train) override;
    };

    class FreightStation : public Station {
    public:
        explicit FreightStation(string name, int numberOfGoods);

        //void loading(Train *train) override;

        //void unloading(Train *train) override;
    };

    class PassengerAndFreightStation : public Station {
    public:
        explicit PassengerAndFreightStation(string name, int numberOfPassengers, int numberOfGoods);

        //void loading(Train *train) override;

        //void unloading(Train *train) override;
    };

}

#endif //RAILWAY_STATION_H
