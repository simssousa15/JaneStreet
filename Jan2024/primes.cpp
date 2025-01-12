#include <iostream>
#include <set>

using namespace std;

int main(){
    for(int i = 999'999'999; i > 99'999'999; i--){
        bool is_prime = true;
        for(int j = 2; j*j <= i; j++){
            if(i % j == 0){
                is_prime = false;
                break;
            }
        }
        if(is_prime){
            string num = to_string(i);
            if(num.size() == 8){ num = "0" + num; }

            set<char> s;
            for(auto c: num){
                s.insert(c);
            }
            if(s.size() != num.size()){ continue;; }

            if(num[1] == '2'){ cout << num << endl;}
            else if(num[2] == '0'){ cout << num << endl;}
            else if(num[3] == '2'){ cout << num << endl;}
            else if(num[4] == '0'){ cout << num << endl;}
            else if(num[5] == '2'){ cout << num << endl;}
            else if(num[6] == '5'){ cout << num << endl;}
            else if(num[7] == '2'){ cout << num << endl;}
            else if(num[8] == '5'){ cout << num << endl;}
        }
    }
    return 0;
}