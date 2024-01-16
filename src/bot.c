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

//returns true if it finds a path with higher evaluation
bool getBestMove_r(Piece board[8][8],
                   Color playColor,
                   Color otherColor,
                   int depth,
                   int* eval) {

    if(depth<=0) {
        int newEval = evalBoard(board, playColor);
        if(newEval>=*eval) {
            *eval=newEval;
            return true;
        }
        return false;
    }

    Piece copiedBoard[8][8];
    Move* moves = malloc(10*sizeof(Move));
    int size = getMoves(board, playColor, &moves);

    if(!size) {
        free(moves);
        return false;
    }

    for(int i=0; i<size; i++) {
        copyBoard(board, copiedBoard);
        movePiece(copiedBoard, moves[i]);
        if(getBestMove_r(copiedBoard, otherColor, playColor, depth-1, eval)) {
            free(moves);
            return true;
        }
    }
    free(moves);
    return false;
}

// find best move brute force depth first
// bool color 0 white 1 black
Move getBestMove(Piece board[8][8],
                  Color playColor,
                  int depth) {

    Piece copiedBoard[8][8];
    Move move;
    int eval = -2000;
    Move* moves = malloc(10*sizeof(Move));
    int size = getMoves(board, playColor, &moves);

    Color otherColor;
    if(playColor==WHITE) {
        otherColor=BLACK;
    } else {
        otherColor=WHITE;
    }

    if(!size) {
        free(moves);
        printf("checkmate\n");
        //freeze, not solved
        char c = getchar();
    }


    for(int i=0; i<size; i++) {
        copyBoard(board, copiedBoard);
        movePiece(copiedBoard, moves[i]);
        if(getBestMove_r(copiedBoard, playColor, otherColor, depth-1, &eval)) {
            move=moves[i];

            /*
            printf("i:%d\n", i);
            printf("playcolor:");
            if(playColor=WHITE){
                printf("WHITE\n");
            }
            else {
                printf("BLACK\n");
            }
            printf("eval:%d\n", eval);

            printf("size:%d\n", size);
            printf("move:");
            printMove(move);

            int c = getchar();
            */
        }
    }
    free(moves);
    return move;
}


/*
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
            */

            //int c = getchar();