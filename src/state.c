#include "types.h"
#include "board.h"

void initState(State* state) {
    initBoard(state->board);
    state->moveListSize = 1;
    state->playColor    = WHITE;
    state->oppColor     = BLACK;
    state->moveCount    = 0;
}