//
// Created by Sebbie on 12/5/2022.
//

#ifndef PA3_INPUT_HANDLING_H
#define PA3_INPUT_HANDLING_H

#include <string>

using namespace std;

class Invalid_Input {
public:
    Invalid_Input(char *in_ptr) : msg_ptr(in_ptr) {}

    const string msg_ptr;
private:
    Invalid_Input();
};

#endif //PA3_INPUT_HANDLING_H
