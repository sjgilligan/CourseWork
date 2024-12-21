#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    float x;
    float z;
    float theta;
    char status[10];
} RobotData;

int parseRobotData(const char *input, RobotData *data) {
    // Parse the input string and populate the RobotData structure
    int parsed = sscanf(input, "%d,%f,%f,%f,%9s", &data->id, &data->x, &data->z, &data->theta, data->status);

    // Check if all fields were successfully parsed
    if (parsed == 5) {
        return 1; // Success
    } else {
        return 0; // Parsing failed
    }
}