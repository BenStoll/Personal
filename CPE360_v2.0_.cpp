#include <stdlib.h>
#include <time.h>

#include <iostream>
using namespace std;

int currentNumCustomers = 0;
/*
int currentOrderTimeLeft = -1;  // meant to replace tickDown function
*/


int totalWaitTime = 0;
int totalServiceTime = 0;
int totalQueueLength = 0;
int totalNumCustomers = 0;

// Tracker is for part 2.1, tracking metrics.  Current issue is initializing
// best times - if we intialize at 0, that complicates matters - maybe
// initialize at super high number, NULL, INF, or first customer?

// This is used to track best and worst case scenarios.  Updated when an if
// statement goes through
class Tracker {
 public:
  string type;
  int length;
  int start;
  int end;

  Tracker(string inputType) {
    type = inputType;
    length = 0;
    start = 0;
    end = 0;
  }
};

Tracker *bestWait = new Tracker("wait");
Tracker *bestService = new Tracker("service");
Tracker *bestQueue = new Tracker("queue");

Tracker *worstWait = new Tracker("wait");
Tracker *worstService = new Tracker("service");
Tracker *worstQueue = new Tracker("queue");

class Customer {
 public:
  int in_time;     // time that you get into the store
  int out_time;    // time that you leave the store
  int order_time;  // time that it takes to make their order
  Customer *next;

  Customer() {
    in_time = 0;
    out_time = 0;
    order_time = 0;

    next = NULL;
  }
};

// update a wait / service tracker
void updateWSTracker(Customer *current, Tracker *tracking, int newLength) {
  tracking->length = newLength;
  tracking->start = current->in_time;

  if (tracking->type == "wait") {
    tracking->end = current->out_time - current->order_time;
  }

  else if (tracking->type == "service") {
    tracking->end = current->out_time;
  }
}

// update queue tracker
void updateQTracker(Tracker *tracking, int newLength, int time) {
  tracking->length = newLength;
  tracking->start = time;
  tracking->end = time;
}

// update wait / service total times and check trackers
void updateWSTimes(Customer *current) {
  // current wait and service times
  int currentWaitTime = current->out_time - current->in_time;
  int currentServiceTime = currentWaitTime + current->order_time;

  // update total counters
  totalWaitTime += currentWaitTime;
  totalServiceTime += currentServiceTime;

  // check best and worst case trackers
  if (bestWait->length > currentWaitTime)
    updateWSTracker(current, bestWait, currentWaitTime);
  if (worstWait->length < currentWaitTime)
    updateWSTracker(current, worstWait, currentWaitTime);

  if (bestService->length > currentServiceTime)
    updateWSTracker(current, bestService, currentServiceTime);
  if (worstService->length < currentServiceTime)
    updateWSTracker(current, worstService, currentServiceTime);
}

class Queue {
 public:
  Customer *head;

  Queue() { head = NULL; }

  // Changed temp to nextCustomer; it's easier to follow
  void enqueue(int time) {
    cout << "A new customer arrives in the queue at tick: " << time << "\n";
    Customer *nextCustomer = new Customer;
    nextCustomer->in_time = time;
    nextCustomer->order_time = rand() % 6 + 1;
    cout << "The order length is: " << nextCustomer->order_time << "\n\n";

    // update currentOrderTimeLeft for empty scenarios
    if (head == NULL) {
      head = nextCustomer;
      //currentOrderTimeLeft = head->order_time;
    }

    else {
      nextCustomer->next = head;
      head = nextCustomer;
    }

    // update counters
    currentNumCustomers++;
    totalNumCustomers++;
  }

  void dequeue(int cur_time) {
    // Empty Queue - should not be touched at all but just in case.
    if (head == NULL) {
      cout << "Line is empty. Lets wait and see if some customers show up."
           << endl;
      if (bestQueue->length > 0)
        updateQTracker(bestQueue, 0,
                       cur_time);  // you're not getting better than 0
      return;
    }

    // One item in the queue
    else if (head->next == NULL) {
      head->out_time = cur_time;
      cout << "Another order fulfilled! They waited: "
           << (head->out_time) - (head->in_time) << endl;
      updateWSTimes(head);
      delete head;
      head = NULL;
      if (bestQueue->length > 1)
        updateQTracker(bestQueue, 1, cur_time);  // maybe it'll never hit 0?
      return;
    }

    else {
      Customer *chase, *follow;
      follow = chase = head;

      {
      while (chase->order_time != 0) {
        follow = chase;
        chase = chase->next;
        if (chase == NULL){
          //There isn't a customer ready
          return;
        }
      }


      chase->out_time = cur_time;

      cout << "Out: " << chase->out_time << " In: " << chase->in_time
           << " Length: " << chase->order_time
           << " order input time: " << cur_time - chase->order_time
           << " current time: " << cur_time << endl;

      cout << "Another Order fulfilled! They waited: "
           << (cur_time) - (chase->in_time) << ". Next customer in line!"
           << endl;

      updateWSTimes(chase);

      totalWaitTime += (chase->out_time - chase->in_time);
      totalServiceTime +=
          (chase->out_time - chase->in_time + chase->order_time);

      //currentOrderTimeLeft = follow->order_time;

      follow->next = chase->next;
      delete chase;
      /*
      Customer *after, *before;
      after = before = head;

      {//Desk 2
      while (after->order_time != 0) {
        before = after;
        after = after->next;
        if (after == NULL){
          //this means that there isn't a thing to dequeue.
          return;
        }
      }
        after->out_time = cur_time;

        cout << "Out: " << after->out_time << " In: " << after->in_time
             << " Length: " << after->order_time
             << " order input time: " << cur_time - after->order_time
             << " current time: " << cur_time << endl;

        cout << "Another Order fulfilled! They waited: "
             << (cur_time) - (after->in_time) << ". Next customer in line!"
             << endl;

        updateWSTimes(after);

        totalWaitTime += (after->out_time - after->in_time);
        totalServiceTime +=
            (after->out_time - after->in_time + after->order_time);

        // currentOrderTimeLeft = follow->order_time;
        before->next = after-> next;
        delete after;
      }
*/
      currentNumCustomers--;
      
      if (worstQueue->length < currentNumCustomers)
        updateQTracker(worstQueue, currentNumCustomers, cur_time);
      }
      return;

    }
  }

  void tickDown(int cur_time) {
    Customer *chase, *follow;
    chase = follow = head;
    if (chase == NULL) {
      return;
    } 

    else if (chase-> next == NULL){
      if (chase->order_time == 0){
        dequeue(cur_time);
      }
      else {
        chase->order_time -= 1;
      }
      return;
    }

    else {
      while (chase->next != NULL) {
        chase = follow;
        chase = chase->next;
      }

      if (chase->order_time == 0) {
        dequeue(cur_time);
        cout << "Removing a customer from the line. Get out." << endl;
      } 
      else {
        chase->order_time -= 1;
      }
      
      if (follow->order_time == 0) {
        dequeue(cur_time);
        cout << "Removing a customer from the line. Get out." << endl;
      } else {
        follow->order_time -= 1;
      }
      return;
    }
  }
};

// Meant to display all values of a tracker for end-of-day metrics.  See Class
// Tracker.
void displayTrackerValues(Tracker *tracking, string BoW, string SoT) {
  // BoW means best or worst
  // SoT means size or time
  cout << BoW << ' ' << tracking->type << " time case: \n"
       << BoW << ' ' << SoT << " = " << tracking->length << endl
       << tracking->type << " start time: " << tracking->start << endl
       << tracking->type << " end time: " << tracking->end << "\n\n";
}

int main() {  // TODO fix up the main function - What is needed? -
  Queue QQ;
  int TIME = 0, generator;

  // srand(time(NULL));

  // store hours, every minute of that day
  while (TIME < 1020) {
    //currentOrderTimeLeft--;
    QQ.tickDown(TIME);

    if (TIME > 0 && TIME <= 120) {
      // 8am through 10am
      generator = rand() % 100 + 1;
      if (generator <= 30) {
        QQ.enqueue(TIME);
      }
      totalQueueLength += currentNumCustomers;
    }

    else if (TIME > 120 && TIME <= 210) {
      // 10am through 11:30am
      generator = rand() % 100 + 1;
      if (generator <= 10) {
        QQ.enqueue(TIME);
      }
      totalQueueLength += currentNumCustomers;
    }

    else if (TIME > 210 && TIME <= 330) {
      // 11:30am through 1:30pm
      generator = rand() % 100 + 1;
      if (generator <= 40) {
        QQ.enqueue(TIME);
      }
      totalQueueLength += currentNumCustomers;
    }

    else if (TIME > 330 && TIME <= 570) {
      // 1:30pm through 5:30pm
      generator = rand() % 100 + 1;
      if (generator <= 10) {
        QQ.enqueue(TIME);
      }
      totalQueueLength += currentNumCustomers;
    }

    else if (TIME > 570 && TIME <= 720) {
      // 5:30pm through 8:00pm
      generator = rand() % 100 + 1;
      if (generator <= 25) {
        QQ.enqueue(TIME);
      }
      totalQueueLength += currentNumCustomers;
    }

    else if (TIME > 720 && TIME <= 900) {
      // 8:00pm through 11pm
      generator = rand() % 100 + 1;
      if (generator <= 20) {
        QQ.enqueue(TIME);
      }
      totalQueueLength += currentNumCustomers;
    }

    else if (TIME > 900 && TIME <= 1020) {
      // 11pm through 1am
      generator = rand() % 100 + 1;
      if (generator <= 10) {
        QQ.enqueue(TIME);
      }
      totalQueueLength += currentNumCustomers;
    }

    //if (QQ.head != NULL && currentOrderTimeLeft <= 0) QQ.dequeue(TIME);

    TIME++;

    // end of day results, this prints out what 2.1 - "How good is the current
    // system?" asks for
    if (TIME >= 1020) {
      cout << "Oh my lord, thank goodness it's closing time. Let's see how we "
              "did, yeah?"
           << endl;

      cout << "Average customer wait time is: "
           << totalWaitTime / totalNumCustomers << "\n\n";

      cout << "Average customer service time is: "
           << totalServiceTime / totalNumCustomers << "\n\n";

      cout << "Average queue length is: " << totalQueueLength / 1020 << "\n\n";

      displayTrackerValues(bestWait, "best", "time");
      displayTrackerValues(worstWait, "worst", "time");
      displayTrackerValues(bestService, "best", "time");
      displayTrackerValues(worstService, "worst", "time");
      displayTrackerValues(bestQueue, "best", "size");
      displayTrackerValues(worstQueue, "worst", "size");
    }
  }
}