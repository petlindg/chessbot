#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "chess.h"
#include "interface.h"
#include "tests.h"

void randomTestGame() {
    int t = time(NULL);
    srand(t);

    Piece board[8][8];
    Move* moves;
    unsigned int size=1, i=1, r;
    Color color;
    Move move;

    initBoard(board);
    printBoard(board);

    while(size) {
        if(i%2) {
            color = BLACK;
        } else {
            color = WHITE;
        }
        size = getMoves(board, color, &moves);
        r = rand()%size;
        movePiece(board, moves[r]);
        printBoard(board);
        printMove(moves[r]);
        free(moves);
        i++;
        if(i>1000) break;
    }
}

bool test_verifyNotCheck() {
    int t = time(NULL);
    srand(t);

    Piece board[8][8];
    Move* moves;
    unsigned int r, size=1, i=0;
    Color color;
    Move move;
    initBoard(board);

    for(unsigned int k=0; k<1000; k++) {

        initBoard(board);
        i=0;

        while(1) {
            if(i%2) {
                color = BLACK;
            } else {
                color = WHITE;
            }
            size = getMoves(board, color, &moves);
            if(size) {
                r=rand() % size;
            }
            movePiece(board, moves[r]);
            free(moves);
            if(i>1000||size==0) {
                break;
            } else if(isCheck(board, color)) {
                printf("Test failed. color {");
                if(color==WHITE) {
                    printf("WHITE");
                } else {
                    printf("BLACK");
                }
                printf("} still in check after moving\n");
                printMove(moves[r]);
                printf("\n");
                printBoard(board);
                printf("seed:%d\n", t);
                printf("k:%d\n", k);
                return false;
            }
            i++;
        }
    }
    printf("Test succeeded.\n");
    return true;
}