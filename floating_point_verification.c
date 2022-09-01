// Compile with:
// gcc -O3 -o floating_point_verification floating_point_verification.c

#include <stdio.h>
#include <string.h>

// Enum of all states
typedef enum
{
    START,
    AFTER_MINUS,
    AFTER_DOT,
    internal_VALID_END_STATES, // internal: anything below is a valid end state 
                               // this can be used for logic because enum element values increase downwards
    SECOND_DIGIT_ONWARDS,
    MANTISSA,
    internal_LAST_STATE        // used for looping through states, when pointer reaches this state, looping ends
} FSM_STATES;

// Information for each state
typedef struct
{
    FSM_STATES state;
    char *tokens; // valid tokens for transition, this will be a string (which is char *)
    FSM_STATES next_state;
} FSM_STATE_INFO;

// Second param: FSM_STATE_INFO* is an array of indiviual
// Third param: cargo is a string representing an integer i.e., "123"
int run(FSM_STATES start_state, FSM_STATE_INFO *state_machine, char *cargo)
{

    FSM_STATE_INFO *p; // assign to state_machine pointer later
    FSM_STATES current_state = start_state;
    FSM_STATE_INFO *found;
    char *c = cargo; // cursor to move through cargo string
                     // c is a pointer to the start of the cargo array
                     // if you dereference c, you get the first character in the cargo array
                     // if you ++c, you move the pointer to the next character in the cargo array

    while (*c != 0)
    {
        found = NULL;
        p = state_machine;
        // Loop through all valid states until current_state is reached
        // TODO: improve efficiency with some sort of lookup
        while (p->state != internal_LAST_STATE)
        {
            if (p->state == current_state)
            {
                // Checks if current char of cargo is in the valid transition tokens form the struct
                if (strchr(p->tokens, *c) != NULL)
                {
                    found = p;
                    break;
                }
            }
            p++;
        }
        // Transition successful, a token matches current character
        if (found != NULL)
        {
            current_state = p->next_state;
            c++;
        }
        else
        {
            printf("%s is bad. Failed at %c\n", cargo, *c);
            return -1;
        }
    }
    // Recall enum states increase, so anything below is greater
    if (current_state > internal_VALID_END_STATES)
    {
        printf("%s is good.\n", cargo);
        return 1;
    }
    else
    {
        printf("%s is bad. More needed.\n", cargo);
        return -2;
    }
}

int main(int argc, char *argv[])
{
    static FSM_STATE_INFO state_machine[] = {
        {START, "1234567890", SECOND_DIGIT_ONWARDS},
        {START, "-", AFTER_MINUS},
        {AFTER_MINUS, "1234567890", SECOND_DIGIT_ONWARDS},
        {SECOND_DIGIT_ONWARDS, "1234567890", SECOND_DIGIT_ONWARDS},
        {SECOND_DIGIT_ONWARDS, ".", AFTER_DOT},
        {AFTER_DOT, "1234567890", MANTISSA},
        {MANTISSA, "1234567890", MANTISSA},
        {internal_LAST_STATE, "", internal_LAST_STATE}};

    run(START, state_machine, "3.14");
    run(START, state_machine, "-7");
    run(START, state_machine, "-22.0");
    run(START, state_machine, "--22.0");
    run(START, state_machine, "-22.a0");
    run(START, state_machine, "-1.");
    run(START, state_machine, "-");
}
