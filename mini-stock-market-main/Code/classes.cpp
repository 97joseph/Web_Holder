#include "classes.h"

#include <cmath>
#include <iostream>
#include <stack>
#include <string>
using namespace std;

// ========================================================================================================================================
//    ORDER
// ========================================================================================================================================
/**
 * @brief Initialize the order.
 * This constructor takes a line of the order input string and initializes the data members.
 *
 * @param orderInputStr order input string e.g. "ord001 B L I 4.25 100"
 * @param arrivalTime Arrival time represented by int. (preferably the counter var from the loop)
 */
Order::Order(string orderInputStr, int arrivalTime) {
  // TODO: IMPLEMENT ME
}

/**
 * @brief Initialize the order.
 * This constructor initializes an order with its values
 */
Order::Order(int aT, string orderID, string t, string mT, bool div, int qty, float price)
    : arrivalTime(aT), id(orderID), type(t), isDivisible(div), targetPrice(price), quantity(qty) {}

/**
 * @brief Initialize a null order.
 */
Order::Order() : isNull(true) {}

/**
 * @brief Return the pending text string.
 * This function returns a string that matches the pending list string displayed on the console.
 *
 * @return Pending string. e.g. "ord002 4.50     50"
 */
string Order::pendingText() {
  // TODO: IMPLEMENT ME
  return "--";
}

/**
 * @brief Return the execution text string.
 * This function returns a string that matches the specified executions.txt text.
 *
 * @param unexecuted if order is unexecuted (return unexecuted text if true e.g. "order ord002 100 shares
 * unexecuted")
 * @return Execution string. e.g. "order ord001 100 shares purchased at price 4.25"
 */
string Order::executionText(float executionPrice, bool unexecuted) {
  // TODO: IMPLEMENT ME
  return "--";
}

// ========================================================================================================================================
//    TRADE
// ========================================================================================================================================
/**
 * @brief Construct a new Trade object and initialized the order pair
 *
 * @param o1 - First order
 * @param o2 - Second order
 */
Trade::Trade(Order& o1, Order& o2) : buyOrder(o1), sellOrder(o2) {
  if (o2.type == "B") {
    buyOrder = o2;
    sellOrder = o1;
  }
}

/**
 * @brief Return execution string of the order pair, simply by returning the execution string of the
 * trade pair.
 *
 * @return Execution string of the pair
 */
string Trade::executionText() {
  return buyOrder.executionText(executionPrice) + "\n" + sellOrder.executionText(executionPrice);
}