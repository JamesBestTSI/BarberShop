#ifndef BARBER_H
#define BARBER_H
#include <list>
#include <memory>
#include "Customer.h"

class Barber{
    private:
        int ID;
        std::shared_ptr<Customer> barbersCustomer;

    public:
        Barber();
        explicit Barber(int id);
        ~Barber();
        int GetID();
        void GiveCustomer(std::shared_ptr<Customer> &currentCustomer);
        void RemoveCustomer();
        std::shared_ptr<Customer> GetCustomer();
        void CutCustomerHair();
};

#endif
