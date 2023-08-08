#include <iostream>
using namespace std;

#include "breakfast.h"
#include "waiter.h"

/**
 * This main function is a testbench for the other functions.
 * EC327 Staff will have their own version. Use this to test your functions.
 * Please DO include the output text file as well however with your submission. 
 * We will use this as a quick compilation test and to see what went wrong if you have issues.
 */
 
 extern const int PANCAKES = 1;
 extern const int FRENCHTOAST = 2;
 extern const int WAFFLES = 3;
 extern const int FRUITPLATE = 4;
 
 int main()
 {
 
	cout << "Breakfast Simulator 2021 Rev 1.0" << endl;
	cout << "How many people are eating breakfast today? (enter an integer): ";
	
	int breakfastCount;
	cin >> breakfastCount;
	
	int* breakFastArray = getBreakfastOrders(breakfastCount);
	char* order = processBreakfastFile(breakFastArray, breakfastCount);
	
	bool validOrder = orderCheck(order);
	double orderTotal = 0.0;
	if (validOrder)
	{
		cout << "You have created a valid order" << endl;
		orderSum(order, orderTotal);
		cout  << "Your order total is $" << orderTotal << endl;
	}
	else
	{
		cout << "There was an error with your order" << endl;
		return 0;
	}
	
	cout << "Enter how much money you are going to give: ";
	double payment;
	cin >> payment;
	
	double *change = pay(payment, orderTotal);
	
	while (*change < 0)
	{
		cout << "Cheapskate! Try again...." << endl;
		cout << "Enter how much money you are going to give: ";
		cin >> payment;
	
		change = pay(payment, orderTotal);
	}
	
	cout << "Thanks! The bill was $" << orderTotal << " and you gave $" << payment << " so your change is $" << *change << endl;
	cout << "Come again!";
	
	
	return 0;
 }
 
 
