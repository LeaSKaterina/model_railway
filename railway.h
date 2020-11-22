//
// Created by HP on 12.11.2020.
//

#ifndef RAILWAY_RAILWAY_H
#define RAILWAY_RAILWAY_H

#include <string>
#include <vector>

using namespace std;

enum StatusOfTheTrain {
    REMOVED_FROM_THE_ROUTE = 0,
    PASSED_THE_ROUTE = 0,
    STANDING,
    ON_THE_WAY,
    IS_BEING_LOADED,
    IS_BEING_UNLOADED
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

enum Action {
    LOADING,
    UNLOADING,
    TRANSIT,
    TEMPORARY_STOP
};

bool checkThatTheFileIsOpen(ifstream &F);

void goToTheNextLine(ifstream &F);

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

    class Station;
    namespace train {

        class Van {
        private:
            static int counter;
        protected:
            Resource maximumLoad;
            Resource currentLoad;
            short int type;
            bool load;
            int number;
        public:
            Van();

            explicit Van(int numberOfResource);

            virtual int loading(int numberOfResource);

            virtual void unloading(int numberOfResource);

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

            void unloading(int numberOfPersons) override;
        };

        class FreightVan : public Van {
        public:
            explicit FreightVan();

            explicit FreightVan(int numberOfGoods);

            int loading(int numberOfGoods) override;

            void unloading(int numberOfGoods) override;
        };

        class Route {
        private:
            vector<Station *> listOfStops;
        public:
            void addStation(Station *station);

            void inputFromString(string &inputString);

            vector<Station *> *getListOfStops();

            Station *getThePointOfDeparture();
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

            void calculateTractionForce(vector<Van *> &vans);

            int getInitialSpeed();

            int getTractionForce();

            void setAge(int newAge);
        };

        class Train {
        private:
            string name;
            Route route;
            Station *currentDepartureStation;
            Locomotive locomotive;
            vector<Van *> listOfVans;
            int timeBeforeArrivalOrDeparture = 0;
            int status;
            int speed{};

            void inputListOfVansFromString(string &inputString);

            void moveAlongTheRoute();

            void updateTractionForceOfLocomotive();

        public:
            Train(string &name, int &locomotiveAge, string &listOfVans, string &route);

            vector<Station *> *getRoute();

            int getTimeBeforeArrivalOrDeparture();

            int getStatus();

            int getSpeed();

            Station *getDepartureStation();

            Station *getArrivalStation();

            void calculateSpeed();

            //--------------------------------------------------------------------------------------------------------------------------ДОДЕЛАТЬ-----------
            int loadingOfPassengerVan(int number, int numberOfPersons);

            int loadingOfFreightVan(int number, int numberOfGoods);

            void unloadingOfPassengerVan(int number, int numberOfPersons);

            void unloadingOfFreightVan(int number, int numberOfGoods);
            //----------------------------------------------------------------------------------------------------------------------------------------------

            void isArrived();

            void departedFrom();

            void simplyExist();

            void setTravelTime(int time);

            void removeFromRoute();

            void passTheRoute();
        };

    }

    using namespace train;

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

    class Map {
    private:
        vector<Station *> listOfStations;
        vector<vector<int>> listOfPaths;

        void moveTheFileToPaths(ifstream &F, int numberOfStations);

        void inputAdjacencyMatrix(ifstream &F, int numberOfStations);

        int getElementOfTheAdjacencyMatrix(int i, int j);

        void inputANewStationFromFile(ifstream& F);

        void inputANewStationWithUnknownType(ifstream &F);

        void inputANewPassengerStation(ifstream &F);

        void inputANewFreightStation(ifstream &F);

        void inputANewPassengerAndFreightStation(ifstream &F);

    public:

        void inputStationsFromFile(const char *path);

        void inputPathsFromFile(const char *path);

        void inputMapFromFile(const char *path);

        Station *getStation(string &nameOfStation);

        int getPath(Station *station1, Station *station2);
    };

    //на выходе должен быть список событий в хронологическом порядке
    //автоматическое моделирование - вывод от t=0 до t=момент, когда последний поезд доедет (станет неактивным)
    //режим работы - поле класса Railway, не читается из файла, а вводится вместе с названием файла входных данных

    class Railway {
    private:
        Map map;
        vector<Train> listOfTrains;
        static int clock;

        static int getRandomAction();

        void calculateTravelTime(Train *train);

        void moveTheFileToTrains(ifstream &F);

        void inputANewTrainFromFile(ifstream& F);

    public:
        explicit Railway(const char *path);

        void inputListOfTrainsFromFile(const char *path);

        void inputModelFromFile(const char *path);

        void putTrainsOnTheMap();

        void liveAUnitOfTime();
    };

}
#endif //RAILWAY_RAILWAY_H
