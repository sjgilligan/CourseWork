#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

extern const int PANCAKES;
extern const int FRENCHTOAST;
extern const int WAFFLES;
extern const int FRUITPLATE;

// 4 FIX statements
// add getBreakfastOrders header
int *getBreakfastOrders(int count) // FIX
{

	int* breakfastArray = new int[count]; // FIX
	int ordernum;
	for (int i = 0; i < count; i++)
	{
		cout << "Order #" << i << " is?: ";
		cin >> ordernum;
		breakfastArray[i] = ordernum; // FIX
	}

	return breakfastArray; // FIX
}

// 6 Fix statements
// add processBreakfastFile header
char *processBreakfastFile(int* orders, int orderCount) // FIX
{
	char *filename = new char;
	filename = (char*) "Orders.txt"; // FIX
	ofstream outputFile;
	outputFile.open(filename);
	outputFile.clear();
	for (int i = 0; i < orderCount; i++)
	{
		switch (orders[i])
		{
		case 1:
			outputFile << "Pancakes" << endl; // FIX
			break;
		case 2:
			outputFile << "FrenchToast" << endl; // FIX
			break;
		case 3:
			outputFile << "Waffles" << endl; // FIX
			break;
		case 4:
			outputFile << "FruitPlate" << endl; // FIX
			break;
		default:
			cout << "Error processing breakfast. There is no " << orders[i] << endl;
		}
	}
	outputFile.close();
	return filename;
}
