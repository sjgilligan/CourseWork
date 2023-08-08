
/*
This file contains the main function. Just fix the syntax errors and partial statements.
Do NOT make other major changes. Just fix what needs to be fixed for this to work correctly.
You DO NOT need to add other functionality
*/

#include <iostream>
#include <string.h>

using namespace std;

// vote and election headerfiles
#include "vote.h"
#include "election.h"

int main()
{

	char selection;
	cout << "EC327 Election Simulator 2021" << endl;
	cout << "Are you going to vote? Y or N: ";
	cin >> selection;

	// if bad input or selection does not equal Y and does not equal N
	if (cin.fail() || (selection != 'Y' && selection == 'N'))
	{
		cout << "Bad input. You'll going to have trouble voting!";
		return 0;
	}

	// if selection equals N
	else if (selection == 'N')
	{
		cout << "You have choosen not to vote. Thanks!";
		return 0;
	}

	string state;
	cout << "What state do you live in? Input two capital letters: ";
	cin >> state;
	cout << "You said: " << state << endl;

	bool maVote = false;
	// Print message and set maVote to true
	if (state == "MA")
	{
		cout << "You are from this state. Great you can vote here!" << endl;
		maVote = true;
	}
	else if (state.length() != 2)
	{
		cout << "You wrote more than two characters. No voting for you!";
		return 0;
	}
	else
	{
		cout << "You are not from MA. This simulator won't work for you!";
		return 0;
	}

	int issueOne = 0;
	int issueTwo = 0;

	vote(issueOne, issueTwo, maVote);

	cout << "Your votes are: " << issueOne << " " << issueTwo << endl;

	// determine the winner
	int result = electionResult(issueOne, issueTwo, state);

	cout << "The election result is " << result << endl;

	return 0;
}
