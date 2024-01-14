#include <stdio.h>

#include <stdlib.h>
#include <stdbool.h>
#include "types.h"
#include "chess.h"
#include "interface.h"
#include "move.h"


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
    Move* moves = malloc(10*sizeof(Move));
    if(getMoves(board, color, &moves) == 0) {
        return -10000;
    }
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
Move findBestMove(Piece board[8][8],
                  Color playColor,
                  Color otherColor,
                  Move* moves,
                  int movesSize,
                  int depthCur,
                  int depthMax,
                  int* evalBest,
                  int* evalCurr,
                  Move* pathBest,
                  Move* pathCurr) {
    if(depthCur<=0) {
        *evalCurr = evalBoard(board, playColor);
        if(*evalCurr>*evalBest) {
            *evalBest=*evalCurr;
            for(int i = 0; i<depthMax; i++) {
                pathBest[i]=pathCurr[i];
            }
        }
        return;
    }
    Piece boardClone[8][8];
    for(int i=0; i<movesSize; i++) {
        copyBoard(board, boardClone);
        movePiece(boardClone, moves[i]);
        Move* movesOther = malloc(10*sizeof(Move));
        int movesSizeOther = getMoves(boardClone, otherColor, movesOther);
        if(movesSizeOther==0) {
            *evalBest==10000;
            return pathCurr[0];
        }
        findBestMove(boardClone, otherColor, playColor, movesOther, movesSizeOther, depth-1);



        
    }

}
