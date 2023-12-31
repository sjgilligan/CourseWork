//
// Created by Sebbie on 12/6/2022.
//

#include "Visual.h"
#include "Board.h"
#include "Point2d.h"
#include <iostream>

using namespace std;

Visual::Visual() {
    for(int j = rows - 1; j > 0; j--){
        for(int i = 0; i <= columns; i++){
            grid[i][j] = ' ';
        }
    }
}

void Visual::Startup() {
    cout << "Welcome to..." << endl;
    cout << "        {}       {}{}      {}{}   {}   {}{}   {}   {}{}{}       {}   {}{}{}{}{}       {}    {}" << endl;
    cout << "      {}       {}    {}    {} {}  {}   {} {}  {}   {}         {}         {}           {}    {}" << endl;
    cout << "    {}         {}    {}    {}  {} {}   {}  {} {}   {}       {}           {}           {}    {}" << endl;
    cout << "    {}         {}    {}    {}   {}{}   {}   {}{}   {}{}{}   {}           {}     {}{}  {}{}{}{}" << endl;
    cout << "      {}       {}    {}    {}    {}}   {}    {}}   {}         {}         {}                 {}" << endl;
    cout << "        {}       {}{}      {}     {}   {}     {}   {}{}{}       {}       {}                 {}" << endl;
    cout << "                                       Created by: ...                                        " << endl;
    cout << endl;
}

void Visual::Plot(Point2d p, char piece) {
    grid[p.x - 1][p.y] = piece;
}

void Visual::Draw(){
    for(int j = rows - 1; j > 0; j--){
        for(int i = 0; i <= columns; i++){
            if(grid[i][j] == ' '){
                cout << "| ";
            }else {
                cout << "|" << grid[i][j];
            }
        }
        cout << "|" << endl;
    }
}


