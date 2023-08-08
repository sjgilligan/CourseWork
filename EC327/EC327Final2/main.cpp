#include <iostream>
#include "Board.h"
//#include <The .h file for player>
#include "Visual.h"
#include "Point2d.h"
#include "Player.h"
//#include <The .h file for the CPU/AI>
//#include <The .h file for rankings>

using namespace std;

int main()
{
    Visual V;
    Board B;
    //system('cls');
	int Select; //Choice to face the CPU, PlayerTwo, or Quitting the program
	char Again; //If yes, starts another game. If no, returns to the title screen

	/* Their type is the player class */
	/* Each player is assigned to their piece */
	/*Player Player1;
	Player Player2;*/

	/* When it is the player's turn, they will drop a piece in one of the columns (1-7)
	// int Player1Drop;
	// int Player2Drop;

	/* Used do-while loop until player decides to quit the game */

	do{
		//Display comes from the graphics file
		//It will show the title screen and the options

		/* Example */
		//Welcome to Connect 4!!!
        V.Startup();

		//1P VS. CPU
		//1P VS. 2P
		//EXIT

		/* Keeps track of the score if prompted to continue */
		/* When returning to to the title screen, the score resets */
		/*int AIScore = 0;
		int P1Score = 0;
		int P2Score = 0;
		int Draw = 0;*/

        cout << "1 or 2 players" << endl;
        cin >> Select;

        if(Select == 1)

        }
    }
		/* If 1P VS. CPU was selected (This is a while loop) */
		//Player 1 needs to input name
		//cout << "Player 1, please enter your name: " << endl;
		//cin >> //Player1.
		/*do {
			//Pieces are given to the player depending on if they're 1st or 2nd
			//Board is on display

			cout << "P1 Wins" << P1Score << endl; //Keeps track of player wins
			cout << "AI Wins" << AIScore << endl; //Keeps track of AI Wins
			cout << "Draw(s)" << Draw << endl; //Keeps track of draws

			while(//The conditions game over conditions are not met)
			{
				cout << "Player 1's turn: ";
				cin >> Player1Drop;

				cout << "CPU's turn";
			}

			/* Winner will be decided if 4-in-a-row was -,|,/,\ */

			/* if Draw */
			//Draw++

			/* if Player 1 wins */
			//P1Score++

			/* if CPU wins */
			//AIScore++

			//cout << "Would you like to play again?" << endl;


		//} while(Again != 'n' || Again != 'N')


		/* If 1P VS. 2P was selected (This is a while loop) */
		//Player 1 needs to input name
		/*cout << "Player 1, please enter your name: " << endl;
		cin >> //Player1.
		cout << "Player 2, please enter your name: " << endl;
		cin >> //Player2.*/
		//do {
			//Pieces are given to the player depending on if they're 1st or 2nd
			//Board is on display

			/*cout << "P1 Wins" << P1Score << endl; //Keeps track of player wins
			cout << "AI Wins" << AIScore << endl; //Keeps track of AI Wins
			cout << "Draw(s)" << Draw << endl; //Keeps track of draws*/

			/*while(//The conditions game over conditions are not met)
			{
				cout << "Player 1's turn: ";
				cin >> Player1Drop;

				cout << "Player 2's turn: ";
				cin >> Player2Drop
			}*/

			/* Winner will be decided if 4-in-a-row was -,|,/,\ */

			/* if Draw */
			//Draw++

			/* if Player 1 wins */
			//P1Score++

			/* if Player 2 wins */
			//P2Score++

			//cout << "Would you like to play again?" << endl;


		//} while(Again != 'n' || Again != 'N');


	//} while(Select != 'q' || 'Q'); //If the player inputs 'q' or 'Q', the loop breaks and the program should be closed


return 0;
}
