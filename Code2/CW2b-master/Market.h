#ifndef MARKET_H_
#define MARKET_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <vector>

using namespace std;

class Order;
struct ComparePriority;
struct CompareArrivalTime;
class StockMarket;
class Output;

// splits a line of the input file into the components of an Order
vector<string> splitLineBySpace(string line);

// takes two opposing Orders, returns them in order of {buy, sell}
tuple<shared_ptr<const Order>, shared_ptr<const Order>>
determineBuyAndSellOrder(shared_ptr<const Order> order,
                         shared_ptr<const Order> other_order);

// returns the max column lengths for both buy and sell orders
// as a tuple for vertical allignment of console output
tuple<int, int, int>
getOrderMaxLengths(set<shared_ptr<const Order>, ComparePriority> orders);

class Order {

public:
  // constructor
  Order(string id, char bs, char lm, char di, int tq, float lp, int time);

  // copy constructor to copy an Order with a new remaining target quantity
  Order(shared_ptr<const Order> other, int target_quantity);

  string getOrderID() const;

  char getBuyOrSell() const;

  char getLimitOrMarket() const;

  char getDivisibleOrIndivisible() const;

  bool getDivisible() const;

  int getTargetQuantity() const;

  float getLimitPrice() const;

  int getArrivalTime() const;

  int getOrderIDLength() const;

  // returns the length of either the limit price or market character ('M')
  int getLimitPriceLength() const;

  int getTargetQuantityLength() const;

  // prints formatted order to console
  void print(int max_id_length, int max_lp_length, int max_tq_length) const;

private:
  const string order_id_;
  const char buy_or_sell_;
  const char limit_or_market_;
  const char divisible_or_indivisible_;
  const int target_quantity_;
  const float limit_price_;
  const int arrival_time_;
};

// set comparator to sort by the established priority criteria
struct ComparePriority {
  bool operator()(shared_ptr<const Order> order,
                  shared_ptr<const Order> other_order);
};

// set comparator to sort by arrival time
struct CompareArrivalTime {
  bool operator()(shared_ptr<const Order> order,
                  shared_ptr<const Order> other_order);
};

class Output {

public:
  // constructor
  Output(ofstream &output_file);

  // outputs exectution information to both the output file and console
  void displayExecution(string buy_id, string sell_id, int quantity_sold,
                        float price);

  // outputs unexecuted Orders to the output file
  void logUnexecutedOrders(
      set<shared_ptr<const Order>, CompareArrivalTime> unexecuted);

  // outputs the current state of the stock market after each Order is handled
  void displayPendingOrders(StockMarket &s);

  // prompt user for input to continue
  static void wait();

private:
  ofstream &output_file_;
};

class StockMarket {

public:
  // constructor
  StockMarket(float last_traded_price, ofstream &output_file);

  // increments time counter by 1
  void updateTime();

  // adds a new Order to its respective collection
  void addOrder(shared_ptr<const Order> order);

  // deletes an Order from its respective collection
  void deleteOrder(shared_ptr<const Order> order);

  // takes two Orders and returns the price of their
  // execution based on the established criteria
  float calculateExecutionPrice(shared_ptr<const Order> order,
                                shared_ptr<const Order> other_order);

  // takes two matching orders, executes them, updates the orders if necessary,
  // and displays and logs execution information. Returns an optionally
  // initialised Order object, and a boolean to confirm its state
  tuple<bool, shared_ptr<const Order>>
  executeOrders(shared_ptr<const Order> order,
                shared_ptr<const Order> priority);

  // returns a boolean stating if the two Orders
  // match based on the existing criteria
  bool checkMatch(shared_ptr<const Order> buy_order,
                  shared_ptr<const Order> sell_order);

  // finds the highest priority matching Order and calls executeOrders.
  // If there is a remaining Order, this method is called recursively
  // till no other Order matches or if there is no remaining order
  void processOrders(shared_ptr<const Order> order);

  // instantiates an Order object from a line of the input file
  shared_ptr<const Order> handleOrderLine(string line);

  // runs an iteration of the stockMarket for each Order line
  void run(string order_line);

  // returns a collection of all unexecuted Orders ordered by arrival time
  set<shared_ptr<const Order>, CompareArrivalTime> getUnexecutedOrders();

  // gets and logs all unexecuted Orders
  void handleEndOfDay();

private:
  float last_traded_price_;
  set<shared_ptr<const Order>, ComparePriority> buy_orders_;
  set<shared_ptr<const Order>, ComparePriority> sell_orders_;
  int time_;
  Output output_;

  friend class Output;
};

#endif /* MARKET_H_ */
