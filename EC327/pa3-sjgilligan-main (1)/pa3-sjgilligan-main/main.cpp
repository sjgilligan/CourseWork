#include "Point2D.h"
#include "Vector2D.h"
#include "GameObject.h"
#include "Building.h"
#include "PokemonCenter.h"
#include "PokemonGym.h"
#include "Trainer.h"
#include "Model.h"
#include "GameCommand.h"
#include "View.h"
#include <iostream>

using namespace std;

int main() {
    Model M;
    //M.ShowStatus();
    View V;
    char command;
    int ID1;
    int ID2;
    int x;
    int y;
    int amount;
    bool quit;
    while (!quit) {
        V.Clear();
        V.Plot(M.GetPokemonCenterPtr(1));
        V.Plot(M.GetPokemonCenterPtr(2));
        V.Plot(M.GetPokemonGymPtr(1));
        V.Plot(M.GetPokemonGymPtr(2));
        V.Plot(M.GetTrainerPtr(1));
        V.Plot(M.GetTrainerPtr(2));
        V.Draw();
        cout << "Enter Command: ";
        cin >> command;
        switch (command) {
            case 'm':
                cin >> ID1;
                cin >> x;
                cin >> y;
                cout << "Moving " << ID1 << " to (" << x << "," << y << ")" << endl;
                DoMoveCommand(M, ID1, Point2D(x, y));
                break;
            case 'c':
                cin >> ID1;
                cin >> ID2;
                cout << "Moving " << ID1 << " to Pokemon Center " << ID2 << endl;
                DoMoveToCenterCommand(M, ID1, ID2);
                break;
            case 'g':
                cin >> ID1;
                cin >> ID2;
                cout << "Moving " << ID1 << " to Pokemon Gym " << ID2 << endl;
                DoMoveToGymCommand(M, ID1, ID2);
                break;
            case 's':
                cin >> ID1;
                cout << "Stopping " << ID1 << endl;
                DoStopCommand(M, ID1);
                break;
            case 'p':
                cin >> ID1;
                cin >> amount;
                cout << ID1 << " buying " << amount << " potions" << endl;
                DoRecoverInCenterCommand(M, ID1, amount);
                break;
            case 'b':
                cin >> ID1;
                cin >> amount;
                cout << ID1 << " battling " << amount << " battles" << endl;
                DoBattleCommand(M, ID1, amount);
                break;
            case 'a':
                cout << "advancing one tick" << endl;
                DoAdvanceCommand(M, V);
                break;
            case 'r':
                cout << "running..." << endl;
                DoRunCommand(M, V);
                break;
            case 'q':
                cout << "quitting";
                quit = true;
                break;
            default:
                cout << "Invalid" << endl;
                break;
        }
    }

}
