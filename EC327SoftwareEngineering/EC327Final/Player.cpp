//
//  Player.cpp
//  ProjectPart
//
//  Created by Matthew Guacho on 12/7/22.
//

#include "Player.h"

Player::Player(){
    name = " ";
    playernum = 1;
    score = 0;
    symbol = ' ';
    movecount = 0;
}

void Player::GiveName() {
    name = "CPU";
}

int Player::AIMove(Board* b){
    cout << "CPU thinking very hard..." << endl;
    srand(time(NULL));
    /*for (int c = 0; c < 6; c++){
        for (int r = 0; r < 7; r++){
            if (b->boardptr[c][r]== 0){
                b->boardptr[c][r] = 2;
                b->winCheck();
                if (b->game_over != 0){
                    return c;
                }
            }
        }
    }*/
    int rand_move = (rand() % 7) + 1;
    return rand_move;
}

void Player::GivePiece(char in_char) {
    if(in_char = 'X'){
        symbol = 'O';
    }else{
        symbol = 'X';
    }
}

void Player::Nameask(int playernum){
    string playname;
    cout<< "Whats the name of player "<< playernum << "?";
    cin >> playname;
    name = playname;
}
void Player::Charask(int playernum){
    char pickchar;
    cout<< "Whats the symbol of player "<<playernum<<"?";
    cin >> pickchar;
    if(pickchar == 'x' || pickchar == 'X') {
        symbol = 'X';
    }
    else if(pickchar == 'o' || pickchar == 'O') {
        symbol = 'O';
    }
    else{
        cout << "Invalid" << endl;
    }
}

int Player::GamesWon(int score){
    score++;
    return score;
}
int Player::MakeMove(){
    int column;
    cout<<"Which column would you like to select? ";
    cin >>column;
    if (column <1 || column >7){
        cout<< "Invalid command. Please select between 1 to 7";
        cin>>column;
    }
    return column;

}
Player::~Player(){

}
int Player::getMoveConut(){
    return movecount;
}

char Player::GetChar() {
    return symbol;
}

string Player::GetName() {
    return name;
}