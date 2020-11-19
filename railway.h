//
// Created by HP on 12.11.2020.
//

#ifndef RAILWAY_RAILWAY_H
#define RAILWAY_RAILWAY_H

#include <string>
#include <vector>

using namespace std;

enum StatusOfTheTrain{
    REMOVED_FROM_THE_ROUTE = 0,
    PASSED_THE_ROUTE = 0,
    ON_THE_WAY,
    STANDING,
    IS_BEING_LOADED,
    IS_BEING_UPLOADED
};

enum Action {
    START,
    TRANSIT,
    TEMPORARY_PARKING, // 10 мин
    LOADING,
    UNLOADING,
};

enum Load {
    IS_LOADED = true,
    IS_NOT_LOADED = false
};

enum KindOfStationOrVanOrResources {
    UNKNOWN_TYPE,
    PASSENGER,
    FREIGHT,
    PASSENGER_AND_FREIGHT
};

namespace rw {

    namespace train {

        //у вагона должно быть макс.количество чего-то и текущее количество чего-то
        class Van {
        private:
            static int counter;
        protected:
            int maximumLoad;
            int currentLoad;
            short int type;
            bool load;
            int number;
        public:
            Van();

            explicit Van(int numberOfResource);

            virtual int loading(int numberOfResource);

            virtual void uploading(int numberOfResource);

            bool vanIsLoaded();

            short int getTypeOfVan();

            int getNumber();

            double getLoadCoefficient();
        };

        class PassengerVan : public Van {
        public:
            explicit PassengerVan();

            explicit PassengerVan(int numberOfPersons);

            int loading(int numberOfPersons) override;

            void uploading(int numberOfPersons) override;
        };

        class FreightVan : public Van {
        public:
            explicit FreightVan();

            explicit FreightVan(int numberOfGoods);

            int loading(int numberOfGoods) override;

            void uploading(int numberOfGoods) override;
        };

        class Stop {
        private:
            string nameOfStation;
            short int action;
        public:
            Stop(string nameOfStation, short int action);

            short int getAction();

            void setAction(short int newAction);

            string* getNameOfStation();

            void setNameOfStation(string newNameOfStation);
        };

        class Schedule {
        private:
            vector<Stop> stops;
        public:
            void inputFromString(string &inputString);
        };

        class Locomotive {
        private:
            int tractionForce{};
            int initialSpeed{};
            int age;
            void calculateInitialSpeed();
        public:
            Locomotive();
            explicit Locomotive(int age);
            void calculateTractionForce(vector<Van*> &vans);
            int getInitialSpeed();
            int getTractionForce();
            void setAge(int newAge);
        };

        class Train {
            //пассажирские вагоны загружаются быстро за фиксированный срок
            //товарные вагоны загружаются/разгружабтся дольше
            //загрузка и тех, и тех - сумма по времени
        private:
            string name;
            Schedule schedule;
            //Stop *currentDepartureStation; <-показывает последнюю станцию, с которой отправлялся поезд (на какой он стоит в даннй момент)
            //short int
            Locomotive locomotive;
            vector<Van*> listOfVans;
            //bool status;
            int speed{};
            void calculateSpeed();
            void inputListOfVansFromString(string &inputString);
            void updateTractionForceOfLocomotive();
            void updateTheParametersOfTheTrain();
        public:
            Train(string &name, int &locomotiveAge, string &listOfVans, string &schedule);

            int loadingOfPassengerVans(int numberOfPersons);

            int loadingOfFreightVans(int numberOfGoods);

            void uploadingOfPassengerVans(int numberOfPersons);

            void uploadingOfFreightVans(int numberOfGoods);
        };

    }

    using namespace train;

    class Resource{
    private:
        int type;
        int amount;
    public:
        explicit Resource();
        explicit Resource(int type, int amount);
        int getType();
        int getAmount();
        void setAmount(int amount);
    };

    class Station {
        //у станции должно быть текущее количество ресурса
        //затем у станции - вектор ресурсов
        //ресурсы должны обновляться со временем (т.е. вагоны забирают или отдают что-то, логично)
    private:
        string name;
        int id;
        static int counter; //чтоб у каждый вершины был номер, так проще формировать карту
    protected:
        vector<Resource> resources{};
        short int type;
    public:
        explicit Station(string name);

        explicit Station(string name, int resource);

        void setName(string newName);

        string getName();

        //virtual void loading(Train train);

        //virtual void uploading(Train train);

        //void temporaryStop();
        //void transit();
    };

    class PassengerStation : public Station {
    public:
        explicit PassengerStation(string name, int numberOfPassengers);
        // explicit - предотвращает неявное преобразование типов
        // (выполняется всякий раз, когда требуется один фундаментальный тип данных,
        // но предоставляется другой, и пользователь не указывает компилятору, как выполнить конвертацию )
        // рекомендуется явно объявлять конструкторы explicit всегда, кроме случаев, когда неявное преобразование семантически оправдано.

        //void loading(Train train) override;

        //void uploading(Train train) override;
    };

    class FreightStation : public Station {
    public:
        explicit FreightStation(string name, int numberOfGoods);

        //void loading(Train train) override;

        //void uploading(Train train) override;
    };

    class PassengerAndFreightStation : public Station {
    public:
        explicit PassengerAndFreightStation(string name, int numberOfPassengers, int numberOfGoods);

        //void loading(Train train) override;

        //void uploading(Train train) override;
    };

    class Map {
    private:
        vector<Station*> listOfStations;
        vector<vector<int>> listOfPaths;
    public:
        void inputStationsFromFile(const char *path);

        void inputPathsFromFile(const char *path);

        void inputMapFromFile(const char *path);
    };

    //на выходе должен быть список событий в хронологическом порядке
    //можно попробовать высчитывать характерные моменты времени для каждого поезда и заносить в расписание
    //затем в соответствии с расписанием менять статус поезда и указатель на текущую станцию отправления
    //можно подумать, как реализовать динамическое расчитывание статусов в помощью переменной "интервал времени" для каждого поезда
    //по истечению этого "интервала времени" параметры поезда будут обновляться
    //если он был в пути, значит смещаем указатель на следующую станцию и смотрим, что там делать
    //если это транзит, то меняем станцию отправления, высчитываем новый интервал (не меняя скорость)
    //если он разгружался/загружался/стоял, то высчитываем новую скорость, новый интервал и статус меняем на "в пути"
    //если в расписании закончились станции, он благополучно доехал до последней, то выполняется действие последней станции и после статус меняется на "маршрут пройден"
    //в случае с динамическим обновлением нужно будет каждый такт обновлять поезда. если есть изменения - выводятся на консоль

    //автоматическое моделирование - вывод от t=0 до t=момент, когда последний поезд доедет (станет неактивным)
    //режим работы - поле класса Railway, не читается из файла, а вводится вместе с названием файла входных данных
    class Railway {
    private:
        Map map;
        vector<Train> listOfTrains;
        static int clock;
    public:
        void inputListOfTrainsFromFile(const char *path);
        void inputModelFromFile(const char *path);
    };

}
#endif //RAILWAY_RAILWAY_H
