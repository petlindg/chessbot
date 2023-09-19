#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdlib.h>

#include "types.h"
#include "chess.h"
#include "interface.h"

int main(){

    srand(time(NULL));
    Piece board[8][8];
    Move* moves;
    unsigned int r, size;
    initBoard(board);
    
    int iterations = 10;

    for(int i=0; i<iterations; i++) {
        printBoard(board);
        size = getMoves(board, WHITE, &moves);
        printf("Size: %d\n", size);
        r=rand()%size;
        movePiece(board, moves[r]);
        free(moves);
        sleep(1);
        printBoard(board);
        size = getMoves(board, BLACK, &moves);
        printf("Size: %d\n", size);
        r=rand()%size;
        movePiece(board, moves[r]);
        free(moves);
        sleep(1);
    }
}