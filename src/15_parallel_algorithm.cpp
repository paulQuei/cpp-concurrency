#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

void generateRandomData(vector<double>& collection, int size) {
  random_device rd;
  mt19937 mt(rd());
  uniform_real_distribution<double> dist(1.0, 100.0);
  for (int i = 0; i < size; i++) {
    collection.push_back(dist(mt));
  }
}

int main() {
  vector<double> collection;
  generateRandomData(collection, 10e6);

  vector<double> copy1(collection);
  vector<double> copy2(collection);

  auto start_time = chrono::steady_clock::now();

  sort(copy1.begin(), copy1.end());
  auto current_time = chrono::steady_clock::now();
  auto duration = chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count();
  cout << "Default sort consuming " << duration << "ms." << endl;

  start_time = chrono::steady_clock::now();
  sort(execution::par_unseq, copy2.begin(),copy2.end());
  current_time = chrono::steady_clock::now();
  duration = chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count();
  cout << "Parallel sort consuming " << duration << "ms." << endl;
}