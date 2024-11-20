//100% made from ChatGPT using this prompt:
//create an FSM in c using this state table:
// | Event/State | Waiting for payment | Waiting for fuel select & nozzle  | Pumping gas | Ask for receipt
// |---|---|---|---|---|
// |Card inserted OR paid inside|Waiting for fuel select & nozzle|N/A|N/A|N/A|
// |Fuel selected AND nozzle removed|N/A|Pumping gas|N/A|N/A|
// |Tank filled OR user removes|N/A|N/A|Ask for receipt|N/A|
// |Receipt accepted/denied OR timeout|N/A|N/A|N/A|Waiting for payment|
#include <stdio.h>

// Define possible states
typedef enum {
    WAITING_FOR_PAYMENT,
    WAITING_FOR_FUEL_SELECT_AND_NOZZLE,
    PUMPING_GAS,
    ASK_FOR_RECEIPT
} State;

// Define possible events
typedef enum {
    EVENT_CARD_INSERTED_OR_PAID_INSIDE,
    EVENT_FUEL_SELECTED_AND_NOZZLE_REMOVED,
    EVENT_TANK_FILLED_OR_USER_REMOVES,
    EVENT_RECEIPT_ACCEPTED_OR_DENIED_OR_TIMEOUT
} Event;

// Function to transition states based on current state and event
State transition(State currentState, Event event) {
    switch (currentState) {
        case WAITING_FOR_PAYMENT:
            if (event == EVENT_CARD_INSERTED_OR_PAID_INSIDE) {
                return WAITING_FOR_FUEL_SELECT_AND_NOZZLE;
            }
            break;

        case WAITING_FOR_FUEL_SELECT_AND_NOZZLE:
            if (event == EVENT_FUEL_SELECTED_AND_NOZZLE_REMOVED) {
                return PUMPING_GAS;
            }
            break;

        case PUMPING_GAS:
            if (event == EVENT_TANK_FILLED_OR_USER_REMOVES) {
                return ASK_FOR_RECEIPT;
            }
            break;

        case ASK_FOR_RECEIPT:
            if (event == EVENT_RECEIPT_ACCEPTED_OR_DENIED_OR_TIMEOUT) {
                return WAITING_FOR_PAYMENT;
            }
            break;

        default:
            // Stay in the current state if no valid transition exists
            return currentState;
    }
    // Return current state if event does not trigger a transition
    return currentState;
}

// Function to print the current state
void printState(State state) {
    switch (state) {
        case WAITING_FOR_PAYMENT:
            printf("State: Waiting for payment\n");
            break;
        case WAITING_FOR_FUEL_SELECT_AND_NOZZLE:
            printf("State: Waiting for fuel selection and nozzle\n");
            break;
        case PUMPING_GAS:
            printf("State: Pumping gas\n");
            break;
        case ASK_FOR_RECEIPT:
            printf("State: Asking for receipt\n");
            break;
    }
}

int main() {
    State currentState = WAITING_FOR_PAYMENT;
    Event event;

    // Example sequence of events
    Event events[] = {
        EVENT_CARD_INSERTED_OR_PAID_INSIDE,
        EVENT_FUEL_SELECTED_AND_NOZZLE_REMOVED,
        EVENT_TANK_FILLED_OR_USER_REMOVES,
        EVENT_RECEIPT_ACCEPTED_OR_DENIED_OR_TIMEOUT
    };

    // Process events
    for (int i = 0; i < sizeof(events) / sizeof(events[0]); i++) {
        event = events[i];
        currentState = transition(currentState, event);
        printState(currentState);
    }

    return 0;
}