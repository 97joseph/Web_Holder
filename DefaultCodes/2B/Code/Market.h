#ifndef MARKET_H_
#define MARKET_H_

#include <iostream>
#include <memory>
#include <vector>

using namespace std;


class Order;  // forward declaration

std::vector<string> splitLineBySpace(string line);

tuple<int, int, int> getOrderMaxLengths(std::vector<shared_ptr<Order>> orders);

class Order {

public:

  Order();

  Order(std::vector<string> components, int time);

  void setOrderID(string id);

  void setBuyOrSell(char bs);

  void setLimitOrMarket(char lm);

  void setDivisibleOrIndivisible(char di);

  void setTargetQuantity(int tq);

  void setLimitPrice(float lp);

  void setArrivalTime(int arrival_time);

  string getOrderID();

  char getBuyOrSell();

  char getLimitOrMarket();

  bool getDivisible();

  int getTargetQuantity();

  float getLimitPrice();

  int getArrivalTime();

  int getOrderIDLength();

  int getLimitPriceLength();

  int getTargetQuantityLength();

  void print(int max_id_length, int max_lp_length, int max_tq_length);

private:
  string orderID_;
  char buy_or_sell_;
  char limit_or_market_;
  char divisible_or_indivisible_;
  int target_quantity_;
  float limit_price_;
  int arrival_time_;
};


class StockMarket {

public:
  // StockMarket();

  StockMarket(float last_traded_price, ofstream& output_file);

  void updateTime();

  void addOrder(shared_ptr<Order> order);

  void displayPendingOrders();

  void deleteOrder(shared_ptr<Order> order);

  void executeOrders(shared_ptr<Order> order, shared_ptr<Order> priority);

  shared_ptr<Order> getPriorityOrder(std::vector<shared_ptr<Order>> matching_orders);

  bool check_match(shared_ptr<Order> buy_order, shared_ptr<Order> sell_order);

  void processOrders(shared_ptr<Order> order);

  shared_ptr<Order> handleOrderLine(string line);

  void run(string order_line);

  void displayExecution(string buy_id, string sell_id, int quantity_sold, float price);

  void logUnexecutedOrders();

private:
  float last_traded_price_;
  std::vector<shared_ptr<Order>> orders_;
  int time_;
  ofstream &output_file_;
};

#endif /* MARKET_H_ */
