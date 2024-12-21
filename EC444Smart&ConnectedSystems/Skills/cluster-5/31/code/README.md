# Code Readme

wasd.py: Creates UDP client, gets keyboard input of W, A, S, D, and X, and then sends those messages to ESP32. Used ChatGPT for this.

In udp_test:
udp.c has a server task that takes in input from laptop, calls movement function.
wasd.c has a movement function that triggers GPIO pins to move car.
wifi_station.c sets up wifi.
control_main.c calls init functions and starts server task.

