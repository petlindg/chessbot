#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "chess.h"
#include "interface.h"
#include "tests.h"

void test_randomTestGame() {
    int t = time(NULL);
    srand(t);

    Piece board[8][8];
    Move* moves = malloc(10*sizeof(Move));
    unsigned int size=1, i=0, r;
    Color color;

    initBoard(board);
    printBoard(board);

    while(size) {
        moves = malloc(10*sizeof(Move));
        if(i%2) {
            color = BLACK;
        } else {
            color = WHITE;
        }
        printf("1\n");
        size = getMoves(board, color, &moves);
        printf("2\n");
        if(i>1000||!size) break;
        r = rand()%size;
        movePiece(board, moves[r]);
        printBoard(board);
        printMove(moves[r]);
        free(moves);
        i++;
    }
}

bool test_verifyNotCheck() {
    int t = time(NULL);
    srand(t);

    Piece board[8][8];
    Move* moves;
    unsigned int r, size=1, i=0;
    Color color;
    initBoard(board);

    for(unsigned int k=0; k<1000; k++) {
        printBoard(board);
        initBoard(board);
        i=0;
        printf("k:%d\n", k);

        while(1) {
            moves = malloc(10*sizeof(Move));
            if(i%2) {
                color = BLACK;
            } else {
                color = WHITE;
            }
            size = getMoves(board, color, &moves);
            if(i>1000||size==0) {
                free(moves);
                break;
            }
            if(size) {
                r=rand() % size;
            }
            movePiece(board, moves[r]);
            i++;

            //doublechecks position
            if(isCheck(board, color)) {
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
            free(moves);
        }
    }
    printf("Test succeeded.\n");
    return true;
}