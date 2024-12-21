# Code Readme

Within quest_5_bot:
-------------------
main.c: call init functions and create 5 tasks: wasd, udp_server, udp_client, ir_range, butterfly_move

udp_server: get WASD data from a user's laptop.

udp_client: request, recieves, and parses optitrack data. Stores that data in to a struct which other functions have access to. The struct looks like:

typedef struct {
    int id;
    float x;
    float z;
    float theta;
    char status[10];
} RobotData;

move: sets up PWM, movement function recieves a number 0-1028 for each wheel, and bit for direction.

turning: given a desired heading and the current heading will rotate return false until it reaches the desired heading.

butterfly_move: main script that sets waypoints and drives the car straight to them, calls turn to set new angle, and stops the cars and moves back when object is detected

range: reads analog signal from IR range finder to detect whether there's an object to avoid


## Reminders
- Describe here any software you have adopted from elsewhere by citation or URL
- Your code should include a header with your team member names and date


