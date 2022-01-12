#pragma once

#include "classes.h"
#include <iostream>
#include <map>
#include <vector>

using namespace std;

/**
 * @brief Read file and return data.
 *
 * This function retrieves the contents of the input files and parses them to the Order list.
 *
 * @param filename The input file name (gotten from the CMD argument)
 * @return pair<float, map<int, Order>>
 */
pair<float, map<int, Order>> getData(string filename);

/**
 * @brief Print pending orders
 *
 * This function prints the Orders in the pendingList starting from position 0 to the position of the
 * iterator. Output string gotten by calling Order.pendingText().
 *
 * @param pendingOrders - List of pending orders
 * @param orderIterator - Position of order (NOTE: Only print from 0 - orderIterator position)
 */
void printPendingOrders(map<int, Order> pendingOrders, map<int, Order>::iterator orderIterator);

/**
 * @brief Find Match for target order
 *
 * This function searches the map from position 0 to the position of the iterator and tries to match it to
 * orders (based on the worksheet conditions).
 *
 * If a match is found, it is returned in the first part of the pair. If the match would require creating a
 * new order i.e. at least one of the orders is divisible (and they satisfy other conditions), then that Order
 * is created and returned in the second part of the pair. NOTE: If remainder is found, increment the counter
 * and assign it as the arrivalTime of the remainder order.
 *
 * @param pendingOrders - List of pending orders
 * @param orderIterator - The target order (NOTE: Only search from 0 - orderIterator position)
 * @return pair<Order, Order> Matched order (if any) and remainder order (if any)
 */
pair<Order, Order> findMatch(map<int, Order> pendingOrders, map<int, Order>::iterator orderIterator,
                             int& counter);

/**
 * @brief Implement the matching process
 *
 * This function handles creating the match and updating the list (and iterator). The function removes the
 * target order (targetOrderIterator->second) and the foundOrder from the pendingOrders list (NOTE: Also
 * update the iterator after removing the targetOrder (see example in the main.cpp file)).
 *
 * Next, the function creates a new Trade with the target and found orders. The execution price is set based
 * on the specifications given in the worksheet.
 *
 * Finally, if there is a remainder order, it is added to the end of the list.
 *
 * @param pendingOrders - Reference to pendingOrders (to allow updating)
 * @param targetOrderIterator - Reference to orderIterator (to allow updating)
 * @param foundOrder - Found order
 * @param lastTradingPrice - The last trading price (Possible trading price)
 * @param remainder - The remainder order (if exists)
 * @return Trade the new trade
 */
Trade handleMatch(map<int, Order>& pendingOrders, map<int, Order>::iterator& targetOrderIterator,
                  Order foundOrder, float lastTradingPrice, Order remainder = {});

/**
 * @brief Print the execution message
 *
 * This function prints the execution message to the console, and also adds the message to the output
 * file "executions.txt"
 *
 * @param trade - Buying and Selling order pair
 * @param output - Reference to output string (to allow updating)
 */
void printAndWriteExecution(Trade trade, string& output);

/**
 * @brief Print the non executed orders
 *
 * This function prints and adds the remaining items in the pendingList as unexecuted.
 *
 * @param pendingOrders - Remaining pending orders
 * @param output - Reference to output string (to allow updating)
 */
void printAndWriteNonExecution(map<int, Order> pendingOrders, string& output);

/**
 * @brief Send output
 *
 * This function writes the output string to the "executions.txt" file and handles any formatting or
 * overwriting issues.
 *
 * @param output - Output string
 */
void sendOutput(string output);