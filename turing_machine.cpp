/* 
    by pylNeuralNet
    2023 PŁ
*/

// Theta = #

/*
    Turing Machine simulation incrementing a given number by a given value
    Problem statement: multi digit (up to 3 digits) decimal number, increment by 5

    | State | 0     | 1     | 2     | 3     | 4     | 5     | 6     | 7     | 8     | 9     | #     |
    |-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|
    | q0    | 5,L,q1| 6,L,q1| 7,L,q1| 8,L,q1| 9,L,q1| 0,L,q2| 1,R,q2| 2,L,q2| 3,L,q2| 4,L,q2| -,L,- |
    | q1    | 0,L,q3| 1,L,q3| 2,L,q3| 3,L,q3| 4,L,q3| 5,L,q4| 6,L,q4| 7,L,q4| 8,L,q4| 9,L,q4| -,L,- |
    | q2    | 1,L,q3| 2,L,q3| 3,L,q3| 4,L,q3| 5,L,q3| 6,L,q4| 7,L,q4| 8,L,q4| 9,L,q4| 0,L,q4| 1,L,- |
    | q3    | 0,L,q5| 1,L,q5| 2,L,q5| 3,L,q5| 4,L,q5| 5,L,q6| 6,L,q6| 7,L,q6| 8,L,q6| 9,L,q6| -,L,- |
    | q4    | 1,L,q5| 2,L,q5| 3,L,q5| 4,L,q5| 5,L,q5| 6,L,q6| 7,L,q6| 8,L,q6| 9,L,q6| 0,L,q6| 1,-,- |
    | q5    | -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,- |
    | q6    | -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| -,-,q7| 1,-,- |
    | q7    | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- | -,-,- |

    alternatively, if the digits number check should be performed first,
    before writing anything, the TM could check whether the requirement that the number should have less than 3 digits is met, and if it is, start writing from theta right-wise or move back to the end and start left-wise
*/

/*  Optional:
    To enable writing Theta to cmd:
    - open CMD, type:1998
    - `chcp 65001`
    - `set PYTHONIOENCODING=utf-8`
    - `chcp 437` to undo
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

enum class Direction {
    L, R, x
};

struct Transition {
    int nextState;
    char writeSymbol;
    Direction direction;
};

struct MT {
    int currentState;
    int readSymbol;
    Transition transition;
};

inline int thetaColumn = 10;

std::vector<std::vector<Transition>> transitionTableForGrade3 = {
    {{0, '0', Direction::L}, {0, '1', Direction::L}, {0, '1', Direction::L} }
};


std::vector<int> acceptingStates = {1, 2, 3, 4, 5, 6};

// -1 in theta column in State means "-"
std::vector<std::vector<Transition>> transitionTableForGrade5 = {
    // q0 - first encountered symbol: either [0-9] or theta. If [0-9], increments by 5 and transitions state to q1 (without carry) or q2 (with carry), if theta - moves head left in an attempt to find a [0-9]
    // q1 - encountered 1 digit by far. writes back the encountered digit because there was no carry. Moves to an analogous state that means 2 digits have been read - one for carry and one without carry (q3/q4), if theta - no more action is taken (-,-,-) because the number has been incremented successfuly and the number has less than 3 digits
    // q2 - encountered 1 digits by far. writes the encountered digit increased by 1 because there was carry. Moves to an analogous state that means 2 digits have been read (q3/q4). if theta - overwrites theta with the carry and no more action is taken (1,-,-)
    // q3 - encountered 2 digits by far. writes back the encountered digit. the rest similarly to q1
    // q4 - encountered 2 digits by far. writes the encountered digit increased by 1 because there was carry. the rest similarly to q2
    // q5 - encountered 3 digits by far. if encountered a digit then moves to q7 which is an end state and no more action is done. if encountered theta then no more action is done -,-,- as the increment of a 3 digits number has succeeded
    // q6 - encountered 3 digits by far. if encountered a digit then moves to q7 which is an end state and no more action is done. if encountered theta then overwrites it with 1 (carry) and no more action is done (1,-,-)
    // q7 - encountered 4 digits, increment is not being performed, no more action is taken (-,-,-). this state is added to be able to discern if the end state is accepting or not.
    // rows: states [q0-q7]
    // columns: [0-9], theta

    //q0
    {{1, '5', Direction::L}, {1, '6', Direction::L}, {1, '7', Direction::L}, {1, '8', Direction::L}, {1, '9', Direction::L}, {2, '0', Direction::L}, {2, '1', Direction::R}, {2, '2', Direction::L}, {2, '3', Direction::L}, {2, '4', Direction::L}, {-1, '-', Direction::L}},
     // q1
    {{3, '0', Direction::L}, {3, '1', Direction::L}, {3, '2', Direction::L}, {3, '3', Direction::L}, {3, '4', Direction::L}, {4, '5', Direction::L}, {4, '6', Direction::L}, {4, '7', Direction::L}, {4, '8', Direction::L}, {4, '9', Direction::L}, {-1, '-', Direction::L}},
    // q2
    {{3, '1', Direction::L}, {3, '2', Direction::L}, {3, '3', Direction::L}, {3, '4', Direction::L}, {3, '5', Direction::L}, {4, '6', Direction::L}, {4, '7', Direction::L}, {4, '8', Direction::L}, {4, '9', Direction::L}, {4, '0', Direction::L}, {-1, '1', Direction::L}},
    // q3
    {{5, '0', Direction::L}, {5, '1', Direction::L}, {5, '2', Direction::L}, {5, '3', Direction::L}, {5, '4', Direction::L}, {6, '5', Direction::L}, {6, '6', Direction::L}, {6, '7', Direction::L}, {6, '8', Direction::L}, {6, '9', Direction::L}, {-1, '-', Direction::L}},
    // q4
    {{5, '1', Direction::L}, {5, '2', Direction::L}, {5, '3', Direction::L}, {5, '4', Direction::L}, {5, '5', Direction::L}, {6, '6', Direction::L}, {6, '7', Direction::L}, {6, '8', Direction::L}, {6, '9', Direction::L}, {6, '0', Direction::L}, {-1, '1', Direction::x}},
    // q5
    {{7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {-1, '-', Direction::x}},
    // q6
    {{7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {7, '-', Direction::x}, {-1, '1', Direction::x}},
    // q7
    {{-1, '-', Direction::x}, {-1, '-', Direction::x}, {-1, '-', Direction::x}, {-1, '-', Direction::x}, {-1, '-', Direction::x}, {-1, '-', Direction::x}, {-1, '-', Direction::x}, {-1, '-', Direction::x}, {-1, '-', Direction::x}, {-1, '-', Direction::x}, {-1, '-', Direction::x}}
};

std::string directionToString(Direction direction) {
    if(direction == Direction::L) return "L";
    else if(direction == Direction::R) return "R";
    else return "-";
}

std::string stateToString(int state) {
    return (state == -1) ? "-" : "q" + std::to_string(state);
}

std::string getInitialTapeContents() {
    std::string tape;
    std::cout << "Enter Theta (as #) followed by a single-digit binary number to increment: ";
    std::cin >> tape;
    return tape;
}

void printState(int currentState, int readSymbol, int nextState, char writeSymbol, Direction direction) {
    std::string readSymbolAsStr = readSymbol == 35 ? "#" : std::to_string(readSymbol);
    std::cout << "MT(" << stateToString(currentState) << ", " << readSymbolAsStr << ", " << stateToString(nextState) << "," << writeSymbol << "," << directionToString(direction) << ")\n";
}

void printTuringMachine(const MT& tm) {
    printState(tm.currentState, tm.readSymbol, tm.transition.nextState, tm.transition.writeSymbol, tm.transition.direction);
}

void printPath(const std::vector<MT>& path) {
    std::cout << "Path:\n";
    int cnt = 0;
    for (const MT& tm : path) {
        std::cout << ++cnt << ". ";
        printTuringMachine(tm);
    }
    std::cout << "\n";
}

void printFinalState(const MT& finalState) {
    std::cout << "Final state: " << stateToString(finalState.currentState) << std::endl;
    
    if(std::find(acceptingStates.begin(), acceptingStates.end(), finalState.currentState) != acceptingStates.end()) {
        std::cout << "The final state is an accepting state." << std::endl;
    } else {
        std::cout << "The final state is not an accepting state." << std::endl;
    }
}

void moveHead(int& head, Direction direction, std::string& tape) {
    if (direction == Direction::L) {
        if (head == 0) return;
        --head;
    } else if (direction == Direction::R) {
        if (head == tape.size() - 1) tape += '.'; // . here is a temporary symbol that increases the string that simulates an infite tape, allocating some memory for it, but this is the part of this TM design here
        ++head;
    }
}

void writeSymbol(Transition& transition, std::string& tape, int head) {
    if (transition.writeSymbol != '-') {
        tape[head] = transition.writeSymbol;
    }
}

// -,-,-
// simulates a moment in which Turing Machine will reach an inactive state - this is to break the loop in the program
// probably more inline with Theory of Automata would be to compare currentState with a list of finalStates, in this case q7, but doing it this way finishes the probram more universally
bool isHeadInInactiveState(const std::vector<MT>& path) {
    return path.back().transition.nextState == -1 && 
        path.back().transition.writeSymbol == '-' &&
        path.back().transition.direction == Direction::x;
}

int main() {
    std::string tape = getInitialTapeContents();

    int state = 0;
    int head = tape.size() - 1;

    std::vector<MT> path;

    while (true) {
        char readSymbol = tape[head];
        int index = readSymbol == '#' ? thetaColumn : readSymbol - '0';
        Transition transition = transitionTableForGrade5[state][index];

        writeSymbol(transition, tape, head);
        moveHead(head, transition.direction, tape);

        MT tm = {state, (readSymbol == '#' ? readSymbol : readSymbol - '0'), transition};
        path.push_back(tm);

        printState(state, tm.readSymbol, transition.nextState, transition.writeSymbol, transition.direction);

        if(readSymbol == '#' || isHeadInInactiveState(path)) break;

        state = transition.nextState;
    }

    printFinalState(path.back());
    printPath(path);

    std::cout << "Final tape state: " << tape << std::endl;

    return 0;
}
