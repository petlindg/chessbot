#include <stdio.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdlib.h>

#include "types.h"
#include "chess.h"
#include "interface.h"
#include "tests.h"

int main(){
    test_verifyNotCheck();

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
}