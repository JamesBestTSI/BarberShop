#include "../include/Shop.h"
#include <string>

Shop::Shop() : openingTime{std::chrono::steady_clock::now()}
{
    barberUID = 0;
    customerUID = 0;
    isOpen = true;
    MaxCustomers = 15;
};

Shop::~Shop(){};

void Shop::Run(){
    system("cls");
    int customerCount = 0;
    AddBarber();
    AddBarber();
    AddCustomer();

    int time = rand() % 5+1;
    while (customerCount < 100)
    {
        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - openingTime).count() > time){
            AddCustomer();
            customerCount++;
            time = rand() % 5+1;
            DisplayQue();
            openingTime = std::chrono::steady_clock::now();
        }
    }
    isOpen = false;
};

void Shop::DisplayQue(){
    std::unique_lock<std::mutex> LockCustomerList(customerListMutex);
    system("cls");
    std::cout << "\n############## CURRENT QUE INFO ##############" << std::endl;
    for (std::list<std::shared_ptr<Barber>>::iterator barb = Barbers.begin(); barb != Barbers.end(); ++barb)
    {
        if ((*barb)->GetCustomer() != nullptr){
            std::cout << "Barber " << (*barb)->GetID() << " is cutting customer " << (*barb)->GetCustomer()->GetID() << "'s hair" << std::endl;
        }
        else{
            std::cout << "Barber " << (*barb)->GetID() << " is not cutting hair" << std::endl;
        }
    }
    std::cout << std::endl;
    int quePos = 1;
    for (std::list<std::shared_ptr<Customer>>::iterator cust = Customers.begin(); cust != Customers.end(); ++cust)
    {
        std::cout << "Customer " << (*cust)->GetID() << " is number " << quePos << " in the que" << std::endl;
        quePos++;
    }
    std::cout << "##############################################\n" << std::endl;
};

void Shop::AddBarber(){
    std::shared_ptr<Barber> barber(new Barber(barberUID));
    Barbers.push_back(barber);
    barberUID++;
    std::thread BarberThread(&Shop::StartBarber, this, std::ref(Barbers.back()));
    BarberThread.detach();
};

void Shop::AddCustomer(){
    std::thread CustomerThread(&Shop::StartCustomer, this);
    CustomerThread.detach();
};

void Shop::RemoveBarber(int IDToRemove){
    for (std::list<std::shared_ptr<Barber>>::iterator currentBarber = Barbers.begin(); currentBarber != Barbers.end();++currentBarber){
        if ((*currentBarber)->GetID() == IDToRemove){
            Barbers.erase(currentBarber++);
            return;
        }
    }
};
void Shop::RemoveCustomer(int IDToRemove){
    for (std::list<std::shared_ptr<Customer>>::iterator currentCustomer = Customers.begin(); currentCustomer != Customers.end(); ++currentCustomer){
        if ((*currentCustomer)->GetID() == IDToRemove){
            Customers.erase(currentCustomer++);
            return;
        }
    }
};

void Shop::StartBarber(std::shared_ptr<Barber> &barber){
    while (isOpen){
        // Create a lock so we can lock editing the customerList data
        std::unique_lock<std::mutex> LockCustomerList(customerListMutex);
        std::cout << "Barber " << barber->GetID() << " is ready to work!" << std::endl;

        // No customer So Barber Sleeps and Waits for next customer
        if (!customerWaiting)
        {
            // No customers waiting so unlock the customer list data and wait
            std::cout << "Barber " << barber->GetID() << " is taking a nap!" << std::endl;
            barberRequired.wait(LockCustomerList);
            std::cout << "Barber " << barber->GetID() << " has been woken up!" << std::endl;
        }

        // Customer waiting, so take customer from list and pass to the barber
        barber->GiveCustomer((*Customers.begin()));
        barber->GetCustomer()->SetHairBeingCut(true);
        // Remove that customer from the list
        Customers.erase(Customers.begin());
        if (Customers.size() == 0){
            customerWaiting = false;
        }
        std::cout << "Barber " << barber->GetID() << " is working on Customer " << barber->GetCustomer()->GetID() <<  std::endl;
        // Unlock the list
        LockCustomerList.unlock();
        // Work on that customer for 5-15
        std::this_thread::sleep_for(std::chrono::seconds((rand()%10)+6));
        barber->CutCustomerHair();

        LockCustomerList.lock();
        if (Customers.size() == 0){
            customerWaiting = false;
        }
        std::cout << "Barber " << barber->GetID() << " finished working on Customer " << barber->GetCustomer()->GetID() << std::endl;
        barber->RemoveCustomer();
        LockCustomerList.unlock();
    }
};

void Shop::StartCustomer(){

    std::shared_ptr<Customer> customer(new Customer(customerUID));
    customerUID++;

    while (isOpen && !customer->isHairCut() && !customer->isLeaving()){             // If the shop is open and the customers hair is not cut and they are not leaving 
        if (!customer->isWaiting()){                                                // and the customer is not yet waiting
            std::unique_lock<std::mutex> LockCustomerList(customerListMutex);       // Lock the customer list
            std::cout << "Customer " << customer->GetID() << " has entered the shop!" << std::endl;
            if (Customers.size() < MaxCustomers){                                   // check for an empty space for another customer
                Customers.push_back(customer);                                      // And Add this customer to the list
                customer->SetWaiting(true);                                         // Set the customer state as waiting
                customer->SetwaitingStartTime();

                if (!customerWaiting){                                              // If there are no customers waiting
                    std::cout << "Customer " << customer->GetID() << " is notifying a barber they have arrived!" << std::endl;
                    customerWaiting = true;                                         // Set "customer waiting" to true
                    barberRequired.notify_one();                                    // Notify a barber
                }
                else{
                    std::cout << "Customer " << customer->GetID() << " is waiting for an available barber!" << std::endl;
                }
                LockCustomerList.unlock();                                          // Then unlock the list so the barber can access it.
                // Else there are already customers waiting, so the customer will just have to wait in line
            }
            // Else there are already too many customers, so they will have to head home
            else
            {
                std::cout << "Customer " << customer->GetID() << " has had to leave due to lack of space!" << std::endl;
                customer->SetLeaving(true);                                             // Set the customer as leaving and wait for their thread to end
            }
        }
        else{
            // Here customer is waiting for haircut to be done
            if (!customer->isHairBeingCut()){
                if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - customer->GetwaitingStartTime()).count() > (rand()%20+16))
                {
                    // If the customer is not having their hair cut yet, and its been longer than rand(15-35 seconds)
                    std::unique_lock<std::mutex> LockCustomerList(customerListMutex); // Lock the customer list
                    std::cout << "Customer " << customer->GetID() << " waited too long and decided to leave!" << std::endl;
                    RemoveCustomer(customer->GetID());                                  // And remove this customer from the list
                    customer->SetLeaving(true);                                         // Set the customer as leaving and wait for their thread to end
                }
            }
            // Else they are having their hair cut so just wait for it to be finished
        }        
    }
    // Here we detect what happened with customer
    // leaving+Waiting  - They waited too long
    // Leaving+!Waiting - No space for customer
    // hairCut          - they had the hair cut
};