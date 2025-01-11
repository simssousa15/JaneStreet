#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <queue>
#include <bits/stdc++.h>

#define N 9

using namespace std;

/*
    Useful links:
    - https://www.ocf.berkeley.edu/~jchu/publicportal/sudoku/sudoku.paper.html
    - https://code.google.com/archive/p/narorumo/wikis/SudokuDLX.wiki
    - https://shivankaul.com/blog/sudoku
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
};

Node generate_list(vector<vector<bool>>grid){
    Node root;
    root.col_id = 0;
    root.left = &root;

    // initialize col nodes
    vector<Node*> last_of_col;
    Node* last_col = &root;
    for(int i=0; i<grid[0].size(); i++){
        Node col;
        col.col_id = i+1;
        col.size = 0;
        col.col = &col;
        // right
        col.right = &root;
        root.left = &col;
        // left
        col.left = last_col;
        last_col->right = &col;
        last_col = &col;
        //up
        col.up = &col;
        //down
        col.down = &col;

        // store end of col
        last_of_col.push_back(&col);
    }

    auto col_nodes = last_of_col;
    //filling the grid
    for(int i=0; i<grid.size(); i++){
        Node* last_of_row = nullptr;
        for(int j=0; j<grid[0].size(); j++){
            if(grid[i][j]){
                Node tmp;
                //horizontal
                if(last_of_row){
                    tmp.left = last_of_row;
                    tmp.right = last_of_row->right;
                    last_of_row->right = &tmp;
                    last_of_row = &tmp;
                }else{
                    tmp.left = &tmp;
                    tmp.right = &tmp;
                    last_of_row = &tmp;
                }
                last_of_row = &tmp;

                //vertical
                tmp.up = last_of_col[j];
                tmp.down = last_of_col[j]->down;
                last_of_col[j]->down = &tmp;
                last_of_col[j] = &tmp;

                tmp.col = col_nodes[j];
                col_nodes[j]->size += 1;

            }
        }
    }


    return root;
}

void cover( Node* n ){
    Node* c = n->col;
    c->right->left = c->left;
    c->left->right = c->right;

    for( Node* row = c->down; row != c; row = row->down ){
        for( Node* rightNode = row->right; rightNode != row; rightNode = rightNode->right ){
            rightNode->up->down = rightNode->down;
            rightNode->down->up = rightNode->up; 
        }
    }
}

void uncover( Node* n ){
    Node* c = n->col;
    for( Node* row = c->up; row != c; row = row->up){
        for( Node* leftNode = row->left; leftNode != row; leftNode = leftNode->right){
            leftNode->up->down = leftNode->down;
            leftNode->down->up = leftNode->up;
        }
    }

    c->right->left = c->left;
    c->left->right = c->right;
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

    return min_col;
}

// not done
search(int k, Node* h){
    if(h->right == h){
        return;
    }else{
        Node* c = getSmallest(h);
        cover(c);

        for( Node* row = c->down; row != c; row = row->down){
            for( Node* rightNode = row->right; rightNode != row; rightNode = rightNode->right){
                cover(rightNode->col);
            }

            search(k+1, h);
            c = row->col;
            
            for( Node* leftNode = row->left; leftNode != row; leftNode = leftNode->left ){
                uncover(leftNode->col);
            }
        }
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

    // search
    
    
    return 0;
}