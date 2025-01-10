#include <iostream>

using namespace std;

int main(){
    int *a;
    {
    int b = 4;
    a = &b;
    }
    cout << *a << endl;
    return 0;
}