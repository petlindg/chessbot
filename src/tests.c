#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "board.h"
#include "tests.h"
#include "interface.h"
#include "move.h"
#include "check.h"
#include "bot.h"

void test_checkGame(int seed) {
    srand(seed);
    int input = 0;

    Piece board[8][8];
    Move* moves = malloc(10*sizeof(Move));
    unsigned int size=1, i=0, r;
    Color color;

    initBoard(board);

    while(size) {
        moves = malloc(10*sizeof(Move));
        if(i%2) {
            color = BLACK;
        } else {
            color = WHITE;
        }
        size = getMoves(board, color, &moves);
        if(i>=1000||!size) {
            break;
        }
        r = rand()%size;
        movePiece(board, moves[r]);
        printMove(moves[r]);
        free(moves);
        i++;
        printBoard(board);
        
        input = 0;
    }
    printf("Amount of moves: %d\n", i);
    free(moves);
    return;
}

void test_randomTestGame() {
    int t = time(NULL);
    //int t = 0;
    srand(t);

    Piece board[8][8];
    Move* moves = malloc(10*sizeof(Move));
    unsigned int size=1, i=0, r;
    Color color;

    initBoard(board);

    while(size) {
        moves = malloc(10*sizeof(Move));
        if(i%2) {
            color = BLACK;
        } else {
            color = WHITE;
        }
        size = getMoves(board, color, &moves);
        if(i>=1000||!size) {
            break;
        }
        r = rand()%size;
        movePiece(board, moves[r]);
        free(moves);
        i++;
    }
    printBoard(board);
    printMove(moves[r]);
    printf("Amount of moves: %d\n", i);
    printf("Seed: %d\n", t);
    free(moves);
    return;
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

void test_memory() {

}

void test_initTestBoard(Piece board[8][8]) {

    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            initPiece_Empty(&board[i][j]);
        }
    }

    initPiece(&board[0][4], PAWN, WHITE);
    initPiece(&board[7][7], ROOK, BLACK);
    return;
}

void test_copyBoard() {
    Piece board[8][8];
    initBoard(board);
    printf("board before copy:\n");
    printBoard(board);

    Piece copiedBoard[8][8];
    copyBoard(board, copiedBoard);
    printf("board after copy:\n");
    printBoard(board);
    printf("copied board:\n");
    printBoard(copiedBoard);
    
    return;
}