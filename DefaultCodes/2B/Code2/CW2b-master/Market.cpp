#include "Market.h"

#include <sstream>

using namespace std;

// ============ Global functions =================
vector<string> splitLineBySpace(string line) {
  vector<string> components;
  stringstream sstream(line);
  string component;
  while (getline(sstream, component, ' ')) {
    components.push_back(component);
  }
return components;
}

tuple<shared_ptr<const Order>, shared_ptr<const Order>>
determineBuyAndSellOrder(shared_ptr<const Order> order,
                         shared_ptr<const Order> other_order) {
  if (order->getBuyOrSell() == 'B') {
    return {order, other_order};
  } else {
    return {other_order, order};
  }
}

tuple<int, int, int>
getOrderMaxLengths(set<shared_ptr<const Order>, ComparePriority> orders) {
  int max_id_length, max_lp_length, max_tq_length;
  max_id_length = max_lp_length = max_tq_length = 1;

  for (shared_ptr<const Order> order : orders) {
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
Order::Order(string id, char bs, char lm, char di, int tq, float lp, int time)
    : order_id_(id), buy_or_sell_(bs), limit_or_market_(lm),
      divisible_or_indivisible_(di), target_quantity_(tq), limit_price_(lp),
      arrival_time_(time){};

Order::Order(shared_ptr<const Order> other, int target_quantity)
    : order_id_(other->getOrderID()), buy_or_sell_(other->getBuyOrSell()),
      limit_or_market_(other->getLimitOrMarket()),
      divisible_or_indivisible_(other->getDivisibleOrIndivisible()),
      target_quantity_(target_quantity), limit_price_(other->getLimitPrice()),
      arrival_time_(other->getArrivalTime()){};

string Order::getOrderID() const { return order_id_; }

char Order::getBuyOrSell() const { return buy_or_sell_; }

char Order::getLimitOrMarket() const { return limit_or_market_; }

char Order::getDivisibleOrIndivisible() const {
  return divisible_or_indivisible_;
}

bool Order::getDivisible() const {
  if (divisible_or_indivisible_ == 'D') {
    return true;
  }
  return false;
}

int Order::getTargetQuantity() const { return target_quantity_; }

float Order::getLimitPrice() const { return limit_price_; }

int Order::getArrivalTime() const { return arrival_time_; }

int Order::getOrderIDLength() const { return int(order_id_.length()); }

int Order::getLimitPriceLength() const {
  int float_padding = 3;
  int lm_length = 1;
  if (limit_or_market_ != 'M') {
    lm_length = to_string(int(limit_price_)).length() + float_padding;
  }
  return lm_length;
}

int Order::getTargetQuantityLength() const {
  return int(to_string(target_quantity_).length());
}

void Order::print(int max_id_length, int max_lp_length,
                  int max_tq_length) const {
  int gap = 3;
  cout << order_id_ << string(max_id_length - order_id_.length() + gap, ' ');

  if (limit_or_market_ == 'M') {
    int lm_length = 1;
    cout << limit_or_market_ << string(max_lp_length - lm_length, ' ');
  } else {
    int float_padding = 3;
    int lp_length = to_string(int(limit_price_)).length() + float_padding;
    cout << string(max_lp_length - lp_length, ' ') << limit_price_;
  }
  cout << string(gap, ' ');

  int tq_length = int(to_string(target_quantity_).length());
  cout << string(max_tq_length - tq_length, ' ') << target_quantity_;
}

// ============ Comparator structs =================

bool ComparePriority::operator()(shared_ptr<const Order> order,
                                 shared_ptr<const Order> other_order) {

  // Market orders have higher priority  than Limit orders
  if (order->getLimitOrMarket() == 'M' &&
      other_order->getLimitOrMarket() == 'L') {
    return true;
  } else if (order->getLimitOrMarket() == 'L' &&
             other_order->getLimitOrMarket() == 'M') {
    return false;
  }

  // priority by arrival time
  else if (order->getLimitPrice() == other_order->getLimitPrice()) {
    return order->getArrivalTime() < other_order->getArrivalTime();
  }

  // priority by highest limit price for buy orders
  else if (order->getBuyOrSell() == 'B' && other_order->getBuyOrSell() == 'B') {
    return order->getLimitPrice() > other_order->getLimitPrice();

    // priority by lowest limit price for sell orders
  } else if (order->getBuyOrSell() == 'S' &&
             other_order->getBuyOrSell() == 'S') {
    return order->getLimitPrice() < other_order->getLimitPrice();
  }

  // priority by arrival time for two limit orders
  else {
    return order->getArrivalTime() > other_order->getArrivalTime();
  }
}

bool CompareArrivalTime::operator()(shared_ptr<const Order> order,
                                    shared_ptr<const Order> other_order) {
  return order->getArrivalTime() < other_order->getArrivalTime();
};

// ============ Output class =================

Output::Output(ofstream &output_file) : output_file_(output_file) {}

void Output::displayExecution(string buy_id, string sell_id, int quantity_sold,
                              float price) {
  output_file_ << "order " << buy_id << " " << quantity_sold
               << " shares purchased at price " << price << endl;
  output_file_ << "order " << sell_id << " " << quantity_sold
               << " shares sold at price " << price << endl;
  cout << "order " << buy_id << " " << quantity_sold
       << " shares purchased at price " << price << endl;
  cout << "order " << sell_id << " " << quantity_sold
       << " shares sold at price " << price << endl;
}

void Output::logUnexecutedOrders(
    set<shared_ptr<const Order>, CompareArrivalTime> unexecuted) {
  for (shared_ptr<const Order> order : unexecuted) {
    output_file_ << "order " << order->getOrderID() << " "
                 << order->getTargetQuantity() << " shares unexecuted" << endl;
  }
}

void Output::displayPendingOrders(StockMarket &s) {
  int buy_max_id, buy_max_lp, buy_max_tq;
  tie(buy_max_id, buy_max_lp, buy_max_tq) = getOrderMaxLengths(s.buy_orders_);

  int sell_max_id, sell_max_lp, sell_max_tq;
  tie(sell_max_id, sell_max_lp, sell_max_tq) =
      getOrderMaxLengths(s.sell_orders_);

  int gap = 3;
  string seperator = "   | ";
  int buy_length = buy_max_id + gap + buy_max_lp + gap + buy_max_tq;
  int sell_length = sell_max_id + gap + sell_max_lp + gap + sell_max_tq;
  int total_length = buy_length + seperator.length() + sell_length;

  // line 1
  cout << "\nLatest price: " << s.last_traded_price_ << endl;
  // line 2
  cout << "Buy" + string(buy_length - string("Buy").length(), ' ') << seperator
       << "Sell" << endl;
  // line 3
  cout << string(total_length, '=') << endl;
  // line 4+ (orders)
  set<shared_ptr<const Order>, ComparePriority>::iterator buy_iterator,
      sell_iterator;
  buy_iterator = s.buy_orders_.begin();
  sell_iterator = s.sell_orders_.begin();

  while (buy_iterator != s.buy_orders_.end() ||
         sell_iterator != s.sell_orders_.end()) {

    // output buy order
    if (buy_iterator != s.buy_orders_.end()) {
      shared_ptr<const Order> buy_order = *buy_iterator;
      buy_order->print(buy_max_id, buy_max_lp, buy_max_tq);
      advance(buy_iterator, 1);
    } else {
      cout << string(buy_length, ' ');
    }
    cout << seperator;
    if (sell_iterator != s.sell_orders_.end()) {
      shared_ptr<const Order> sell_order = *sell_iterator;
      sell_order->print(sell_max_id, sell_max_lp, sell_max_tq);
      advance(sell_iterator, 1);
    }
    cout << endl;
  }
  cout << endl;
}

void Output::wait() {
  cout << "Press 'Enter' to continue..." << endl;
  cin.get();
}

// ============ StockMarket class =================

StockMarket::StockMarket(float last_traded_price, ofstream &output_file)
    : output_(output_file) {
  last_traded_price_ = last_traded_price;
  time_ = 0;
}
void StockMarket::updateTime() { time_ += 1; }
void StockMarket::addOrder(shared_ptr<const Order> order) {
  if (order->getBuyOrSell() == 'B') {
    buy_orders_.insert(order);
  } else {
    sell_orders_.insert(order);
  }
  updateTime();
}
void StockMarket::deleteOrder(shared_ptr<const Order> order_to_delete) {
  if (order_to_delete->getBuyOrSell() == 'B') {
    buy_orders_.erase(order_to_delete);
  } else {
    sell_orders_.erase(order_to_delete);
  }
}
float StockMarket::calculateExecutionPrice(
    shared_ptr<const Order> order, shared_ptr<const Order> other_order) {
  float execution_price;
  if (order->getLimitOrMarket() == 'L' &&
      other_order->getLimitOrMarket() == 'L') {
    if (order->getArrivalTime() < other_order->getArrivalTime()) {
      execution_price = order->getLimitPrice();
    } else {
      execution_price = other_order->getLimitPrice();
    }
  }
  else if (order->getLimitOrMarket() == 'L' ||
           other_order->getLimitOrMarket() == 'L') {
    if (order->getLimitOrMarket() == 'L') {
      execution_price = order->getLimitPrice();
    } else {
      execution_price = other_order->getLimitPrice();
    }
  }
  else {
    execution_price = last_traded_price_;
  }
  return execution_price;
}
tuple<bool, shared_ptr<const Order>>
StockMarket::executeOrders(shared_ptr<const Order> order,
                           shared_ptr<const Order> other_order) {
  float execution_price = calculateExecutionPrice(order, other_order);
  int quantity_sold =
      min(order->getTargetQuantity(), other_order->getTargetQuantity());

  int remaining_order_quantity = order->getTargetQuantity() - quantity_sold;
  int remaining_other_order_quantity =
      other_order->getTargetQuantity() - quantity_sold;

  shared_ptr<const Order> buy_order, sell_order;
  tie(buy_order, sell_order) = determineBuyAndSellOrder(order, other_order);
  output_.displayExecution(buy_order->getOrderID(), sell_order->getOrderID(),
                           quantity_sold, execution_price);

  bool remains = false;
  shared_ptr<const Order> updated_order;
  if (remaining_order_quantity != 0) {
    remains = true;
    updated_order =
        make_shared<const Order>(Order(order, remaining_order_quantity));
  }
  if (remaining_other_order_quantity != 0) {
    remains = true;
    updated_order = make_shared<const Order>(
        Order(other_order, remaining_other_order_quantity));
  }
  deleteOrder(order);
  deleteOrder(other_order);
  if (remaining_order_quantity != 0 || remaining_other_order_quantity != 0) {
    addOrder(updated_order);
  }

  last_traded_price_ = execution_price;
  output_.displayPendingOrders(*this);

  return {remains, updated_order};
}

bool StockMarket::checkMatch(shared_ptr<const Order> order,
                             shared_ptr<const Order> other_order) {
  shared_ptr<const Order> buy_order, sell_order;
  tie(buy_order, sell_order) = determineBuyAndSellOrder(order, other_order);

  // invalid if both are limit orders and the buy
  // order price is lower than the sell order
  if (buy_order->getLimitOrMarket() != 'M' &&
      sell_order->getLimitOrMarket() != 'M') {
    if (buy_order->getLimitPrice() < sell_order->getLimitPrice()) {
      return false;
    }
  }

  // invalid if neither are divisible and target quantities aren't equal
  if (!buy_order->getDivisible() && !sell_order->getDivisible()) {
    if (buy_order->getTargetQuantity() != sell_order->getTargetQuantity()) {
      return false;
    }
  }

  // invalid if only one is divisible and the divisible order's
  // target quantity is lower than the indivisible order's
  else if (!buy_order->getDivisible() || !sell_order->getDivisible()) {
    if (buy_order->getDivisible() &&
        buy_order->getTargetQuantity() < sell_order->getTargetQuantity()) {
      return false;
    }
    if (sell_order->getDivisible() &&
        sell_order->getTargetQuantity() < buy_order->getTargetQuantity()) {
      return false;
    }
  }

  return true;
}

void StockMarket::processOrders(shared_ptr<const Order> order) {
  set<shared_ptr<const Order>, ComparePriority>::iterator iterator, end;

  // sets iterator to the opposite Order type's collection
  if (order->getBuyOrSell() == 'B') {
    iterator = sell_orders_.begin();
    end = sell_orders_.end();
  } else {
    iterator = buy_orders_.begin();
    end = buy_orders_.end();
  }

  // finds first (highest-priority) matching order
  bool matched = false;
  shared_ptr<const Order> matched_order;
  while (iterator != end) {
    shared_ptr<const Order> other_order = *iterator;
    if (checkMatch(order, other_order)) {
      matched = true;
      matched_order = other_order;
      break;
    }
    advance(iterator, 1);
  }

  // recursive base condition
  if (!matched) {
    return;
  }

  // perform execution
  bool remains = false;
  shared_ptr<const Order> updated_order;
  tie(remains, updated_order) = executeOrders(order, matched_order);
  if (remains) {
    processOrders(updated_order);
  }
}
shared_ptr<const Order> StockMarket::handleOrderLine(string line) {
  vector<string> components = splitLineBySpace(line);
  string id = components[0];
  char bs = components[1][0];
  char lm = components[2][0];
  char di = components[3][0];
  int tq = stoi(components.end()[-1]);

  float lp;
  if (lm == 'L') {
    lp = stof(components.end()[-2]);
  } else {
    lp = 0;
  }

  shared_ptr<const Order> order =
      make_shared<const Order>(id, bs, lm, di, tq, lp, time_);
  return order;
}
void StockMarket::run(string order_line) {
  shared_ptr<const Order> order = handleOrderLine(order_line);
  addOrder(order);
  output_.displayPendingOrders(*this);
  processOrders(order);
  output_.wait();
}
set<shared_ptr<const Order>, CompareArrivalTime>
StockMarket::getUnexecutedOrders() {
  set<shared_ptr<const Order>, CompareArrivalTime> unexecuted;
  copy(buy_orders_.begin(), buy_orders_.end(),
       inserter(unexecuted, unexecuted.begin()));
  copy(sell_orders_.begin(), sell_orders_.end(),
       inserter(unexecuted, unexecuted.begin()));
  return unexecuted;
}
void StockMarket::handleEndOfDay() {
  set<shared_ptr<const Order>, CompareArrivalTime> unexecuted =
      getUnexecutedOrders();
  output_.logUnexecutedOrders(unexecuted);
}
