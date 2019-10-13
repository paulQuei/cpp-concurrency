#include <iostream>
#include <thread>

using namespace std;

int a = 0;
int b = 0;
bool start = false;

void increment() {
  while (!start) {
    // waiting...
  }

  for (int i = 0; i < 100; i++) {
    a++;
    a++;
    a++;
    b++;
    b++;
    b++;
    cout << "a=" << a << ", b=" << b << endl;
  }
}

int main() {

  thread t1(increment);
  thread t2(increment);
  thread t3(increment);

  start = true;
  t1.join();
  t2.join();
  t3.join();

  return 0;
}