#include <iostream>
#include "readfile.h"
#include "mode.h"

using namespace std;

void printModes(int input[], int size,  int frequency, int result[], int result_count)
{
    cout << "Input: ";
    for(int i = 0; i < size; i++)
	{
        cout << input[i];
        if (i != size -1)
            cout << ", ";
    }
    cout << endl;
	
    cout << "Modes: ";
    for(int i = 0 ; i < result_count; i++)
	{
        cout << result[i];
        if (i != result_count -1)
            cout << ", ";
    }
    cout << endl;
    cout << "Frequency : " << frequency << endl;
}

int main()
{
	const int NUM_TERMS = 10; // 10 numbers for input
	const int RANGE_COUNT = 21; // -10 to 10
	int input[NUM_TERMS], result[RANGE_COUNT];
	int result_frequency, result_count;
	
	readFiletoArray("inputs/input_01.txt", input);
	findModes(input, NUM_TERMS, result_frequency, result, result_count);
	printModes(input, NUM_TERMS, result_frequency, result, result_count);
	
	readFiletoArray("inputs/input_02.txt", input);
	findModes(input, NUM_TERMS, result_frequency, result, result_count);
	printModes(input, NUM_TERMS, result_frequency, result, result_count);
	
	readFiletoArray("inputs/input_03.txt", input);
	findModes(input, NUM_TERMS, result_frequency, result, result_count);
	printModes(input, NUM_TERMS, result_frequency, result, result_count);
	
	readFiletoArray("inputs/input_04.txt", input);
	findModes(input, NUM_TERMS, result_frequency, result, result_count);
	printModes(input, NUM_TERMS, result_frequency, result, result_count);
	
	return 0;
}
