#include <stdio.h>

#include <stdbool.h>
#include <stdlib.h>

#include "types.h"
#include "chess.h"
#include "interface.h"

int main(){
    Piece board[8][8];
    initBoard(board);

    printBoard(board);
    printf("0\n");
    Move* moves;
    int size = getMoves(board, WHITE, moves);
    printf("Size: %d\n", size);
    printMoves(moves, size);


}