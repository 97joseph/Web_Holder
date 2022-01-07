#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

// Class order
class Order {

private:
	string id;
	char type1;
	char type2;
	char type3;
	double price;
	int quantity;
	bool executed;	// Determines if order is executed or not
	int arrivalTime;

public:

	// Default constructor
	Order() {
		this->id = "";
		this->type1 = '\0';
		this->type2 = '\0';
		this->type3 = '\0';
		this->price = 0.0;
		this->quantity = 0;
		this->executed = false;
		this->arrivalTime = -1;
	}

	// Parameterized constructor
	Order(string id, char type1, char type2, char type3, double price, int quantity, int arrivalTime) {
		this->id = id;
		this->type1 = type1;
		this->type2 = type2;
		this->type3 = type3;
		this->price = price;
		this->quantity = quantity;
		this->executed = false;
		this->arrivalTime = arrivalTime;
	}

	// Copy constructor
	Order(const Order& newOrder) {
		this->id = newOrder.id;
		this->type1 = newOrder.type1;
		this->type2 = newOrder.type2;
		this->type3 = newOrder.type3;
		this->price = newOrder.price;
		this->quantity = newOrder.quantity;
		this->executed = newOrder.executed;
		this->arrivalTime = newOrder.arrivalTime;
	}

	// *************************
	// Getter and setter methods
	// *************************

	string getID() {
		return this->id;
	}

	char getType1() {
		return this->type1;
	}

	char getType2() {
		return this->type2;
	}

	char getType3() {
		return this->type3;
	}

	double getPrice() {
		return this->price;
	}

	int getQuantity() {
		return this->quantity;
	}

	int getArrivalTime() {
		return this->arrivalTime;
	}

	void setID(string id) {
		this->id = id;
	}

	void setType1(char type1) {
		this->type1 = type1;
	}

	void setType2(char type2) {
		this->type2 = type2;
	}

	void setType3(char type3) {
		this->type3 = type3;
	}

	void setPrice(double price) {
		this->price = price;
	}

	void setQuantity(int quantity) {
		this->quantity = quantity;
	}

	void setArrivalTime(int arrivalTime) {
		this->arrivalTime = arrivalTime;
	}

	// ***************
	// Utility methods
	// ***************

	void execute() {
		this->executed = true;
	}

	bool isExecuted() {
		return this->executed;
	}

	void display() {

		if (this->type2 == 'M') {
			cout << left << setw(8) << this->id << "  " << left << setw(8) << this->type2 << "  " << left << setw(8) << this->quantity;
		}

		else {
			cout << left << setw(8) << this->id << "  " << left << setw(8) << this->price << "  " << left << setw(8) << this->quantity;
		}
	}

	// ********************
	// Overloaded operators
	// ********************

	void operator= (const Order& newOrder) {
		this->id = newOrder.id;
		this->type1 = newOrder.type1;
		this->type2 = newOrder.type2;
		this->type3 = newOrder.type3;
		this->price = newOrder.price;
		this->quantity = newOrder.quantity;
		this->executed = newOrder.executed;
		this->arrivalTime = newOrder.arrivalTime;
	}
};

// Function to load data from file
vector<Order> loadData(string filePath, double& lastTradingPrice) {
	ifstream inFile(filePath);
	vector<Order> finalOrders;

	if (inFile) {
		vector<Order> orders;
		const int SIZE = 100;
		char array[SIZE];
		string s;

		inFile.getline(array, SIZE);
		lastTradingPrice = stod(array);

		int arrivalTime = 1;

		while (inFile.getline(array, SIZE)) {
			Order newOrder;

			// Setting arrival time of order
			newOrder.setArrivalTime(arrivalTime);
			arrivalTime = arrivalTime + 1;

			// Picking order ID
			string str = "";

			for (int i = 0; array[i] != 32; i++) {
				str = str + array[i];
			}

			newOrder.setID(str);

			int spaceCount = 0;

			for (int i = 0; array[i] != '\0'; i++) {

				if (array[i] == 32) {
					spaceCount = spaceCount + 1;

					// Picking order type 1
					if (spaceCount == 1) {
						str = "";

						for (int j = i + 1; array[j] != 32; j++) {
							str = str + array[j];
						}

						newOrder.setType1(str[0]);
					}

					// Picking order type 2
					else if (spaceCount == 2) {
						str = "";

						for (int j = i + 1; array[j] != 32; j++) {
							str = str + array[j];
						}

						newOrder.setType2(str[0]);
					}

					// Picking order type 3
					else if (spaceCount == 3) {
						str = "";

						for (int j = i + 1; array[j] != 32; j++) {
							str = str + array[j];
						}

						newOrder.setType3(str[0]);
					}

					// Picking order price or quantity if marketing order
					else if (spaceCount == 4) {
						str = "";

						for (int j = i + 1; array[j] != 32; j++) {
							str = str + array[j];
						}

						if (newOrder.getType2() == 'M') {
							newOrder.setQuantity(stoi(str));
						}

						else {
							newOrder.setPrice(stod(str));
						}
					}

					// Picking order quantity
					else if (spaceCount == 5) {
						str = "";

						for (int j = i + 1; array[j] != 32; j++) {
							str = str + array[j];
						}

						newOrder.setQuantity(stoi(str));
					}
				}
			}

			orders.push_back(newOrder);
		}

		// Sorting all orders on basis of priority
		vector<Order> marketingOrders;
		vector<Order> buyingOrders;
		vector<Order> saleOrders;

		for (int i = 0; i < orders.size(); i++) {

			if (orders[i].getType1() == 'B' && orders[i].getType2() == 'M') {
				marketingOrders.push_back(orders[i]);
			}

			else if (orders[i].getType1() == 'S') {
				saleOrders.push_back(orders[i]);
			}

			else {
				buyingOrders.push_back(orders[i]);
			}
		}

		// Sorting buying orders on basis of price
		bool swap = true;
		
		while (swap) {
			swap = false;
			
			for (int i = 0; i < buyingOrders.size() - 1; i++) {
				
				if (buyingOrders[i].getPrice() < buyingOrders[i + 1].getPrice()) {
					Order temp(buyingOrders[i]);
					buyingOrders[i] = buyingOrders[i + 1];
					buyingOrders[i + 1] = temp;
					swap = true;
				}
			}
		}

		// Coping all orders to final orders vector
		for (int i = 0; i < marketingOrders.size(); i++) {
			finalOrders.push_back(marketingOrders[i]);
		}

		for (int i = 0; i < buyingOrders.size(); i++) {
			finalOrders.push_back(buyingOrders[i]);
		}

		for (int i = 0; i < saleOrders.size(); i++) {
			finalOrders.push_back(saleOrders[i]);
		}

		inFile.close();
	}

	else {
		cout << "Error opening file ..." << endl;
	}

	return finalOrders;
}

// Function to match orders
void matchOrders(vector<Order> orders, double lastTradingPrice) {
	ofstream outFile("executions.txt");

	for (int i = 0; i < orders.size(); i++) {
		vector<int> matchingOrdersIndexes;	// Consists of indexes of matching orders

		// If order is a buying order
		if (orders[i].getType1() == 'B' && !orders[i].isExecuted()) {

			for (int j = 0; j < orders.size(); j++) {

				// If order is a selling order
				if (orders[j].getType1() != 'B' && !orders[j].isExecuted()) {

					// If price of buying order is more than price of selling order or one of them is marketing order
					if ((orders[i].getPrice() >= orders[j].getPrice()) || (orders[i].getType2() == 'M' || orders[j].getType2() == 'M')) {

						// Both orders are indivisible
						if (orders[i].getType3() == 'I' && orders[j].getType3() == 'I') {

							if (orders[i].getQuantity() == orders[j].getQuantity()) {
								matchingOrdersIndexes.push_back(j);
							}
						}

						// If one order is divisible and other is not
						else if (orders[i].getType3() == 'D' && orders[j].getType3() == 'I') {
							
							if (orders[i].getQuantity() >= orders[j].getQuantity()) {
								matchingOrdersIndexes.push_back(j);
							}
						}

						else if (orders[j].getType3() == 'D' && orders[i].getType3() == 'I') {

							if (orders[j].getQuantity() >= orders[i].getQuantity()) {
								matchingOrdersIndexes.push_back(j);
							}
						}

						// If both orders are divisible
						else {
							matchingOrdersIndexes.push_back(j);
						}
					}
				}
			}

			for (int k = 0; k < matchingOrdersIndexes.size(); k++) {

				if (orders[i].isExecuted()) {
					break;
				}

				int targetIndex = matchingOrdersIndexes[k];
				double selectedPrice;

				if (orders[i].getType2() == 'M' && orders[targetIndex].getType2() == 'M') {
					selectedPrice = lastTradingPrice;
				}

				else {

					if (orders[i].getType2() == 'L' && orders[targetIndex].getType2() == 'L') {

						if (orders[i].getArrivalTime() > orders[targetIndex].getArrivalTime()) {
							selectedPrice = orders[targetIndex].getPrice();
						}

						else {
							selectedPrice = orders[i].getPrice();
						}
					}

					else {
						
						if (orders[i].getPrice() > orders[targetIndex].getPrice()) {
							selectedPrice = orders[i].getPrice();
						}

						else {
							selectedPrice = orders[targetIndex].getPrice();
						}
					}
				}

				if (orders[i].getQuantity() > orders[targetIndex].getQuantity()) {
					int remainingQuantity = orders[i].getQuantity() - orders[targetIndex].getQuantity();
					orders[i].setQuantity(orders[targetIndex].getQuantity());
					Order newOrder(orders[i].getID(), orders[i].getType1(), orders[i].getType2(), orders[i].getType3(), orders[i].getPrice(), remainingQuantity, orders[i].getArrivalTime());
					orders.push_back(newOrder);
				}

				else if (orders[targetIndex].getQuantity() > orders[i].getQuantity()) {
					int remainingQuantity = orders[targetIndex].getQuantity() - orders[i].getQuantity();
					orders[targetIndex].setQuantity(orders[i].getQuantity());
					Order newOrder(orders[targetIndex].getID(), orders[targetIndex].getType1(), orders[targetIndex].getType2(), orders[targetIndex].getType3(), orders[targetIndex].getPrice(), remainingQuantity, orders[targetIndex].getArrivalTime());
					orders.push_back(newOrder);
				}

				cout << "order " << orders[i].getID() << " " << orders[i].getQuantity() << " shares purchased at price " << selectedPrice << endl;
				cout << "order " << orders[targetIndex].getID() << " " << orders[targetIndex].getQuantity() << " shares sold at price " << selectedPrice << endl;
				outFile << "order " << orders[i].getID() << " " << orders[i].getQuantity() << " shares purchased at price " << selectedPrice << endl;
				outFile << "order " << orders[targetIndex].getID() << " " << orders[targetIndex].getQuantity() << " shares sold at price " << selectedPrice << endl;
				orders[i].execute();
				orders[targetIndex].execute();
			}
		}
	}

	// Displaying unexecuted orders
	for (int i = 0; i < orders.size(); i++) {
		
		if (!orders[i].isExecuted()) {
			cout << "order " << orders[i].getID() << " " << orders[i].getQuantity() << " shares unexecuted" << endl;
			outFile << "order " << orders[i].getID() << " " << orders[i].getQuantity() << " shares unexecuted" << endl;
		}
	}

	outFile.close();
}

// Function to display orders
void displayOrders(vector<Order> orders, double lastTradingPrice) {
	vector<Order> buyingOrders;
	vector<Order> saleOrders;

	for (int i = 0; i < orders.size(); i++) {

		if (orders[i].getType1() == 'B') {
			buyingOrders.push_back(orders[i]);
		}

		else {
			saleOrders.push_back(orders[i]);
		}
	}

	cout << "Last trading price: " << lastTradingPrice << endl;
	cout << "Buy\t\t\t\tSell" << endl;
	cout << "---------------------------------------------------------" << endl;

	int i = 0;
	int j = 0;

	while (true) {

		if (i == buyingOrders.size() && j == saleOrders.size()) {
			break;
		}

		if (i < buyingOrders.size()) {
			buyingOrders[i].display();
			cout << "\t";
			i = i + 1;
		}

		if (j < saleOrders.size()) {
			saleOrders[j].display();
			j = j + 1;
		}

		cout << endl;
	}
}

int main(int argc, char* argv[]) {
	cout << fixed << setprecision(2);
	double lastTradingPrice = 0.0;
	vector<Order> orders = loadData(argv[1], lastTradingPrice);
	displayOrders(orders, lastTradingPrice);
	matchOrders(orders, lastTradingPrice);
	return 0;
}