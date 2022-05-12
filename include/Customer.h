#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <chrono>

class Customer{
    private:
        int ID;
        bool hairCut = false;
        bool havingHairCut = false;
        bool waiting = false;
        bool leaving = false;
        std::chrono::steady_clock::time_point waitingStartTime;

    public:
        Customer();
        explicit Customer(int id);
        ~Customer();
        int GetID();
        void SetHairCut(bool state);
        bool isHairCut();
        void SetHairBeingCut(bool state);
        bool isHairBeingCut();
        void SetWaiting(bool state);
        bool isWaiting();
        void SetLeaving(bool state);
        bool isLeaving();
        void SetwaitingStartTime();
        std::chrono::steady_clock::time_point GetwaitingStartTime();
};

#endif
