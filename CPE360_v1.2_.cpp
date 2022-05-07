#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;
//We don't have a solution to avg queue size just yet if you want to take a look at it.

//Additionally, we need to start looking at the optimzation code and see how we can run two people at a time. This could be done by calling the tickDown function
//and removing from the class and make it accept a pointer to the queue we want to adjust? Or we could simply dupe and 
//adjust that funciton to call it but not go all the way down the string...

class Customer {
 public:
  int in_time;          //time that you get into the store
  int out_time;         //time that you leave the store
  int order_time;       //time spent in queue
  int order_len;        //amount to time for order to be processed (1-6)
  Customer *next;       //Next customer behind 

  Customer() { 
    in_time = 0;
    out_time = 0;
    order_time = 0;
    order_len = 0;

    next = NULL;
   }
};


class Queue {
 public:
  Customer *head;

  Queue() { 
    head = NULL;
  }

  // reconcile the enqueue and customer arrival + rename dequeue? - done
  /*
  void customerArrival(int generatorIn, int timeIn) {
    generatorIn = rand() % 6 + 1;
    cout << "New customer arrived at time: " << timeIn << endl;
  }
  */

  void enqueue(int time) {
    cout << "A new customer arrives in the queue at tick: " << time << "\n";
    int order = rand() % 6 + 1;
    Customer *temp = new Customer;
    temp->in_time = time;
    temp->order_len = order;

    if (head == NULL) {
      head = temp;
    } 
    
    else {
      temp->next = head;
      head = temp;
    }
  }

  void dequeue(int cur_time) {
    //Empty Queue - should not be touched at all but just in case.
    if (head == NULL) {
      cout << "Line is empty. Lets wait and see if some customers show up." << endl;
      return;
    } 
    
    //One item in the queue
    else if (head->next = NULL) {
      head->out_time = cur_time;
      cout << "Another order fulfilled! They waited: "
           << (head->in_time) - (head->out_time) << endl;
      delete head;//TODO add in the summation of the times waited in line and total time waited
      head = NULL;
      return;
    } 
    

    else {
      Customer *chase, *follow;
      follow = chase = head;

      while (chase->next != NULL) {
        follow = chase;
        chase = follow->next;
      }

      chase -> out_time = cur_time;
      follow -> order_len = follow->in_time - cur_time;           //I think this works to say how much the next customer waits in line
      //, but I would like a second opinion. I don't think that this works for the first and last customers though.

      //TODO same thing, add in summation of time waited in line and overall

      cout << "Another Order fulfilled! They waited: "
           << (chase -> in_time) - (cur_time) << "Next customer in line!" << endl;

      follow->next = NULL;
      delete chase;
      return;
    }
  }

  void tickDown(int cur_time){
    Customer *temp;
    temp = head;
    if (temp == NULL){
      return;
    }
    else{
      while (temp->next != NULL) {
        temp = temp->next;
      }

      if (temp -> order_len == 0) {  //Seg Fault here?? 
      // TODO figure out how to read the time in a customer to know when to fulfill order. - Maybe fixed? double check
        dequeue(cur_time);
        cout << "Removing a customer from the line. Get out." << endl;
      }
      else {
        temp->order_len -= 1;
      }
      //return customer_count;
    }
  }
};



int main() { //TODO fix up the main function - What is needed? - 
  Queue QQ;
  int TIME = 0, generator;
  int cumulitive_time = 0;
  int customer_count = 0;         //This in't working quite right, I think that we might need a static variable type to carry over jsut the last customers order time...
  // as many variables as you need


  //srand(time(NULL));

  // store hours, every minute of that day
  while (TIME < 1020) {
    // 1. should we add a new customer to the Q at this minute?
    if (TIME > 0 && TIME <= 120) {
      // 8am through 10am
      generator = rand() % 100 + 1;
      if (generator <= 30) {
        //customerArrival(generator, TIME); //TODO see above (enqueue)
        QQ.enqueue(TIME);
        customer_count += 1;
      }
    } 

    else if (TIME > 120 && TIME <= 210) {
      // 10am through 11:30am
      generator = rand() % 100 + 1;
      if (generator <= 10){
        QQ.enqueue(TIME);
        customer_count += 1;
      }
    } 

    else if (TIME > 210 && TIME <= 330) {
      // 11:30am through 1:30pm
      generator = rand() % 100 + 1;
      if (generator <= 40) {
        QQ.enqueue(TIME);
        customer_count += 1;
      }
    } 

    else if (TIME > 330 && TIME <= 570) {
      // 1:30pm through 5:30pm
      generator = rand() % 100 + 1;
      if (generator <= 10) {
        QQ.enqueue(TIME);
        customer_count += 1;
      }
    }

    else if (TIME > 570 && TIME <= 720) {
      // 5:30pm through 8:00pm
      generator = rand() % 100 + 1;
      if (generator <= 25) {
        QQ.enqueue(TIME);
        customer_count += 1;
      }
    } 

    else if (TIME > 720 && TIME <= 900) {
      // 8:00pm through 11pm
      generator = rand() % 100 + 1;
      if (generator <= 20) {
        QQ.enqueue(TIME);
        customer_count += 1;
      }
    } 

    else if (TIME > 900 && TIME <= 1020) {
      // 11pm through 1am
      generator = rand() % 100 + 1;
      if (generator <= 10) {
        QQ.enqueue(TIME);
        customer_count += 1;
      }
    }

    // 2. is a customer going to place an order this minute?
    //int order_time = rand() % 6 + 1;
    // 3. is a customer ready to depart the store at this minute?
    QQ.tickDown(TIME);

/*    
    Customer *temp = head;
    while (temp ->next != NULL){

    }

    if (temp->order_len == 0) {  // TODO figure out how to read the time in a customer to know when to fulfill order. - Maybe fixed? double check
      QQ.dequeue(TIME);
      customer_count -= 1;
    } else {
      temp -> order_len -= 1;
    }
    */
    TIME++;
  }
}