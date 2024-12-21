# Code Readme

In main folder:


cat_track_main.c: Calls init functions, and creates 8 tasks. Printing to terminal is also done here. All general I2C set up and scanning happens here. The accelerometer and temp tasks update externed variables so that new values can be printed to the terminal.

accelerometer.c/.h: Sets up and reads/write from the ADX, determines the activity and the time of activity. The activity is determined by total acceleration. The thresholds for active is 1.15 g and 1.6 g for highly active.

display.c/.h: Sets up and writes to the alphanumeric display, also does the cycling of the display given a string from button_task.

button.c/.h: Sets up hardware interupt, on button push a string is created with the cat's name, activity and activity time.

temp.c/.h: Sets up, polls, and does conversion for the thermistor.



## Reminders
Some CHATGPT used in button.c to remind myself (Sebastian) how to concatonate strings in C and to calculate a measure of activity from the acceleration.


