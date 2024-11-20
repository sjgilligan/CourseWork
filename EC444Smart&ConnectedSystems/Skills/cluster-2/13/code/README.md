# Code Readme

Three tasks, 1. sends the data via I2C to the display. 2. Polls for input over UART. 3. Performs the cycling of the text if there are more that 4 characters.

Used a global variable for the string the user sends and then an array to hold the 4 characters that should be displayed.

Used CHATGPT to make the function that converts ASCII to the binary value for the display.

