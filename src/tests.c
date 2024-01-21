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

void test_findBestMove() {
    Piece board[8][8];
    Move* moves;
    unsigned int size=1, i=0;
    Color color;
    Move move;
    int evalBest, evalCurrent;

    initBoard(board);

    while(size) {
        moves = malloc(10*sizeof(Move));
        if(i%2) {
            color = BLACK;
        } else {
            color = WHITE;
        }
        size = getMoves(board, color, &moves);
        if(i>=25||!size) {
            free(moves);
            break;
        }
        move = getBestMove(board, color, 5, moves, size);
        movePiece(board, move);
        //printMoves(moves, size);
        free(moves);
        i++;
        printBoard(board);
        printMove(move);
    }

    printf("Amount of moves: %d\n", i);
    return;
}

void test_playFindBestMove() {
    Piece board[8][8];
    Move* moves;
    unsigned int size=1, i=0;
    Color color;
    Move move;
    int evalBest, evalCurrent;

    //initBoard(board);
    test_initTestBoard(board);

    printBoard(board);

    while(size) {
        moves = malloc(10*sizeof(Move));
        if(i%2) {
            color = BLACK;
            size = getMoves(board, color, &moves);
            move = getBestMove(board, color, 1, moves, size);
        } else {
            color = WHITE;
            size = getMoves(board, color, &moves);
            move = promptMove(moves, size);
        }

        if(i>=200||!size) {
            free(moves);
            break;
        }

        movePiece(board, move);

        free(moves);
        i++;
        printBoard(board);
        printMove(move);
    }

    printf("Amount of moves: %d\n", i);
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

void test_MCTS() {

    //int t = time(NULL);
    int t = 0;
    srand(t);

    Piece board[8][8];
    int i=0;
    Color color;
    Move move;

    //test_initTestBoard(board);
    initBoard(board);

    Node* root = malloc(sizeof(Node));
    Node* node = root;
    initNode(board, WHITE, node);

    printBoard(board);

    while(node->numberOfChildren) {
        if(i%2) {
            color = BLACK;
            printf("Color: BLACK\n");
        } else {
            color = WHITE;
            printf("Color: WHITE\n");
        }

        move = MCTS(board, color, 100, &node);
        movePiece(board, move);
        if(i>=250) {
            break;
        }
        
        i++;
        printf("i:%d\n", i);
        printMove(move);
        printBoard(board);
    }

    freeTree(root);
    
    printf("Amount of moves: %d\n", i);
    return;
}