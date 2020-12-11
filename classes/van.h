//
// Created by HP on 23.11.2020.
//

#ifndef RAILWAY_VAN_H
#define RAILWAY_VAN_H

#include "resource.h"

class VanException : public Exception{
private:
    int amountOfExceptionalResource;
public:
    VanException(string error, int amountOfResource);
    int getAmountOfExceptionalResource();
};

enum Load {
    IS_LOADED = true,
    IS_NOT_LOADED = false
};


namespace rw {
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

            virtual void loading(int numberOfResource);

            virtual void unloading(int numberOfResource);

            bool isLoaded();

            bool isEmpty();

            short int getTypeOfVan();

            int getMaximumLoad();

            int getNumber();

            double getLoadCoefficient();

            int getCurrentLoad();
        };

        class PassengerVan : public Van {
        public:
            explicit PassengerVan();

            explicit PassengerVan(int numberOfPersons);

            void loading(int numberOfPersons) override;

            void unloading(int numberOfPersons) override;
        };

        class FreightVan : public Van {
        public:
            explicit FreightVan();

            explicit FreightVan(int numberOfGoods);

            void loading(int numberOfGoods) override;

            void unloading(int numberOfGoods) override;
        };

    }
}

#endif //RAILWAY_VAN_H
