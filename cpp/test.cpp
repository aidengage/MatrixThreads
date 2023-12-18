#include <iostream>
#include <thread>

using namespace std;

void hello() {
    cout << "hello thread test" << endl;
}

void printNum(int num) {
    cout << num << endl;
}

int main() {
    int x = 32;
    thread t1(hello);
    thread t2(printNum, x);
    t1.join();
    t2.join();
}