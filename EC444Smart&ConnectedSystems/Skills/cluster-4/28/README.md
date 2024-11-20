#  Leader Election

Author: Sebastian Gilligan

Date: 2024-11-9

### Summary

Network system where there is one leader/coordiantor and several other 'followers.' Without a central computer, the individual ESP32s must determine who has the lowest ID number. The process of doing that is sending out an election signal 'e, id' followed by responses 'a, id' and then the winner sends out a 'v, id' and a heartbeat signal 'h, id.' If a new ESP joins and determines that they have a lower ID than the one in the heartbeat they send out a 'e, id.' If a new ESP joins and they have a higher ID, no election occurs. 

### Evidence of Completion

Video: https://drive.google.com/file/d/1vLTkiePuGEW8cj374oY7mGUu6g8ChNNL/view?usp=sharing




