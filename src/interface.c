#include "types.h"
#include "interface.h"
#include <stdio.h>

void printPiece(Piece* piece) {
    switch(piece->pieceType) {
        //empty square
        case EMPTY:
            printf("..");
            break;
        //white pieces
        case PAWN:
            if(piece->color==WHITE) {
                printf("wP");
                break;
            }
            printf("bP");
            break;
        case KNIGHT:
            if(piece->color==WHITE) {
                printf("wk");
                break;
            }
            printf("bk");
            break;
        case BISHOP:
            if(piece->color==WHITE) {
                printf("wB");
                break;
            }
            printf("bB");
            break;
        case ROOK:
            if(piece->color==WHITE) {
                printf("wR");
                break;
            }
            printf("bR");
            break;
        case QUEEN:
            if(piece->color==WHITE) {
                printf("wQ");
                break;
            }
            printf("bQ");
            break;
        case KING:
            if(piece->color==WHITE) {
                printf("wK");
                break;
            }
            printf("bK");
            break;
    }
}

void printBoard(Piece board[8][8]) {
    printf("-------------------------------\n\n");
        for(int i=7; i>=0; i--) {
            printf("%d  ", i+1);
            for(int j=7; j>=0; j--) {
            printf(" ");
            printPiece(&board[j][i]);
            }
            printf("\n");
        }
    printf("\n    A  B  C  D  E  F  G  H\n");
    printf("-------------------------------\n");
}
