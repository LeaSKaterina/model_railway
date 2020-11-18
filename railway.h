//
// Created by HP on 12.11.2020.
//

#ifndef RAILWAY_RAILWAY_H
#define RAILWAY_RAILWAY_H

#include <string>
#include <vector>

using namespace std;

enum Action {
    START,
    TRANSIT,
    TEMPORARY_PARKING,
    LOADING,
    UNLOADING,
};

enum Load {
    IS_LOADED = true,
    IS_NOT_LOADED = false
};

enum KindOfStationOrVan {
    UNKNOWN_TYPE,
    PASSENGER,
    FREIGHT,
    PASSENGER_AND_FREIGHT
};

namespace rw {

    namespace train {

        class Van {
        private:
            static int counter;
        protected:
            short int type;
            bool load;
            int number;
        public:
            Van();

            virtual void loading();

            virtual void uploading();

            bool vanIsLoaded();

            short int getTypeOfVan();

            int getNumber();
        };

        class PassengerVan : public Van {
        public:
            PassengerVan();

            void loading() override;

            void uploading() override;
        };

        class FreightVan : public Van {
        public:
            FreightVan();

            void loading() override;

            void uploading() override;
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
        private:
            string name;
            Schedule schedule;
            Locomotive locomotive;
            vector<Van*> listOfVans;
            int speed{};
            void calculateSpeed();
            void inputListOfVansFromString(string &inputString);
            void updateTractionForceOfLocomotive();
        public:
            Train(string &name, int &locomotiveAge, string &listOfVans, string &schedule);

            void loadingOfPassengerVans();

            void loadingOfFreightVans();

            void loadingOfAllVans();

            void uploadingOfPassengerVans();

            void uploadingOfFreightVans();

            void uploadingOfAllVans();
            //void temporaryStop();
            //void transit();
        };

    }

    using namespace train;

    class Station {
    private:
        string name;
        int id;
        static int counter; //чтоб у каждый вершины был номер, так проще формировать карту
    protected:
        short int type;
    public:
        explicit Station(string name);

        void setName(string newName);

        string getName();

        virtual void loading(Train train);

        virtual void uploading(Train train);
    };

    class PassengerStation : public Station {
    public:
        explicit PassengerStation(string name);
        // explicit - предотвращает неявное преобразование типов
        // (выполняется всякий раз, когда требуется один фундаментальный тип данных,
        // но предоставляется другой, и пользователь не указывает компилятору, как выполнить конвертацию )
        // рекомендуется явно объявлять конструкторы explicit всегда, кроме случаев, когда неявное преобразование семантически оправдано.

        void loading(Train train) override;

        void uploading(Train train) override;
    };

    class FreightStation : public Station {
    public:
        explicit FreightStation(string name);

        void loading(Train train) override;

        void uploading(Train train) override;
    };

    class PassengerAndFreightStation : public Station {
    public:
        explicit PassengerAndFreightStation(string name);

        void loading(Train train) override;

        void uploading(Train train) override;
    };

    class Map {
    private:
        vector<Station *> listOfStations;
        vector<vector<int>> listOfPaths;
    public:
        void inputStationsFromFile(const char *path);

        void inputPathsFromFile(const char *path);

        void inputMapFromFile(const char *path);
    };

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
