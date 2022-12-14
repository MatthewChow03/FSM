#   High level overview
#
#   Parse using tokens, where a token is a character, and each input integer is a string
#       Compare character equality to determine if tokens are valid
#       A transition is a valid token equailty
#   Use a dictionary to store each state's valid transitions and next states
#   Move along states until error or no more tokens to compare
#       Only specific * states can be valid numbers if they are the final state

from collections import namedtuple

# Tuple where values are accessible with a dot notation (i.e., myToken.next_state)
tok_next_t = namedtuple('tok_next_t', ['tokens', 'next_state'])

class myFSM:
    def __init__(self):
        # Dictionary of states, valid transition tokens, and valid next states
        self.tok_next = {}

    def add_state(self, name, tokens, next_state):
        # tok_next is a list of tok_next_t namedtuples
        # i.e. a list of the valid tokens and transition states
        if name not in self.tok_next:
            self.tok_next[name] = []
        # List of tokens and states can build up for each key in the KV pairs
        self.tok_next[name].append(tok_next_t(tokens, next_state))
        
    def run(self, cargo):
        orig_num = cargo    # Cargo argument of fsm.run() will be a string, so len(str) can be used
        state = "START"
        err = ""
        while True:
            if(len(cargo) > 0):
                # Get next token (i.e. character) and remove it from the string
                token = cargo[0]
                cargo = cargo[1:]
                found = False

                # Compare token with valid transition tokens for the state
                for tn in self.tok_next[state]:
                    if token in tn.tokens:
                        # Token is valid, jump to next state
                        state = tn.next_state
                        found = True
                        break
                        
                if not found:
                    # Token not found, switch to ERROR state
                    err = "Got " + token + " in state " + state
                    state = "ERROR"                    
            else:
                # No more characters left, could be valid if in * state
                # * in the state name means it is a valid place to end
                if '*' in state:
                    state = "GOOD"
                else:
                    err = "More needed."
                    state = "ERROR"
                    
            if state == "GOOD":
                print(orig_num + " is good.")
                break
            if state == "ERROR":
                print(orig_num + " is bad: " + err)
                break

fsm = myFSM()
fsm.add_state("START", "1234567890", "SECOND_DIGIT_ONWARDS*")
fsm.add_state("START", "-", "AFTER_MINUS")
fsm.add_state("AFTER_MINUS", "1234567890", "SECOND_DIGIT_ONWARDS*")
fsm.add_state("SECOND_DIGIT_ONWARDS*", "1234567890", "SECOND_DIGIT_ONWARDS*")
fsm.add_state("SECOND_DIGIT_ONWARDS*", ".", "AFTER_DOT")
fsm.add_state("AFTER_DOT", "1234567890", "MANTISSA*")
fsm.add_state("MANTISSA*", "1234567890", "MANTISSA*")

fsm.run("3.14")
fsm.run("-7")
fsm.run("-22.0")
fsm.run("--22.0")
fsm.run("-22.a0")
fsm.run("-1.")
fsm.run("-")

