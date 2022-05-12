#include "../include/Customer.h"
#include <string>

Customer::Customer(){};
Customer::Customer(int id) { ID = id; };
Customer::~Customer(){};

int Customer::GetID() { return ID; };
void Customer::SetHairCut(bool state) { hairCut = state; };
bool Customer::isHairCut() { return hairCut; };
void Customer::SetHairBeingCut(bool state) { havingHairCut = state; };
bool Customer::isHairBeingCut() { return havingHairCut; };
void Customer::SetWaiting(bool state) { waiting = state; };
bool Customer::isWaiting() { return waiting; };
void Customer::SetLeaving(bool state) { leaving = state; };
bool Customer::isLeaving() { return leaving; };
void Customer::SetwaitingStartTime() { waitingStartTime = std::chrono::steady_clock::now(); };
std::chrono::steady_clock::time_point Customer::GetwaitingStartTime() { return waitingStartTime; };