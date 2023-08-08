#include <iostream>
using namespace std;

//Put your code here
void ptrsAreReallyFun()
{
	//Put your code for picture 1 here	

	
	//Last 4 lines of this section DON'T CHANGE
	cout << "Picture 1" << endl;
	cout << midtermArray[0] << " " << *(midtermArray+1) << endl;
	cout << *ptrToArray << " " << *arrayOfPtrs[1] << endl;
	cout << testChar << " " << *arrayOfPtrs[0] << endl;
	//***********************************************************
	
	
	//Put your code for picture 2 here

		
	//Last 2 lines of this section DON'T CHANGE
	for (int i=0; i<2; i++)
	cout << *(heapPtrArray+i) << endl;

	
	//***********************************************************
	
	
	//Put your code for picture 3 here

		
	//Last 8 lines of this section DON'T CHANGE
	cout << *intermedPtr << endl;
	cout << **deAllocPtr << endl;
	cout << *badPtr << endl;
	int *testPtr = new int;
	*testPtr = 56;
	cout << *intermedPtr << endl;
	cout << **deAllocPtr << endl;
	cout << *badPtr << endl;
	
	
}


/*Put your shiftElementsLower function here
This function takes a pointer to an array and a size
It then should return a new array that has all the same elements 
BUT they are each shifted one position lower (one index lower). The lowest
element should "wrap around" and end up being the last element (now 
the highest element).
*/

int * shiftElementsLower(int * px, int size) 
{ 
	//Add code here
	
	return returnArray;
}


/*
 This code is presented for you to test your code. No need to change it (but you can if you want)
*/

int main()
{

	//Section 1 - Recreate pictures in midterm booklet
	ptrsAreReallyFun();

	
	//Section 2 - create the shiftElementsLower function
	int array[5] = {1, 2, 3, 4, 5};
	int *newArray = shiftElementsLower(array, 5);
	
	for(int i = 0; i<5; i++)
	{
		cout << newArray[i] << " ";
	}
	return 0;

}

