#include "types.h"
#include "interface.h"
#include "board.h"
#include <stdio.h>
#include <stdbool.h>

#include <unistd.h>

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
                for(int j=0; j<8; j++) {
                printf(" ");
                printPiece(&board[j][i]);
            }
            printf("\n");
        }
    printf("\n    A  B  C  D  E  F  G  H\n");
    printf("-------------------------------\n");
}

void printMoves(Move* moves, int size) {
    for(int i=0; i<size; i++) {
        printMove(moves[i]);
    }
    return;
}

void printColor(Color color) {
    if(color==WHITE) {
        printf("WHITE");
    } else {
        printf("BLACK");
    }
    return;
}

void printPieceType(PieceType pieceType) {
    switch(pieceType) {
        case EMPTY:
            printf("EMPTY");
            return;
        case PAWN:
            printf("PAWN");
            return;
        case KNIGHT:
            printf("KNIGHT");
            return;
        case BISHOP:
            printf("BISHOP");
            return;
        case ROOK:
            printf("ROOK");
            return;
        case QUEEN:
            printf("QUEEN");
            return;
        case KING:
            printf("KING");
            return;
    }
    printf("NaN");
    return;
}

void printMove(Move move) {

    printf("(%d, %d) -> (%d, %d) ",
        move.from.x, move.from.y, move.to.x, move.to.y);
    printPieceType(move.promoteTo);
    printf("\n");
    return;
}