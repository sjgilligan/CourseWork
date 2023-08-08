/**
 * This function records your vote.

 It checks if valid is true and then prompts the user.


 * i1 = 1 for Kanye, 2 for The Rock and 3 for other
 * i2 = 1 for Densmore, 2 for Kodos, and 3 for Kang


 * @param int i1 President value
 * @param int i2 Supreme leader value
 * @param bool valid - a boolean value if the user can vote

 If the vote is valid prompt the user for the president and record the result in the correct variable
 The prompt should be EXACTLY: "President: Kanye (1), The Rock (2), Other (3): "
 Then prompt for the supreme leader and record using EXACTLY: "Supreme Leader: Densmore (1), Kodos (2), Kang (3): "

 If the user provides a value higher than 3 for either entry, both should be set to -1 and it should output "Invalid entry"

 If the vote in invalid both i1 and i2 should be set to -1 and it should output "You can't vote!"

 */

#include <iostream>

using namespace std;

const int INVALID = -1;

// Add vote function here

void vote(int& i1, int& i2, bool valid)
{
    if (valid == true)
    {
        cout << "President: Kanye (1), The Rock (2), Other (3): " << endl;
        cin >> i1;
        cout << "Supreme Leader: Densmore (1), Kodos (2), Kang (3): " << endl;
        cin >> i2;
        if (i1 > 3 || i2 > 3)
        {
            cout << "You can't vote!";
            i1 = -1;
            i2 = -1;
        }
    }
}
