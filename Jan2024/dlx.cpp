#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <queue>
#include <bits/stdc++.h>
#include <chrono>

#define N 9

using namespace std;

/*
    Useful links:
    - https://www.ocf.berkeley.edu/~jchu/publicportal/sudoku/sudoku.paper.html
    - https://code.google.com/archive/p/narorumo/wikis/SudokuDLX.wiki
    - https://shivankaul.com/blog/sudoku
    - https://github.com/ShivanKaul/Sudoku-DLX
*/
// 1. Find all the solutions to the sudoku (1-9 digits)
/*
Startign Sudoku

 ---------------------
 _ _ _ | _ _ _ | _ 2 _
 _ _ _ | _ _ _ | _ _ 5
 _ 2 _ | _ _ _ | _ _ _
 ---------------------
 _ _ 0 | _ _ _ | _ _ _
 _ _ _ | _ _ _ | _ _ _
 _ _ _ | 2 _ _ | _ _ _
 ---------------------
 _ _ _ | _ 0 _ | _ _ _
 _ _ _ | _ _ 2 | _ _ _
 _ _ _ | _ _ _ | 5 _ _
 ---------------------

----------------------
 _ _ _ | _ _ _ | _ 2 _
 _ _ _ | _ 2 _ | _ _ 5
 _ 2 _ | _ _ _ | _ _ _
 ---------------------
 _ _ 0 | _ _ _ | _ _ _
 _ _ _ | _ _ _ | _ _ _
 _ _ _ | 2 _ _ | _ _ _
 ---------------------
 _ _ _ | _ 0 _ | _ _ _
 _ _ _ | _ _ 2 | _ _ _
 _ _ _ | _ _ _ | 5 _ _
 ---------------------
 */

/*
0: (0, 1) (1, 3) (2, 7) (3, 2) (4, 5) (5, 8) (6, 4) (7, 6) (8, 0) 
2: (0, 7) (1, 4) (2, 1) (3, 0) (4, 6) (5, 3) (6, 2) (7, 5) (8, 8) 
5: (0, 2) (1, 8) (2, 5) (3, 7) (4, 1) (5, 4) (6, 0) (7, 3) (8, 6)

*/
/*
    **** Generate toriodal doubly linked list ****
*/

// root col_id = 0
struct Node
{
    int col_id = -1;
    int size = -1;
    Node* col = nullptr;
    Node* up = nullptr;
    Node* down = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;

    int i, j, val;
};

// cells with these values can be changed
vector<int> not_fixed = {1, 3, 4, 6, 7, 8};

int rand_int(int min, int max){
    return min + (rand() % (max - min + 1));
}

class MemoGcd{
    private:
        unordered_map<string, pair<long, int>> memo;
        int max_size = 10'000;
        long long hits = 0;
        long long total = 0;

        string _concat (vector<string> full_rows){
            string key = "";
            for(auto row : full_rows){
                key += row + ",";
            }
            return key;
        }

    public:
        // MemoGcd(){ keys.reserve(max_size); }

        pair<long, int> calc(vector<string> full_rows){
            string key = _concat(full_rows);

            if(memo.find(key) != memo.end()){
                hits++;
                return memo[key];
            }
            if(memo.size() > max_size){
                auto itr = memo.begin();
                advance(itr, rand_int(0, max_size));
                memo.erase(itr);
            }
            total++;
            if( total%10000 == 0 ){ cout << "Memo Hits: " << (double)hits/total << "%" << endl; }

            long max_gcd = INT_MAX;
            int missing_val = 9;
            //compute gcd for grid with default values
            for(auto row : full_rows){
                if(max_gcd == INT_MAX){ max_gcd = stol(row); }
                else{ max_gcd = __gcd(max_gcd, stol(row)); }
            }

            //try to change the values of the grid to improve the gcd
            for(auto n : not_fixed){
                long new_gcd = INT_MIN;
                for(auto row : full_rows){
                    string num = row;
                    for(auto &c : num){ c == n + '0' ? c='9' : c=c; }
                    if(new_gcd == INT_MIN){ new_gcd = stol(num); }
                    else{ new_gcd = __gcd(new_gcd, stol(num)); }
                }
                if(new_gcd > max_gcd){
                    max_gcd = new_gcd;
                    missing_val = n;
                }
            }

            memo[key] = {max_gcd, missing_val};
            return {max_gcd, missing_val};
        }
};

class Solution{
    private:
        vector<Node*> sol;
        vector<vector<int>> grid;
        vector<int> in_row_ctr;
        int missing_val = 9;
        MemoGcd gcd = MemoGcd();

        vector<string> _get_full_rows(){
            vector<string> full_rows;
            for(auto row : grid){
                string num = "";
                for(auto n : row){
                    if(n == -1){ break; }
                    num += to_string(n);
                }
                if(num.size() == 9){
                    full_rows.push_back(num);
                }
            }
            return full_rows;
        }

    public:
        Solution(){
            for(int i=0; i<N*N; i++){ sol.push_back(nullptr); }
            for(int i=0; i<N; i++){
                vector<int> temp;
                for(int j=0; j<N; j++){
                    temp.push_back(-1);
                }
                grid.push_back(temp);
            }
            for(int i=0; i<N; i++){ in_row_ctr.push_back(0); }
        }

        void add(int k, Node* row){
            grid[row->i][row->j] = row->val;
            in_row_ctr[row->i]++;
            sol[k] = row;
        }

        void remove(int k){ 
            grid[sol[k]->i][sol[k]->j] = -1;
            int this_row_ctr = --in_row_ctr[sol[k]->i];
            sol[k] = nullptr;
        }

        void printf(){
            for(auto s: grid){
                for(auto ss: s){
                    if(ss == missing_val){ cout << "9"; }
                    else{ cout << ss; }
                }
                cout << endl;
            }
        }

        long get_gcd(){

            vector<string> full_rows = _get_full_rows();
            if(full_rows.size() == 0){ return INT_MAX; }
            if(full_rows.size() == 1){ return stol(full_rows[0]); }

            auto g = gcd.calc(full_rows);
            missing_val = g.second;
            return g.first;
        }

        vector<vector<int>> get_grid(){
            return grid;
        }   
};

Solution sol;
vector<vector<int>> best_grid;
long best_gcd = 0;
int small_k = INT_MAX;
auto start_time = chrono::high_resolution_clock::now();
auto last_k = start_time;

Node* generate_list(vector<vector<bool>>grid, vector<vector<int>> easy){
    Node* root = new Node();
    root->col_id = 0;
    root->left = root;

    // initialize col nodes
    vector<Node*> last_of_col;
    Node* last_col = root;
    for(int i=0; i<grid[0].size(); i++){
        Node* col = new Node();
        col->col_id = i+1;
        col->size = 0;
        col->col = col;
        // right
        col->right = root;
        root->left = col;
        // left
        col->left = last_col;
        last_col->right = col;
        last_col = col;
        //up
        col->up = col;
        //down
        col->down = col;

        // store end of col
        last_of_col.push_back(col);
    }

    auto col_nodes = last_of_col;
    //filling the grid
    for(int i=0; i<grid.size(); i++){
        Node* last_of_row = nullptr;
        for(int j=0; j<grid[0].size(); j++){
            if(grid[i][j]){
                Node* tmp = new Node();
                tmp->i = easy[i][0];
                tmp->j = easy[i][1];
                tmp->val = easy[i][2];
                //horizontal
                if(last_of_row){
                    tmp->left = last_of_row;
                    tmp->right = last_of_row->right;
                    tmp->left->right = tmp;
                    tmp->right->left = tmp;
                }else{
                    tmp->left = tmp;
                    tmp->right = tmp;
                }
                last_of_row = tmp;

                //vertical
                tmp->up = last_of_col[j];
                tmp->down = last_of_col[j]->down;
                tmp->up->down = tmp;
                tmp->down->up = tmp;
                last_of_col[j] = tmp;

                tmp->col = col_nodes[j];
                col_nodes[j]->size += 1;

            }
        }
    }


    return root;
}

void list_check(Node* h){
    cout << "start list check..." << endl;
    for(Node* c = h->right; c != h; c = c->right){
        for(Node* r = c->down; r != c; r = r->down){
            int ctr = 1;
            for(Node* n = r->right; n != r; n = n->right){
                ctr++; 
            }
            if(ctr != 4){ cout << "row.size() != 4" << endl; }
        }
    }
    cout << "right down ok" << endl;
    for(Node* c = h->left; c != h; c = c->left){
        for(Node* r = c->up; r != c; r = r->up){
            int ctr = 1;
            for(Node* n = r->left; n != r; n = n->left){
                ctr++;
            }
            if(ctr != 4){ cout << "row.size() != 4" << endl; }
        }
    }
    cout << "left up ok" << endl;
}

void cover( Node* n ){
    Node* c = n->col;
    c->right->left = c->left;
    c->left->right = c->right;

    
    for( Node* row = c->down; row != c; row = row->down ){
        for( Node* rightNode = row->right; rightNode != row; rightNode = rightNode->right ){
            rightNode->up->down = rightNode->down;
            rightNode->down->up = rightNode->up;
            rightNode->col->size--;
        }
    }
}

void uncover( Node* n ){
    Node* c = n->col;
    for( Node* row = c->up; row != c; row = row->up){
        for( Node* leftNode = row->left; leftNode != row; leftNode = leftNode->left){
            leftNode->col->size++;
            leftNode->up->down = leftNode;
            leftNode->down->up = leftNode;
        }
    }

    c->right->left = c;
    c->left->right = c;
}

Node* getSmallest( Node* h){
    Node* c = h->right;
    int min_size = c->size;
    Node* min_col = c;
    
    while(c->right != h){
        c = c->right;
        if(c->size < min_size){
            min_size = c->size;
            min_col = c;
        }
    }

    if(min_col == h){ cout << "Error getSmallest: min_col == h" << endl; }

    return min_col;
}

void search(int k, Node* h){
    if(h->right == h){
        /* int gcd = sol.get_gcd();
        if(gcd > best_gcd){
            best_gcd = gcd;
            best_grid = sol.get_grid();
            cout << "### Better sol found ###" << endl;
            cout << best_gcd << endl;
            sol.printf();
            cout << "------------------------" << endl;
        } */
        return;
    }else{
        Node* c = getSmallest(h);
        cover(c);

        for( Node* row = c->down; row != c; row = row->down){
            sol.add(k, row);

            for( Node* rightNode = row->right; rightNode != row; rightNode = rightNode->right){
                cover(rightNode);
            }

            // if(sol.get_gcd() > best_gcd){ search(k+1, h); }
            search(k+1, h);       
            sol.remove(k);

            for( Node* leftNode = row->left; leftNode != row; leftNode = leftNode->left ){
                uncover(leftNode);
            }
        }
        uncover(c);
        
        if(k < small_k){
            auto now = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::seconds>(now - last_k);
            auto total_duration = chrono::duration_cast<chrono::seconds>(now - start_time);
            small_k = k;
            cout << "k: " << k << " diff: " << duration.count() << "s" << " total: " << total_duration.count() << "s" <<  endl;
            last_k = now;
        }

        return;
    }
}



int main(){

    // Each Cols represents a constraint
    // *** Four Constrains ***
    // 1. Only one number per cell (cols * rows)
    // 2. Only one digit per row (digits * rows)
    // 3. Only one digit per col (digits * cols)
    // 4. Only one digit per square (digits * regions)
    int rows_size = pow(N, 2) * 4;
    vector<bool> row (rows_size, false);

    // each row represets a possible number in a cell
    // possible digits
    vector<vector<bool>> grid;
    vector<vector<int>> easy;
    for(int d=0; d<9; d++){
        for(int r=0; r<9; r++){
            for(int c=0; c<9; c++){
                // skip cells already assigned
                if(r == 0 && c == 7 && d!= 2){ continue; }
                if(r == 1 && c == 8 && d!= 5){ continue; }
                if(r == 2 && c == 1 && d!= 2){ continue; }
                if(r == 3 && c == 2 && d!= 0){ continue; }
                if(r == 5 && c == 3 && d!= 2){ continue; }
                if(r == 6 && c == 4 && d!= 0){ continue; }
                if(r == 7 && c == 5 && d!= 2){ continue; }
                if(r == 8 && c == 6 && d!= 5){ continue; }
                //forced assignments
                if(r == 1 && c == 4 && d!= 2){ continue; }

                easy.push_back({r, c, d});

                auto temp = row;
                // constrain 1
                temp[r*9 + c] = true;
                // constrain 2
                temp[pow(N, 2) + r*9 + d] = true;
                // constrain 3
                temp[pow(N, 2)*2 + c*9 + d] = true;
                // constrain 4
                int reg = r - r%3 + c/3;
                temp[pow(N, 2)*3 + reg*9 + d] = true;

                grid.push_back(temp);
            }
        }
    }

    /* 
    cout << "row: " << grid.size() << endl;
    cout << "cols: " << grid[0].size() << endl; 
    */

    Node* h = generate_list(grid, easy);
    list_check(h);

    search(0, h);
    
    
    return 0;
}