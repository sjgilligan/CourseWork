#include "pa2Functions.h"
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <cstring>
using namespace std;

extern const int ENTRIESS = 10;

void initialize()
{
    cout << "EC327: Introduction to Software Engineering" << endl;
    cout << "Fall 2022" << endl;
    cout << "Programming Assignment 2" << endl;
    cout << "Value of Entries is: " << ENTRIESS << endl;
}

void writeDataToFile(const char *file)
{
    //cout << "working";
    char code;
    int intinput;
    bool exit = false;
    double first, last, delta;
    int ifirst, ilast;
    int count;
    ofstream outdata;
    outdata.open(file);
    do
    {
        count = 0;
        cout << "Please enter command code" << endl;
        cin >> code;
        if (checkCode(code) == false)
        {
            cout << "Invalid Command Code" << endl;
        }
        else if (code == 'Q' || code == 'q')
        {
            exit = true;
        }
        else
        {
            cout << "Enter command parameters: ";
            switch (code)
            {
            case 'o':
            case 'O':
                break;
            case 'f':
            case 'F':
                cin >> intinput;
                cout << factorial(intinput) << endl;
                outdata << factorial(intinput) << endl;
                break;
            case 'b':
            case 'B':
                cin >> intinput;
                cout << fibonacci(intinput) << endl;
                outdata << fibonacci(intinput) << endl;
                break;
            case 'r':
            case 'R':
                cin >> first;
                cin >> last;
                cin >> delta;
                if (delta <= 0 || first > last)
                {
                    cout << "No computation needed" << endl;
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << findSqrtValue(i) << endl;
                            outdata << findSqrtValue(i) << endl;
                            count++;
                        }
                    }
                }
                break;
            case 'l':
            case 'L':
                cin >> first;
                cin >> last;
                cin >> delta;
                if (delta <= 0 || first > last)
                {

                    cout << "No computation needed";
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << naturalLog(i) << endl;
                            outdata << naturalLog(i) << endl;
                            count++;
                        }
                    }
                }
                break;
            case 'c':
            case 'C':
                cin >> first;
                cin >> last;
                cin >> delta;
                if (delta <= 0 || first > last)
                {
                    cout << "No computation needed";
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << areaCircle(i) << endl;
                            outdata << areaCircle(i) << endl;
                            count++;
                        }
                    }
                }
                break;
            case 'u':
            case 'U':
                cin >> first;
                cin >> last;
                cin >> delta;
                if (delta <= 0 || first > last)
                {
                    cout << "No computation needed";
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << areaSquare(i) << endl;
                            outdata << areaSquare(i) << endl;
                            count++;
                        }
                    }
                }
                break;
            case 's':
            case 'S':
            case 'n':
            case 'N':
            case 'x':
            case 'X':
                cin >> first;
                cin >> last;
                cin >> delta;
                if (delta <= 0 || first > last)
                {
                    cout << "No computation needed";
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << doMath(i, code) << endl;
                            outdata << doMath(i, code) << endl;
                            count++;
                        }
                    }
                }
                break;
            case 'e':
            case 'E':
                cin >> ifirst;
                cin >> ilast;
                for (int i = ifirst; i <= ilast; i = i + 2)
                {
                    cout << findNextEvenValue(i) << endl;
                    outdata << findNextEvenValue(i) << endl;
                }
                break;
            case 'd':
            case 'D':
                cin >> ifirst;
                cin >> ilast;
                for (int i = ifirst; i <= ilast; i = i + 2)
                {
                    cout << findNextOddValue(i) << endl;
                    outdata << findNextOddValue << endl;
                }
                break;
            case 'y':
            case 'Y':
                cin >> first;
                cin >> last;
                cin >> delta;
                if (delta <= 0 || first > last)
                {
                    cout << "No computation needed";
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << findNyanCatValue(i) << endl;
                            outdata << findNyanCatValue(i) << endl;
                            count++;
                        }
                    }
                }
                break;
            case 'k':
            case 'K':
                cin >> first;
                cin >> last;
                cin >> delta;
                if (delta <= 0 || first > last)
                {
                    cout << "No computation needed";
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << lucky(i) << endl;
                            outdata << lucky(i) << endl;
                            count++;
                        }
                    }
                }
                break;
            }
        }
    } while (exit == false);
    outdata.close();
}

void readDataFromFile(const char *file)
{
    char code;
    int intinput;
    double dubinput;
    double first, last, delta;
    int ifirst, ilast;
    bool exit = false;
    int count;
    ifstream indata;
    indata.open(file);
    while (exit == false && indata >> code)
    {
        count = 0;
        cout << "Please enter command code" << endl;
        cout << code << endl;
        if (checkCode(code) == false)
        {
            cout << "Invalid Command Code" << endl;
        }
        else if (code == 'Q' || code == 'q')
        {
            exit = true;
        }
        else
        {
            cout << "Enter command parameters: ";
            switch (code)
            {
            case 'o':
            case 'O':
                break;
            case 'i':
            case 'I':
                break;
            case 'f':
            case 'F': // single int
                indata >> intinput;
                cout << intinput << endl;
                cout << factorial(intinput) << endl;
                break;
            case 'b':
            case 'B': // single int
                indata >> intinput;
                cout << intinput << endl;
                cout << fibonacci(intinput) << endl;
                break;
            case 'r':
            case 'R': // first last delta
                indata >> first;
                indata >> last;
                indata >> delta;
                cout << first << " " << last << " " << delta << endl;
                if (delta <= 0 || first > last)
                {
                    cout << "No computation needed" << endl;
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << findSqrtValue(i) << endl;
                            count++;
                        }
                    }
                }
                break;
                break;
            case 'l':
            case 'L':
                indata >> first;
                indata >> last;
                indata >> delta;
                cout << first << " " << last << " " << delta << endl;
                if (delta <= 0 || first > last)
                {

                    cout << "No computation needed";
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << naturalLog(i) << endl;
                            count++;
                        }
                    }
                }
                break;
            case 'c':
            case 'C':
                indata >> first;
                indata >> last;
                indata >> delta;
                cout << first << " " << last << " " << delta << endl;
                if (delta <= 0 || first > last)
                {
                    cout << "No computation needed";
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << areaCircle(i) << endl;
                            count++;
                        }
                    }
                }
                break;
            case 'u':
            case 'U':
                indata >> first;
                indata >> last;
                indata >> delta;
                cout << first << " " << last << " " << delta << endl;
                if (delta <= 0 || first > last)
                {
                    cout << "No computation needed";
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << areaSquare(i) << endl;
                            count++;
                        }
                    }
                }
                break;
            case 's':
            case 'S':
            case 'n':
            case 'N':
            case 'x':
            case 'X':
                indata >> first;
                indata >> last;
                indata >> delta;
                cout << first << " " << last << " " << delta << endl;
                if (delta <= 0 || first > last)
                {
                    cout << "No computation needed";
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << doMath(i,code) << endl;
                            count++;
                        }
                    }
                }
                break;
            case 'e':
            case 'E':
                indata >> ifirst;
                indata >> ilast;
                cout << ifirst << " " << ilast << endl;
                for (int i = ifirst; i <= ilast; i = i + 2)
                {
                    cout << findNextEvenValue(i) << endl;
                }
                break;
            case 'd':
            case 'D':
                indata >> ifirst;
                indata >> ilast;
                cout << ifirst << " " << ilast << endl;
                for (int i = ifirst; i <= ilast; i = i + 2)
                {
                    cout << findNextOddValue(i) << endl;
                }
                break;
            case 'y':
            case 'Y':
                indata >> first;
                indata >> last;
                indata >> delta;
                cout << first << " " << last << " " << delta << endl;
                if (delta <= 0 || first > last)
                {
                    cout << "No computation needed";
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << findNyanCatValue(i) << endl;
                            count++;
                        }
                    }
                }
                break;
            case 'k':
            case 'K':
                indata >> first;
                indata >> last;
                indata >> delta;
                cout << first << " " << last << " " << delta << endl;
                if (delta <= 0 || first > last)
                {
                    cout << "No computation needed";
                    break;
                }
                else
                {
                    for (double i = first; i <= last; i = i + delta)
                    {
                        if (count >= ENTRIESS)
                        {
                            break;
                        }
                        else
                        {
                            cout << lucky(i) << endl;
                            count++;
                        }
                    }
                }
                break;
            }
        }
    }
    return;
}

bool checkCode(char code)
{
    bool output;
    if (code == 'F' || code == 'f' || code == 'B' || code == 'b' || code == 'R' || code == 'r' || code == 'U' || code == 'u' || code == 'C' || code == 'c' || code == 'E' || code == 'e' || code == 'K' || code == 'k' || code == 'S' || code == 's' || code == 'N' || code == 'n' || code == 'X' || code == 'x' || code == 'L' || code == 'l' || code == 'Y' || code == 'y' || code == 'D' || code == 'd' || code == 'I' || code == 'i' || code == 'O' || code == 'o' || code == 'Q' || code == 'q')
    {
        output = true;
        return output;
    }
    else
    {
        output = false;
        return output;
    }
}

int factorial(int input)
{
    int output = 1;
    for (int i = 1; i <= input; i++)
    {
        output = output * i;
    }
    return output;
}

int fibonacci(int input)
{
    int term1 = 0;
    int term2 = 1;
    int next = 0;
    if (input == 1)
    {
        return term1;
    }
    else if (input == 2)
    {
        return term2;
    }
    else
    {
        for (int i = 1; i <= input; i++)
        {
            next = term1 + term2;
            term1 = term2;
            term2 = next;
        }
    }
    return next;
}

double findSqrtValue(double input)
{
    return sqrt(input);
}

double naturalLog(double input)
{
    return log(input);
}

double areaCircle(double input)
{
    return (input * input) * 6.28;
}

double areaSquare(double input)
{
    return input * input;
}

double doMath(double input, char code)
{
    switch (code)
    {
    case 's':
    case 'S':
        return sin(input);
    case 'n':
    case 'N':
        return cos(input);
    case 'x':
    case 'X':
        return exp(input);
    }
    return 0;
}

int findNextOddValue(int input)
{
    if (input % 2 == 0)
        return input + 1;
    else
        return input + 2;
}

int findNextEvenValue(int input)
{
    if (input % 2 == 0)
        return input + 2;
    else
        return input + 1;
}

double findNyanCatValue(double input)
{
    return (pow((4 * input), input) + input + 10);
}

double lucky(double input)
{
    srand(time(NULL) * input);
    double output = rand();
    return output;
}