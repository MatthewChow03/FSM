/**
 *      High level overview
 * 
 *      A table of states and events can be indexed to call funcitons (the events are stored in columns as function pointers)
 *      Each event returns a state and takes in the current state and event
 *      Typedef is used throughout to shorted code with aliases
 * 
 *      Initially state = START = 0, first call to step_state() passes in event = START_LOOPING = 0
 *      Then, the state machine returns state = LOOP = 1
 *      step_state() will be called with state = LOOP = 1 ad event = PRINT_HELLO = 1
 *      This will print "Hello World!"
 * 
 *      Note, there is no protection anywhere for out of bounds
 * 
 */

#include <stdio.h>
#include <stdlib.h>

enum states {
    START,
    LOOP,
    END,
} state;    // create an enum state that is global
typedef enum states states;     // now you can use "states myVarName" instead of "enum states myVarName"

enum events {
    START_LOOPING,
    PRINT_HELLO,
    STOP_LOOPING,
};
typedef enum events events;     // now you can use "events myVarName" instead of "enum events myVarName"

// Typedef funciton pointers
// Using the type event_handler will create a function pointer that takes in 2 params and returns an enum states type
// https://stackoverflow.com/questions/4295432/typedef-function-pointer
typedef states (*event_handler)(states, events);

states start_looping(states state, events event) {
    return LOOP;
}

states print_hello(states state, events event) {
    printf("Hello World!\n");
    printf("State: %d\nEvent: %d\n\n", state, event);
    return LOOP;
}

states stop_looping(states state, events event) {
    return END;
}

// Use designated initializers to initialize specific elements of an array
// The result is the following

// row0, start state:   {start_looping}
// row1, loop state:    {print_hello, stop_looping}
// row2, end state:     {} -- no events for the end state

event_handler transitions[STOP_LOOPING+1][END+1] = {
    [START] =   { [START_LOOPING] = &start_looping, },
    [LOOP] =    { [PRINT_HELLO] = &print_hello, 
                  [STOP_LOOPING] = &stop_looping,           // ----------------------------------------- https://stackoverflow.com/questions/7518815/function-pointer-automatic-dereferencing
                },
};

void step_state(events event) {
    event_handler handler = transitions[state][event];  // get the funciton pointer of an event
    if (!handler)
        exit(1);
    state = (*handler)(state, event);  // trigger the event     // ------------------------------------------ ^ dereferencing * isn't necessary and & isn't necessary either
}

int main(void) {
    step_state(START_LOOPING);
    step_state(PRINT_HELLO);
    step_state(PRINT_HELLO);
    step_state(STOP_LOOPING);
    return 0;
}