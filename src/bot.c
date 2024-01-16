#include <stdio.h>

#include <stdlib.h>
#include <stdbool.h>
#include "types.h"
#include "check.h"
#include "interface.h"
#include "move.h"
#include "board.h"


int evalPiece(Piece piece, Color color) {
    int val;
    switch(piece.pieceType) {
        case PAWN:
            val = 1;
            break;
        case BISHOP:
            val = 3;
            break;
        case KNIGHT:
            val = 3;
            break;
        case ROOK:
            val = 5;
            break;
        case QUEEN:
            val = 9;
            break;
        default:
            return 0;
    }
    if(piece.color != color) {
        val *= -1;
    }
    return val;
}

// evaluates gain in favor of color {color}
int evalBoard(Piece board[8][8], Color color) {
    int val = 0;
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            val+=evalPiece(board[i][j], color);
        }
    }
    return val;
}


// find best move brute force depth first
// bool color 0 white 1 black
void getBestMove(Piece board[8][8],
                  Color playColor,
                  Color otherColor,
                  int depth,
                  int* evalBest,
                  int* evalCurrent,
                  Move* moveBest,
                  Move* moveCurrent) {

    Move* moves = malloc(10*sizeof(Move));
    int movesSize = getMoves(board, playColor, &moves);

    if(depth<=0) {
        return;
    }

    if(!movesSize) {
        *evalCurrent = -1000;
        free(moves);
        return;
    }

    Piece boardClone[8][8];
    for(int i=0; i<movesSize; i++) {
        copyBoard(board, boardClone);
        movePiece(boardClone, moves[i]);
        getBestMove(boardClone, otherColor, playColor,
                     depth-1, evalBest, evalCurrent,
                     moveBest, moveCurrent);
        *evalCurrent=evalBoard(boardClone, playColor);
        if(*evalCurrent>*evalBest) {
            *evalBest=*evalCurrent;
            *moveBest=moves[i];

            printf("playcolor:");
            if(playColor=WHITE){
                printf("WHITE\n");
            }
            else {
                printf("BLACK\n");
            }
            printf("evalCurrent:%d\n", *evalCurrent);
            printf("evalBest:%d\n", *evalBest);

            printf("movesSize:%d\n", movesSize);
            printf("moveCurrent:");
            printMove(*moveCurrent);
            printf("moveBest:");
            printMove(*moveBest);

            int c = getchar();
        }
    }
    free(moves);
    return;
}
