#include <stdlib.h>
#include <time.h>

#include <iostream>
using namespace std;

void customerArrival(int generatorIn, int timeIn) {
  generatorIn = rand() % 6 + 1;
  cout << "New customer arrived at time: " << timeIn << endl;
}
class Customer {
 public:
  int in_time;
  int out_time;
  int order_time;
};

class Queue {
 public:
  Customer *head;
  Queue() {}

  void enqueue(int in_time, int order_time) {}

  void dequeue() {}
};

int main() {
  int TIME = 0, generator;
  int cumulitive_time = 0;
  int customer_count = 0;
  // as many variables as you need
  srand(time(NULL));

  // store hours, every minute of that day
  while (TIME < 1020) {
    // 1. should we add a new customer to the Q at this minute?
    if (TIME > 0 && TIME <= 120) {
      // 8am through 10am
      generator = rand() % 100 + 1;
      if (generator <= 30) {
        customerArrival(generator, TIME);
        // Q.enqueue(TIME,generator);
      }
    } else if (TIME > 120 && TIME <= 210) {
      // 10am through 11:30am
    } else if (TIME > 210 && TIME <= 330) {
      // 11:30am through 1:30pm
    } else if (TIME > 330 && TIME <= 570) {
      // 1:30pm through 5:30pm
    } else if (TIME > 570 && TIME <= 720) {
      // 5:30pm through 8:00pm
    } else if (TIME > 720 && TIME <= 900) {
      // 8:00pm through 11pm
    } else if (TIME > 900 && TIME <= 1020) {
      // 11pm through 1am
    }

    // 2. is a customer going to place an order this minute?
    int order_time = rand() % 6 + 1;
    // 3. is a customer ready to depart the store at this minute?
    TIME++;
  }
  // 2. is a customer going to place an order this minute?
  int order_time = rand() % 6 + 1;
  // 3. is a customer ready to depart the store at this minute?
  TIME++;
}
/*
  60% ->  regular questions

  Traversals
  BSTs vs HT vs Linear
  Sorting: HEAP or Quick or Merge
  HTs: Show table with collision avoidance

  40% -> BSTs

  3-4 new functions
*/