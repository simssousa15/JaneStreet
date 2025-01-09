#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <queue>
#include <bits/stdc++.h>

#define N 9

using namespace std;

/*
Startign Sudoku

 ---------------------
 _ _ _ | _ _ _ | _ 2 _
 _ _ _ | _ _ _ | _ _ _
 _ 2 _ | _ _ _ | _ _ _
 ---------------------
 _ _ 0 | _ _ _ | _ _ _
 _ _ _ | _ _ _ | _ _ 5
 _ _ _ | 2 _ _ | _ _ _
 ---------------------
 _ _ _ | _ 0 _ | _ _ _
 _ _ _ | _ _ 2 | _ _ _
 _ _ _ | _ _ _ | 5 _ _
 ---------------------
 */

// This Sudoku is from 0-8
class Sudoku{
    private:
        vector<vector<bool>> rows;
        vector<vector<bool>> cols;
        vector<vector<bool>> squares;

        vector<vector<int>> sudo;

        int nxt_i=0, nxt_j=0;
        bool full = false;

    public:
        Sudoku(){
            vector<bool> l (N, false);
            vector<int> n(N, -1);
            for(int i=0; i<9; i++){
                rows.push_back(l);
                cols.push_back(l);
                squares.push_back(l);
                sudo.push_back(n);
            }

            // initial values
            bool init = 
            insert(0, 7, 2) &&
            insert(1, 8, 5) &&
            insert(2, 1, 2) &&
            insert(3, 2, 0) &&
            insert(5, 3, 2) &&
            insert(6, 4, 0) &&
            insert(7, 5, 2) &&
            insert(8, 6, 5);   

            if(!init){ cout << "Error with initialization!" << endl; }      
        }

        bool operator<(const Sudoku& other) const{
            return (nxt_i * 9 + nxt_j) < (other.nxt_i * 9 + other.nxt_j);
        }

        bool insert(int i, int j, int val){
            if(rows[i][val]){ return false; }
            else if(cols[j][val]){ return false; }
            else if(squares[i - i%3 + j/3][val]){ return false; }
            else if(sudo[i][j] != -1){ return false; }
            else{
                rows[i][val] = true;
                cols[j][val] = true;
                squares[i - i%3 + j/3][val] = true;
                sudo[i][j] = val;

                return true;
            }
        }

        void inc_nxt(){
            while(sudo[nxt_i][nxt_j] != -1){
                nxt_j++;
                if(nxt_j == 9){
                    nxt_j = 0;
                    nxt_i++;
                    if(nxt_i == 9){
                        full = true;
                        return;
                    }
                }
            }
        }

        bool insert_nxt(int val){
            bool b = insert(nxt_i, nxt_j, val);
            if(b){ inc_nxt(); }
            return b;
        }

        bool is_full(){
            return full;
        }

        void printf(){
            cout << "########################" << endl;
            for(auto r : sudo){
                for(auto c : r){
                    if(c == -1){
                        cout << "_";
                    }else{
                        cout << c;
                    }
                    cout << " ";
                }
                cout << endl;
            }
            cout << "########################" << endl;
        }

        int get_size(){
            return (nxt_i * 9 + nxt_j);
        }

};


int main(){

    time_t start, end;
    time(&start);
    ios_base::sync_with_stdio(false);

    Sudoku s;
    priority_queue<Sudoku> q;
    q.push(s);
    
    int total_solutions = 0;
    while(q.size()){
        s = q.top();
        q.pop();

        if(s.is_full()){
            if(total_solutions % 10000 == 0){
                cout << "-----------------" << endl;
                cout << "Queue size: " << q.size() << endl;
                cout << "Solutions: " << total_solutions+1 << endl;
                s.printf();
            }
            total_solutions++;
            continue;;
        }

        for(int i=0; i<9; i++){
            Sudoku temp = s;
            if(temp.insert_nxt(i)){
                q.push(temp);
            }
        }
    }
    
    time(&end);

    cout << "Total solutions: " << total_solutions << endl;
    double time_taken = double(end - start);
    cout << "Total time: " << fixed << time_taken << setprecision(5) << " sec" << endl;

    return 0;
}