#ifndef SHOP_H
#define SHOP_H
#include "Barber.h"
#include "Customer.h" 
#include <iostream>
#include <list>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

class Shop{
private:
    bool isOpen = true;
    int MaxCustomers;
    std::list<std::shared_ptr<Barber>> Barbers;
    std::list<std::shared_ptr<Customer>> Customers;
    int barberUID;
    int customerUID;
    std::mutex customerListMutex;
    std::condition_variable barberRequired;
    bool customerWaiting = false;
    std::chrono::steady_clock::time_point openingTime;

public:
    Shop();
    ~Shop();
    void Run();
    void AddBarber();
    void AddCustomer();
    void RemoveBarber(int IDToRemove);
    void RemoveCustomer(int IDToRemove);
    void StartBarber(std::shared_ptr<Barber> &barber);
    void StartCustomer();
    void DisplayQue();
};

#endif
