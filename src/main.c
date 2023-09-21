#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdlib.h>

#include "types.h"
#include "chess.h"
#include "interface.h"

int main(){
    int t = time(NULL);

    srand(t);
    Piece board[8][8];
    Move* moves;
    unsigned int r, size=1, i=0;
    Color color;
    Move move;
    initBoard(board);
    
    int iterations = 10;

    printBoard(board);


    /*
    while(size!=0) {
        if(i%2) {
            color = BLACK;
            size = getMoves(board, color, &moves);
            r=rand() % size;
            movePiece(board, moves[r]);
        } else {
            color = WHITE;
            size = getMoves(board, color, &moves);
            while(1) {
                promptMove(&move);
                printMove(move);
                for(int j=0; j<size; j++) {
                    if(moves[j].from==move.from&&moves[j].to==move.to) {
                        movePiece(board, move);
                        hasMoved=true;
                        break;
                    }
                }
                if(hasMoved) {
                    hasMoved=false;
                    break;
                }
                printf("invalid move, make a new one\n");
            }
        }
        
        printBoard(board);
        free(moves);
        printf("Size:%d, i:%d\n", size, i);
        if(i>1000) break;
        i++;
    }
    */

    for(unsigned int k=0; k<1000000; k++) {
        // game loop of random moves until one color wins
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
                exit(0);
            }
            i++;
        }
    }
    printf("Test succeeded\n");
    printf("seed:%d\n", t);
}