//
// Created by Sebbie on 12/9/2022.
//

#include <iostream>
#include <string>
#include "Board.h"
#include "Visual.h"
#include "Point2d.h"
#include "Player.h"

using namespace std;

int main(){
    Visual V;
    Board B;
    int move;
    int answer_check;
    int Select; //Choice to face the CPU, PlayerTwo, or Quitting the program

    do {
        V = Visual();
        B = Board();
        V.Startup();
        move = 0;
        answer_check = 4;

        cout << "Select your choice (1-3)" << endl;
        cout << "1: 1P VS. CPU" << endl;
        cout << "2: 1P VS. 2P" << endl;
        cout << "3: Exit Game" << endl;
        cin >> Select;

        if (Select == 1) {
            Player Player1;
            Player1.Nameask(1);
            Player1.Charask(1);
            Player CPU;
            CPU.GiveName();
            CPU.GivePiece(Player1.GetChar());

            V.Draw();
            do {
                move = Player1.MakeMove();
                B.placePiece(move, Player1.GetChar());
                V.Plot(B.getMove(move), Player1.GetChar());
                V.Draw();
                if(B.winCheck() == 1){
                    cout << Player1.GetName() << " wins!" << endl;
                }
                if(B.winCheck() == 2){
                    cout << CPU.GetName() << " wins!" << endl;
                }
                if(B.winCheck() == 3){
                    cout << Player1.GetName() << " and " << CPU.GetName() << " TIE!" << endl;
                }
                move = CPU.AIMove(&B);
                B.placePiece(move,CPU.GetChar());
                V.Plot(B.getMove(move), CPU.GetChar());
                V.Draw();
                if(B.winCheck() == 1){
                    cout << Player1.GetName() << " wins!" << endl;
                }
                if(B.winCheck() == 2){
                    cout << CPU.GetName() << " wins!" << endl;
                }
                if(B.winCheck() == 3){
                    cout << Player1.GetName() << " and " << CPU.GetName() << " TIE!" << endl;
                }
            }while(B.winCheck() == 0);
        }
        if(Select == 2) {
            Player Player1;
            Player1.Nameask(1);
            Player1.Charask(1);
            Player Player2;
            Player2.Nameask(2);
            Player2.GivePiece(Player1.GetChar());

            V.Draw();
            do {
                cout << Player1.GetName() << ", ";
                move = Player1.MakeMove();
                B.placePiece(move, Player1.GetChar());
                V.Plot(B.getMove(move), Player1.GetChar());
                V.Draw();
                if(B.winCheck() == 1){
                    cout << Player1.GetName() << " wins!" << endl;
                    break;
                }
                if(B.winCheck() == 2){
                    cout << Player2.GetName() << " wins!" << endl;
                    break;
                }
                if(B.winCheck() == 3){
                    cout << Player1.GetName() << " and " << Player2.GetName() << " TIE!" << endl;
                    break;
                }
                cout << Player2.GetName() << ", ";
                move = Player2.MakeMove();
                B.placePiece(move, Player2.GetChar());
                V.Plot(B.getMove(move), Player2.GetChar());
                V.Draw();
                if(B.winCheck() == 1){
                    cout << Player1.GetName() << " wins!" << endl;
                    break;
                }
                if(B.winCheck() == 2){
                    cout << Player2.GetName() << " wins!" << endl;
                    break;
                }
                if(B.winCheck() == 3){
                    cout << Player1.GetName() << " and " << Player2.GetName() << " TIE!" << endl;
                    break;
                }
            } while (B.winCheck() == 0);
        }

    }while(Select != 3);
}
