#include <iostream>
#include <sstream>
#include "Market.h"
#include <vector>
#include <fstream>
#include <tuple>
#include <algorithm>

using namespace std;


// ============ Global functions =================

std::vector<string> splitLineBySpace(string line) {
  std::vector<string> components;
  stringstream sstream(line);
  string component;

  while (getline(sstream, component, ' ')) {
    components.push_back(component);
  }

  return components;
}

// returns a tuple of column lengths for vertical allignment of console output
tuple<int, int, int> getOrderMaxLengths(std::vector<shared_ptr<Order>> orders) {
  int max_id_length = 1;
  int max_lp_length = 1;
  int max_tq_length = 1;

  for (shared_ptr<Order> order : orders) {

    if (order->getOrderIDLength() > max_id_length) {
      max_id_length = order->getOrderIDLength();
    }
    if (order->getLimitPriceLength() > max_lp_length) {
      max_lp_length = order->getLimitPriceLength();
    }
    if (order->getTargetQuantityLength() > max_tq_length) {
      max_tq_length = order->getTargetQuantityLength();
    }
  }
  return {max_id_length, max_lp_length, max_tq_length};
}

// ============ Order class =================

Order::Order() {}

Order::Order(std::vector<string> components, int time) {
  setOrderID(components[0]);
  setBuyOrSell(components[1][0]);
  setLimitOrMarket(components[2][0]);
  setDivisibleOrIndivisible(components[3][0]);
  if (getLimitOrMarket() == 'L') {
    setLimitPrice(stof(components.end()[-2]));
  } else {
    setLimitPrice(0);
  }
  setTargetQuantity(stoi(components.end()[-1]));
  setArrivalTime(time);
}

void Order::setOrderID(string id) {
  orderID_ = id;
}

void Order::setBuyOrSell(char bs) {
  buy_or_sell_ = bs;
}

void Order::setLimitOrMarket(char lm) {
  limit_or_market_ = lm;
}

void Order::setDivisibleOrIndivisible(char di) {
  divisible_or_indivisible_ = di;
}

void Order::setTargetQuantity(int tq) {
  target_quantity_ = tq;
}

void Order::setLimitPrice(float lp) {
  limit_price_ = lp;
}

void Order::setArrivalTime(int arrival_time) {
  arrival_time_ = arrival_time;
}

string Order::getOrderID() {
  return orderID_;
}

char Order::getBuyOrSell() {
  return buy_or_sell_;
}

char Order::getLimitOrMarket() {
  return limit_or_market_;
}

bool Order::getDivisible() {
  if (divisible_or_indivisible_ == 'D') {
    return true;
  }
  return false;
  // return divisible_or_indivisible_;
}

int Order::getTargetQuantity() {
  return target_quantity_;
}

float Order::getLimitPrice() {
  return limit_price_;
}

int Order::getArrivalTime() {
  return arrival_time_;
}

int Order::getOrderIDLength() {
  return int(getOrderID().length());
}

int Order::getLimitPriceLength() {
  // only gets the number of characters to be shown in the console output
  if (getLimitOrMarket() != 'M') {
    return to_string(int(getLimitPrice())).length() + 3;
  }
  return 1;  // length of a single character 'M'
}

int Order::getTargetQuantityLength() {
  return int(to_string(getTargetQuantity()).length());
}

// console output for an individual order
void Order::print(int max_id_length, int max_lp_length, int max_tq_length) {
  int gap = 3;

  cout << orderID_ << string(max_id_length - orderID_.length() + gap, ' ');

  if (limit_or_market_ == 'M') {
    cout << limit_or_market_ << string(max_lp_length - 1, ' ');
  } else {
    int lp_length = to_string(int(limit_price_)).length() + 3; // decimal point + 2dp
    cout << string(max_lp_length - lp_length, ' ') << limit_price_;
  }

  cout << string(gap, ' ') << target_quantity_;
  int tq_length = int(to_string(target_quantity_).length());
  cout << string(max_tq_length - tq_length, ' ');
}

// ============ StockMarket class =================

// StockMarket::StockMarket() {}

StockMarket::StockMarket(float last_traded_price, ofstream &output_file) : output_file_(output_file) {
    last_traded_price_ = last_traded_price;
    time_ = 0;
}

void StockMarket::updateTime() {
  time_ += 1;
}

// adds a new order to the front of the orders_ vector
void StockMarket::addOrder(shared_ptr<Order> order) {
  orders_.insert(orders_.begin(), order);
  updateTime();
}

// handles the console output after each change to pending orders
// @TODO: order by priority
void StockMarket::displayPendingOrders() {

  std::vector<shared_ptr<Order>> buy_orders;
  std::vector<shared_ptr<Order>> sell_orders;
  for (shared_ptr<Order> order : orders_) {
    if (order->getBuyOrSell() == 'B') {
      buy_orders.push_back(order);
    } else {
      sell_orders.push_back(order);
    }
  }

  int buy_max_id, buy_max_lp, buy_max_tq;
  tie(buy_max_id, buy_max_lp, buy_max_tq) = getOrderMaxLengths(buy_orders);
  int sell_max_id, sell_max_lp, sell_max_tq;
  tie(sell_max_id, sell_max_lp, sell_max_tq) = getOrderMaxLengths(sell_orders);

  int gap = 3;
  string seperator = "| ";
  int buy_length = buy_max_id + buy_max_lp + buy_max_tq + (gap * 2);  // 2 breaks, 3 spaces per break
  int sell_length = sell_max_id + sell_max_lp + sell_max_tq + (gap * 2);  // 2 breaks, 3 spaces per break
  int total_length = buy_length + gap + seperator.length() + sell_length;

  // line 1
  cout << "\nLatest price: " << last_traded_price_ << endl;

  // line 2 and 3
  cout << "Buy" + string(buy_length + gap - 3, ' ') << seperator  // - 3 for len("Buy")
  << "Sell" << endl << string(total_length, '=') << endl;

  // line 4+ (orders)
  int row_count = int(max(buy_orders.size(), sell_orders.size()));
  for (int i=0; i<row_count; i++) {

    // output buy order if exists, handle whitespace between orders
    if (i < int(buy_orders.size())) {
      buy_orders[i]->print(buy_max_id, buy_max_lp, buy_max_tq);
      cout << string(gap, ' ');
    } else {
      // cout << string(total_length - sell_length - 2, ' ');
      cout << string(buy_length + gap, ' ');
    }
    cout << seperator;

    // output sell order if exists
    if (i < int(sell_orders.size())) {
      sell_orders[i]->print(sell_max_id, sell_max_lp, sell_max_tq);
    }
    cout << endl;
  }
  cout << endl;
}

// remove order from the orders_ vector (is this dereferencing the smart pointer?)
void StockMarket::deleteOrder(shared_ptr<Order> order_to_delete) {
  orders_.erase(std::remove(orders_.begin(), orders_.end(), order_to_delete), orders_.end());
}

// executes a buy/sell pair of orders
void StockMarket::executeOrders(shared_ptr<Order> order, shared_ptr<Order> other_order) {

  float execution_price;
  //  If both the buy and sell orders are limit orders, then the limit price of
  // the "older" order (the one arriving earlier) is the execution price
  if (order->getLimitOrMarket() == 'L' && other_order->getLimitOrMarket() == 'L') {
    if (order->getArrivalTime() < other_order->getArrivalTime()) {
      execution_price = order->getLimitPrice();
    } else {
      execution_price = other_order->getLimitPrice();
    }
  }

  //  If one of them is a market order and the other is a limit order,
  // then the limit order's limit price is the execution price;
  else if (order->getLimitOrMarket() == 'L' || other_order->getLimitOrMarket() == 'L') {
    if (order->getLimitOrMarket() == 'L') {
      execution_price = order->getLimitPrice();
    } else {
      execution_price = other_order->getLimitPrice();
    }
  }

  // If both are market orders, then the execution price of the previous
  // transaction is used as the execution price.
  else {
    execution_price = last_traded_price_;
  }

  // determine buy and sell orders
  shared_ptr<Order> buy_order;
  shared_ptr<Order> sell_order;
  if (order->getBuyOrSell() == 'B') {
    buy_order = order;
    sell_order = other_order;
  } else {
    buy_order = other_order;
    sell_order = order;
  }

  // save IDs for output after orders are deleted
  string buy_id = buy_order->getOrderID();
  string sell_id = sell_order->getOrderID();

  // determine quantity_sold
  int quantity_sold = min(buy_order->getTargetQuantity(), sell_order->getTargetQuantity());

  // writes execution to the text file
  displayExecution(buy_order->getOrderID(), sell_order->getOrderID(), quantity_sold, execution_price);

  // remove quantity sold from target quantity
  buy_order->setTargetQuantity(buy_order->getTargetQuantity() - quantity_sold);
  sell_order->setTargetQuantity(sell_order->getTargetQuantity() - quantity_sold);

  // delete order if target quantity reached 0
  if (buy_order->getTargetQuantity() == 0) {
    deleteOrder(buy_order);
  }
  if (sell_order->getTargetQuantity() == 0) {
    deleteOrder(sell_order);
  }

  // updates member variable last_traded_price_ once order is executed
  last_traded_price_ = execution_price;
}

// determines which matching order should be prioritised to be executed
shared_ptr<Order> StockMarket::getPriorityOrder(std::vector<shared_ptr<Order>> matching_orders) {

  std::vector<shared_ptr<Order>> priority_orders;

  // check for market orders
  for (shared_ptr<Order> order : matching_orders) {
    if (order->getLimitOrMarket() == 'M') {
      priority_orders.push_back(order);
    }
  }

  // if no market orders, get best priced orders
  if (priority_orders.empty()) {
    shared_ptr<Order> best_priced_order = matching_orders[0];
    for (shared_ptr<Order> order : matching_orders) {
      if (order->getBuyOrSell() == 'B') {
        if (order->getLimitPrice() >= best_priced_order->getLimitPrice()) {
          priority_orders.push_back(order);
        }
      } else {
        if (order->getLimitPrice() <= best_priced_order->getLimitPrice()) {
          priority_orders.push_back(order);
        }
      }
    }
  }

  // return priority order with earliest (lowest) arrival time
  shared_ptr<Order> highest_priority_order = priority_orders[0];
  for (shared_ptr<Order> order : priority_orders) {
    if (order->getArrivalTime() < highest_priority_order->getArrivalTime()) {
      highest_priority_order = order;
    }
  }
  return highest_priority_order;
}

// checks if a buy and sell order are matching (could be executed)
bool StockMarket::check_match(shared_ptr<Order> order, shared_ptr<Order> other_order) {

  // make sure one is buy and the other is sell
  if (order->getBuyOrSell() == other_order->getBuyOrSell()) {
    return false;
  }

  // find which order is the buy order and which is the sell order
  shared_ptr<Order> buy_order;
  shared_ptr<Order> sell_order;
  if (order->getBuyOrSell() == 'B') {
    buy_order = order;
    sell_order = other_order;
  } else {
    buy_order = other_order;
    sell_order = order;
  }

  //  valid match if the limit price of b is higher than or equal to the
  // limit price of s, or if one or both of them are market orders
  if (buy_order->getLimitOrMarket() != 'M' && sell_order->getLimitOrMarket() != 'M') {
    if (buy_order->getLimitPrice() < sell_order->getLimitPrice()) {
      return false;
    }
  }

  // if both are indivisible orders, they must have the same tq
  if (!buy_order->getDivisible() && !sell_order->getDivisible()) {
    if (buy_order->getTargetQuantity() != sell_order->getTargetQuantity()) {
      return false;
    }
  }

  // if only one is divisible, make sure the divisible
  // order's tq >= indivisible order's tq
  else if (!buy_order->getDivisible() || !sell_order->getDivisible()) {
    if (buy_order->getDivisible() && buy_order->getTargetQuantity() < sell_order->getTargetQuantity()) {
      return false;
    }
    if (sell_order->getDivisible() && sell_order->getTargetQuantity() < buy_order->getTargetQuantity()) {
      return false;
    }
  }

  return true;
}

// handles matching, prioritising and execution of an order (recursively if divided)
void StockMarket::processOrders(shared_ptr<Order> order) {

  // get collection of matching orders
  std::vector<shared_ptr<Order>> matching_orders;
  for (shared_ptr<Order> other_order : orders_) {

    // skip matching with self (not really necessary)
    // if (&other_order == &order) {
    //   continue;
    // }

    if (check_match(order, other_order)) {
      matching_orders.push_back(other_order);
    }
  }

  // the recursive base case
  if (matching_orders.empty()) {
    return;
  }

  // get highest priority matching order
  shared_ptr<Order> priority = getPriorityOrder(matching_orders);

  // perform execution
  executeOrders(order, priority);

  // console output updated for every Order
  displayPendingOrders();

  // check if one of the orders was divided, if so then this is the new order.
  if (find(orders_.begin(), orders_.end(), order) != orders_.end() && find(orders_.begin(), orders_.end(), priority) == orders_.end()) {
    processOrders(order);
  } else if (find(orders_.begin(), orders_.end(), priority) != orders_.end() && find(orders_.begin(), orders_.end(), order) == orders_.end()) {
    processOrders(priority);
  }
}

// creates an Order object from a line of the input file
shared_ptr<Order> StockMarket::handleOrderLine(string line) {
  std::vector<string> components = splitLineBySpace(line);
  shared_ptr<Order> order = make_shared<Order>(components, time_);
  return order;
}

// runs the stock market for one new added order
void StockMarket::run(string order_line) {
  shared_ptr<Order> order = handleOrderLine(order_line);
  addOrder(order);

  // console output updated for every Order
  displayPendingOrders();

  // handles order matching, prioritisation and execution
  processOrders(order);
}

// writes execution info to output file
void StockMarket::displayExecution(string buy_id, string sell_id, int quantity_sold, float price) {
  output_file_ << "order " << buy_id << " " << quantity_sold << " shares purchased at price " << price << endl;
  output_file_ << "order " << sell_id << " " << quantity_sold << " shares sold at price " << price << endl;
  cout << "order " << buy_id << " " << quantity_sold << " shares purchased at price " << price << endl;
  cout << "order " << sell_id << " " << quantity_sold << " shares sold at price " << price << endl;
}

void StockMarket::logUnexecutedOrders() {
  // could rewrite this for pop() if priority queue is used for orders_
  for (shared_ptr<Order> order : orders_) {
    output_file_ << "order " << order->getOrderID() << " " << order->getTargetQuantity() << " shares unexecuted" << endl;
  }
}
