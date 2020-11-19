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

    class Station;
    namespace train {

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

        class Route{
        private:
            vector<Station*> listOfStations;
            Station *currentDepartureStation; //<-показывает последнюю станцию, с которой отправлялся поезд
            // (или на какой он находится в данный момент, т.к. станция по прибытию становится пунктом отправления)
        public:
            void addStation(Station *station);
            void inputFromString(string &inputString); //-----------------------------------------------------------------------------РЕАЛИЗОВАТЬ
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
        private:
            string name;
            Route route;
            Locomotive locomotive;
            vector<Van*> listOfVans;
            //bool status; //----------------------------------------------------------------------------------------------------------РЕАЛИЗОВАТЬ
            int speed{};
            void calculateSpeed();
            void inputListOfVansFromString(string &inputString);
            void updateTractionForceOfLocomotive();
            void updateTheParametersOfTheTrain(); //------------------------------------------------------------------------------------ПЕРЕДЕЛАТЬ
        public:
            Train(string &name, int &locomotiveAge, string &listOfVans, string &route);

            int loadingOfPassengerVans(int numberOfPersons);

            int loadingOfFreightVans(int numberOfGoods);

            void uploadingOfPassengerVans(int numberOfPersons); //-----------------------------------------------------------------------ПЕРЕДЕЛАТЬ

            void uploadingOfFreightVans(int numberOfGoods); //---------------------------------------------------------------------------ПЕРЕДЕЛАТЬ
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
    private:
        string name;
        int id;
        static int counter;
    protected:
        vector<Resource> resources{};
        short int type;
    public:
        explicit Station(string name);

        explicit Station(string name, int resource);

        void setName(string newName);

        string getName();

        //пассажирские вагоны загружаются быстро за фиксированный срок
        //товарные вагоны загружаются/разгружаются дольше
        //загрузка и тех, и тех - сумма по времени

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

        Station* getStation(string &nameOfStation);
    };

    //на выходе должен быть список событий в хронологическом порядке
    //с течением времени необходимо менять указатель на текущую станцию отправления в соответствии с маршрутом
    //нужно реализовать динамическое расчитывание статусов поезда с помощью переменной "время до прибытия/отправления" для каждого поезда
    //каждый такт обновляются поля "время до прибытия/отправления" всех поездов".
        // Когда это поле зануляется (по истечению "времени"), происходит изменение(обновление) остальных полей поезда. Изменения выводятся на консоль

    //если поезд был в пути, значит смещаем указатель на станцию прибытия и смотрим, что там делать
    //в зависимости от действия на станции присваиваем "времени до отправления" соответствующее значение
                    //транзит - меняем станцию отправления, высчитываем "время до прибытия" (не меняя скорость), меняем статус на "в пути"
                    //стоянка - статус меняем на "стоит", по истечению соответствующего количества времени высчитываем новое "время до прибытия" и статус меняем на "в пути"
                    //разгрузка/загрузка - статус меняем на "стоит", по истечению соответствующего количества времени высчитываем новую скорость,
                        // новое "время до прибытия" и статус меняем на "в пути"
    //если в расписании закончились станции, поезд благополучно доехал до последней, то статус меняется на "маршрут пройден"

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
