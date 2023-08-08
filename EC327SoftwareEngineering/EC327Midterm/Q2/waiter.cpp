#include <fstream>
#include <iostream>
#include <string>
using namespace std;

extern const int PANCAKES;
extern const int FRENCHTOAST;
extern const int WAFFLES;
extern const int FRUITPLATE;

// 2 FIX statements total in this function
// Put orderCheck Function Here
bool orderCheck(char *order) // FIX
{
	ifstream readFile;
	readFile.open(order);
	string line;
	while (getline(readFile, line))
	{
		cout << "Validating " << line << "..." << endl;
		if (line == "Pancakes")
		{
			continue;
		}
		if (line == "FrenchToast")
		{
			continue;
		}
		if (line == "Waffles")
		{
			continue;
		}
		if (line == "FruitPlate") // FIX to check if the line is a valid choice of options
		{
			continue;
		}
		else
		{
			cout << line << " is not a valid breakfast choice!" << endl;
			readFile.close();
			return false;
		}
	}
	readFile.close();
	return true;
}

// 3 FIX statements total
// Must add the sum logic
// Put the orderSum Function here
void orderSum(char *order, double &ordertotal) // FIX
{
	cout << "Calculating sum for " << order << "..." << endl;
	ifstream orderFile;			  // FIX
	orderFile.open("Orders.txt"); // FIX
	string line;
	while (getline(orderFile, line))
	{
		cout << line << " is processing...." << endl;
		if (line == "Pancakes")
			ordertotal += 5.99;
		if (line == "FrenchToast")
			ordertotal += 7.53;
		if (line == "Waffles")
			ordertotal += 6.99;
		if (line == "FruitPlate")
			ordertotal += 2.43;
		// add things up
		// Pancakes cost 5.99
		// French Toast costs 7.53
		// Waffles cost 6.99
		// FruitPlate costs 2.43
	}
	orderFile.close();
	return;
}

// Put the pay function here
// It should substract the bill (how much you owe) from the payment (how much you paid)
// It should return as needed by Q2.cpp
double* pay(const double payment, const double orderTotal)
{
	double change;
	double* changeptr;
	changeptr = &change;
	change = payment - orderTotal;
	return &change;
}