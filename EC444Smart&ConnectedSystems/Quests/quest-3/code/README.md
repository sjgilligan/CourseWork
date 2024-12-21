# Code Readme

cat_track:
1. cat_track_main.c: run init functions for peripherals, create all 10 tasks. We also have our log_data task here which packages our payload.
2. udp.c: udp_client_task creates the socket between the ESP32 and the RasPi. We have both a server task reveiving data (i.e. if there is a new leader), and a client task sending data to the RasPi (activity state and time).
3. accelerometer.c: this is where acceleration data is read in and the activity state is determined and counts how long the cat has been in a state. The data is continually rolled so that after 10 minutes the data is disgarded.
4. display.c: all operations needed to run the display, has function to concert ascii values and a task to cycle through a string.
5. buzzer.c: Has a task that either turns on or off the buzzer depending on a change in a global variable changed by a UDP packet.

raspberrypi:
1. node_server.js: This istens for data from udp.c as a server and then sends leader status back to udp.c as a client. It also sends data from udp.c to index.html for plotting and hosts a webpage to display a graph.
2. index.html: This listens for cumulative data delivered from the node.js socket and plots a stacked-column graph with a cat ID on the x-axis and activity time on the y-axis continuously. It also resets the graph upon a 0 received from the active or highly active fields.
3. mjpeg_server.py: Hosts the picamera stream in a webpage, which can then be accessed from anywhere.

## Reminders
We used this python file to stream the camera feed to HTML: https://github.com/janakj/py-mjpeg
The UDP code is taken from skill 22. 
The accelerometer and display code is repurposed from Quest 2.


