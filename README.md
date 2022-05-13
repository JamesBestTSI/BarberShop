# The Barbershop Project

This project creates a thread for each barber and a thread for each customer that uses mutex's, locks and conditional vars in order to access, modify and use data between them all.

## The Shop class
This creates a list of barbers and a list of customers.
It is used to create new barbers and customer threads, and kick starting off the main loop of the program.

## The Customer class
The customer class holds all the vars needed to keep track of a customer and what current stage they are at.
When we start off a new customer thread, we pass in the customer instance that we want to be working with.

Each customer will do the following:
- Enter the shop at a random time.
- If the barber is asleep, wake him up to get their hair cut.
- If the barber is busy, they waill take a seat in the que.
- If they have waited for too long they will get up and leave.

## The Barber class
The barber class holds all the vars needed to work on a customer.
Then we start off a new barber thread, we pass in the barber instance that we want to be working with.

Each barber will do the following:
- Enter the shop and be ready to work.
- If there are no customers, he will take a nap.
- When a customer wakes him up, he will cut that customers hair.
- Once he has cut their hair, he will check for another customer.
- If there is a customer, he will cut the next one in the queue.
- If there is no customer, he will go back to sleep. 