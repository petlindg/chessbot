#include <stdio.h>

#include <stdlib.h>
#include <stdbool.h>
#include "types.h"
#include "chess.h"
#include "interface.h"
#include "move.h"


int evalPiece(Piece piece) {
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
    if(piece.color==BLACK) {
        val*=-1;
    }
    return val;
}

//positive number favor white, negative favor black
int evalBoard(Piece board[8][8], Color color) {
    Move* moves = malloc(10*sizeof(Move));
    if(getMoves(board, color, &moves) == 0) {
        if(color==WHITE) {
            return -10000;
        }
        return 10000;
    }
    int val = 0;
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            val+=evalPiece(board[i][j]);
        }
    }
    return val;
}

/*
// find best move brute force depth first
Move findBestMove(Piece board[8][8], Color playColor, Color moveColor, Move* moves, int movesSize, int depth, Move* moveSeq) {
    if(depth<=0) return;
    Piece boardClone[8][8];
    for(int i=0; i<movesSize; i++) {
        copyBoard(board, boardClone);
        movePiece(boardClone, moves[i])
        findBestMove

        
    }

}
*/