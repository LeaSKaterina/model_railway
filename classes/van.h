//
// Created by HP on 23.11.2020.
//

#ifndef RAILWAY_VAN_H
#define RAILWAY_VAN_H

#include "resource.h"

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
            int type;
            bool load;
            int number;
            void setTypeOfResources(int newType);
        public:
            Van();

            explicit Van(int numberOfResource);

            virtual int loading(int numberOfResource);

            virtual int unloading(int numberOfUnloadedResource);

            bool isLoaded();

            bool isEmpty();

            int getTypeOfVan();

            int getMaximumLoad();

            int getNumber();

            double getLoadCoefficient();

            int getCurrentLoad();
        };

        class PassengerVan : public Van {
        public:
            explicit PassengerVan();

            explicit PassengerVan(int numberOfPersons);

            int loading(int numberOfPersons) override;

            int unloading(int numberOfPersons) override;
        };

        class FreightVan : public Van {
        public:
            explicit FreightVan();

            explicit FreightVan(int numberOfGoods);

            int loading(int numberOfGoods) override;

            int unloading(int numberOfGoods) override;
        };

    }
}

#endif //RAILWAY_VAN_H
