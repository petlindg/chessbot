#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdlib.h>

#include "types.h"
#include "chess.h"
#include "interface.h"

int main(){

    srand(time(NULL));
    Piece board[8][8];
    Move* moves;
    unsigned int r, size=1, i=0;
    Color color;
    Move move;
    bool hasMoved = false;
    initBoard(board);
    
    int iterations = 10;

    printBoard(board);



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

    /*
    // game loop of random moves until one color wins
    while(size!=0) {
        if(i%2) {
            color = BLACK;
        } else {
            color = WHITE;
        }
        size = getMoves(board, color, &moves);
        r=rand() % size;
        movePiece(board, moves[r]);
        printBoard(board);
        free(moves);
        if(i>1000) break;
        printf("Size:%d, i:%d\n", size, i);
        i++;
    }
    */
}