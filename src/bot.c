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

    //printf("eval:%d depth:%d\n", *eval, depth);
    
    if(depth<=0) {
        int newEval = evalBoard(board, playColor);
        //printBoard(board);
        //printf("eval:%d newEval:%d\n", *eval, newEval);
        if(newEval>*eval) {
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

    //printMoves(moves, size);
    for(int i=0; i<size; i++) {
        copyBoard(board, copiedBoard);
        movePiece(copiedBoard, moves[i]);
        //printBoard(copiedBoard);
        
        *eval=-*eval;
        bool newBestMove = getBestMove_r(copiedBoard, otherColor, playColor, depth-1, eval);
        *eval=-*eval;

        if(newBestMove) {
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
                  int depth,
                  Move* moves,
                  int size) {

    Piece copiedBoard[8][8];
    Move move;
    int eval = 2000;

    Color otherColor;
    if(playColor==WHITE) {
        otherColor=BLACK;
    } else {
        otherColor=WHITE;
    }

    for(int i=0; i<size; i++) {
        copyBoard(board, copiedBoard);
        movePiece(copiedBoard, moves[i]);

        eval = -eval;
        bool newBestMove = getBestMove_r(copiedBoard, otherColor, playColor, depth-1, &eval);
        eval = -eval;

        if(newBestMove) {
            printf("i:%d\n", i);
            printMove(moves[i]);
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