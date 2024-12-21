# Code Readme

In station:

state.c/.h: This file contains our state machine for electing a cooridator. The FSM receives input from queue supplied in the udp file. The FSM can recieve four signals: 
  1. {v, id} there is a new leader, so you lost the election and need to be a follower.
  2. {h, id} a heartbeat from the leader, check if your ID is lower if so send an election.
  3. {e, id} tell all other ESPs there's an election.
  4. {a, id} an answer to the election, perform election and if you have the lowest ID you are now leader

Some other functionality: if no heartbeat comes in after 3 seconds an election is sent, if after 5 seconds no answers come in then the ESP can declare itself leader. Outgoing traffic also put in to a queue.

- udp.c/.h: This file recieves and sends traffic over wifi, reading and writing to/from a queue with state.c . The FSM traffic gets put on port 3333, and the voting traffic gets put on 5555. 

- irtx.c/.h: This file transmits the vote through IR. It encodes the vote (either R or B) and the voter ID.
- ir-rx.c/.h: This file receives the incoming IR vote from another ESP and sets the global variable currentVote to the vote received. 
- ir.c/.h: This file contains the repeating functions from both TX and RX that allow for initialization and functionality. 

- leader_main.c/.h: This is the main file of the project that runs all other tasks and initialization functions. 

- led.c/.h: This file controls the LEDs of the esp based on its current state. 

- wifi_station.c/.h: This file allows for the esps to connect to wifi. 

Front End: 
tingo_election folder contains tingo_election.c and webclient.html 
- tingo_election.c hosts the node server to receive and incoming votes from the leader. It then stores this in the election_db on the raspberry pi and sends it to the html file through socket.
- webclient.html runs the html table on the website and displays the database with total votes and buttons to reset or update the database. 




## Reminders
- Describe here any software you have adopted from elsewhere by citation or URL
- Your code should include a header with your team member names and date


