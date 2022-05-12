#include "../include/Barber.h"
#include <string>

Barber::Barber(){};
Barber::Barber(int id) { 
    ID = id;
};
Barber::~Barber(){};

int Barber::GetID() { return ID; };

void Barber::GiveCustomer(std::shared_ptr<Customer> &currentCustomer)
{
    barbersCustomer = std::move(currentCustomer);
};

std::shared_ptr<Customer> Barber::GetCustomer() { return barbersCustomer; };

void Barber::CutCustomerHair(){
    barbersCustomer->SetHairCut(true);
};