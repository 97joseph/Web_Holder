#include "Market.h"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  ofstream output_file("executions.txt");
  ifstream input_file(argv[1]);
  cout << fixed << setprecision(2);
  output_file << fixed << setprecision(2);

  string text_buffer;
  getline(input_file, text_buffer);
  StockMarket stockMarket(stof(text_buffer), output_file);
  getline(input_file, text_buffer);
  do {
    stockMarket.run(text_buffer);
  } while (getline(input_file, text_buffer));

  stockMarket.handleEndOfDay();

  input_file.close();
  output_file.close();
}
