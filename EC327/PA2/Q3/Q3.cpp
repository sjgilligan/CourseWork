#include <iostream>
using namespace std;

int *ReverseMultiply(int *list, int size)
{
    int *newlist = new int[size * 2 + 1];
    int product = 1;
    for (int i = 0; i < size; i++)
    {
        newlist[i] = list[i];
        newlist[i + size] = list[size - i - 1];
        product = product * list[i] * list[i];
    }
    newlist[size * 2] = product;
    return newlist;
}

int * ReverseAdd(int *list, int size)
{
    int *newlist = new int[size * 2 - 1];
    for(int i = 0; i < size; i++)
    {
        newlist[i] = list[i];
        newlist[i + size] = list[size - i - 1] + list[size - i - 2];
    }
    return newlist;
}

int main()
{
    int size;
    cout << "Enter the number of entries: ";
    cin >> size;
    int *list = new int[size];
    for (int i = 0; i < size; i++)
    {
        cout << "Entry " << i << " is: ";
        cin >> list[i];
    }
    cout << "Original array is: ";
    for (int i = 0; i < size; i++)
    {
        cout << list[i] << " ";
    }
    cout << " and the address of the zero element is: " << &list << endl;
    cout << "One array is: ";
    for (int i = 0; i < (size*2) + 1; i++)
    {
        cout << ReverseMultiply(list,size)[i] << " ";
    }
    cout << "and the address of zero element is: " << &ReverseMultiply(list,size)[0] << endl;
    cout << "Two array is: ";
    for (int i = 0; i < (size*2 - 1); i++)
    {
        cout << ReverseAdd(list,size)[i] << " ";
    }
    cout << "and the address of zero element is: " << &ReverseAdd(list,size)[0] << endl;
    return 0;
}