#include "pa2Functions.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

extern const int ENTRIES = 10;

int main()
{
    char code;
    int intinput;
    double first, last, delta;
    int ifirst, ilast;
    bool exit = false;
    char *file = new char[50];
    int count;
    initialize();
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
            case 'O': // good
                cin >> file;
                //cout << "uh";
                exit = true;
                writeDataToFile(file);
                break;
            case 'i':
            case 'I': // good
                cin >> file;
                exit = true;
                readDataFromFile(file);
                break;
            case 'f': // good
            case 'F':
                cin >> intinput;
                cout << factorial(intinput) << endl;
                break;
            case 'b': // good
            case 'B':
                cin >> intinput;
                cout << fibonacci(intinput) << endl;
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
                        if (count >= ENTRIES)
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
                        if (count >= ENTRIES)
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
                        if (count >= ENTRIES)
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
                        if (count >= ENTRIES)
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
                        if (count >= ENTRIES)
                        {
                            break;
                        }
                        else
                        {
                            cout << doMath(i, code) << endl;
                            count++;
                        }
                    }
                }
                break;
            case 'e': // first and last
            case 'E':
                cin >> ifirst;
                cin >> ilast;
                for (int i = ifirst; i <= ilast; i = i + 2)
                {
                    cout << findNextEvenValue(i) << endl;
                }
                break;
            case 'd': // first and last
            case 'D':
                cin >> ifirst;
                cin >> ilast;
                for (int i = ifirst; i <= ilast; i = i + 2)
                {
                    cout << findNextOddValue(i) << endl;
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
                        if (count >= ENTRIES)
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
                        if (count >= ENTRIES)
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
    } while (exit == false);
    return 0;
}
