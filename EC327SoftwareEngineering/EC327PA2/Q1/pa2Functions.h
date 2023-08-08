#ifndef PA2FUNCTIONS_H
#define PA2FUNCTIONS_H
#include <fstream>
#include <stdlib.h>

void initialize();
bool checkCode(char);
void writeDataToFile(const char *);
void readDataFromFile(const char *);
int factorial(int);
int fibonacci(int);
double findSqrtValue(double);
double naturalLog(double);
double areaCircle(double);
double areaSquare(double);
int findNextOddValue(int);
int findNextEvenValue(int);
double findNyanCatValue(double);
double doMath(double, char);
double lucky(double);

#endif