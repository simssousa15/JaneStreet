#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <queue>
#include <bits/stdc++.h>

#define N 9

using namespace std;

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
 */


/*
    **** Generate toriodal doubly linked list ****
*/

// root col_id = 0
struct Node
{
    int col_id = -1;
    Node* up = nullptr;
    Node* down = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
};

Node generate_list(vector<vector<bool>>grid){
    Node root;
    root.col_id = 0;
    root.left = &root;

    vector<Node> last_of_
    Node last_col = root;
    for(int i=0; i<grid.size(); i++){
        Node col;
        col.col_id = i+1;
        // right
        col.right = &root;
        root.left = &col;
        // left
        col.left = &last_col;
        last_col.right = &col;
        last_col = col;
    }


    return root;
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

    auto l = generate_list(grid);
    
    
    return 0;
}