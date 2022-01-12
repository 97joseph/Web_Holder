#include <iostream>
#include <map>
#include <vector>

#include "classes.h"
#include "functions.h"
using namespace std;

int main(int argc, char* argv[]) {
  // Read input file and initialize the data
  pair<float, map<int, Order>> data = getData("example.txt");

  float lastTradingPrice = data.first;
  map<int, Order> pendingOrders = data.second;
  int counter = pendingOrders.size() + 1;
  string output;

  // Loop through the list and execute matches
  for (auto it = pendingOrders.begin(); it != pendingOrders.end();) {
    // Print pending orders
    printPendingOrders(pendingOrders, it);

    // Find ma newly added order <FoundMatch?, Remainder?>
    pair<Order, Order> match = findMatch(pendingOrders, it, counter);

    // If match found
    if (!match.first.isNull) {
      Trade trade = handleMatch(pendingOrders, it, match.first, lastTradingPrice, match.second);
      printAndWriteExecution(trade, output);
    }

    it++;
  }

  // Add the unexecuted orders and return result
  printAndWriteNonExecution(pendingOrders, output);
  sendOutput(output);

  return 0;
}

/**
 * TODO
 *
 * 1. Loop through input file and initialize the pending list
 *
 * 2. Loop through the list
 *  - (All searches and prints are done from index 0 - index of current iterator)
 *  - printPendingOrders()
 *    - Loop through the pending orders list and call cout << order.pendingString() (along with whatever
 *      formatting)
 *  - findMatch(Order order (the newly created order)):
 *    - Write a find_if, max, etc. lambda to return a match for [order] depending on order type
 *      - set the lambda based on the type.
 *        - e.g. if [order] is B.O, lambda = [](x..) {[order].price <= x.price() || x.isMarketOrder... &&
 *          type = opposite}
 *      - Make sure all conditions are met
 *    - If order is found ([found order])
 *      - run matchOrder(bO, sO)
 *         - Remove bO and sO from pendingOrders
 *         - If either of them is divisible
 *            - Add new order with the remainder to pendingOrders
 *            - If !newOrderStartPos (first time adding extra orders), set newOrderStartPos to the
 *              arrivalTime of the newly created remainder order.
 *         - Log their executionText and also add the text to the "executions.txt" output file
 *
 * 3. Print the unexecuted Orders
 *  - Loop through the pendingList (which currently holds the unexecuted orders) and print
 *    Order.executionString(unexecuted = true)
 *
 *
 */

/**
 * TUTORIALS
 *
 * Modify map while iterating:
 * https://stackoverflow.com/questions/8234779/how-to-remove-from-a-map-while-iterating-it Writing lambdas
 * for searching containers:
 * https://stackoverflow.com/questions/26766136/how-to-get-min-or-max-element-in-a-vector-of-objects-in-c-based-on-some-field
 */

// EXAMPLE OF HOW TO MODIFY/RESIZE MAP DURING ITERATION
// map<int, string> myMap;
// myMap[1] = "Apple";
// myMap[2] = "Beach";
// myMap[3] = "Cat";
// myMap[4] = "Drink";

// for (auto it = myMap.cbegin(); it != myMap.cend();) {
//   if (it->first == 2) {
//     it = myMap.erase(it);
//     myMap[5] = "New";
//   }
//   cout << it->first << ": " << it->second << endl;
//   ++it;
// }