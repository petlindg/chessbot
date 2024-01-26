#include <stdio.h>

#include "types.h"
#include "interface.h"
#include "bot.h"

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

void printMoveCoords(Move move) {

    printf("(%d, %d) -> (%d, %d) ",
        move.from.x, move.from.y, move.to.x, move.to.y);
    printPieceType(move.promoteTo);
    printf("\n");
    return;
}

void printMove(Move move) {

    char x1, x2;

    switch(move.from.x) {
        case 0:
            x1='A';
            break;
        case 1:
            x1='B';
            break;
        case 2:
            x1='C';
            break;
        case 3:
            x1='D';
            break;
        case 4:
            x1='E';
            break;
        case 5:
            x1='F';
            break;
        case 6:
            x1='G';
            break;
        case 7:
            x1='H';
            break;
        default:
            printf("ERROR: file interface.c function printMove\n");
            printf("Invalid coordinate.\n");
            printf("move.from.x = %d\n", move.from.x);
    }

    switch(move.to.x) {
        case 0:
            x2='A';
            break;
        case 1:
            x2='B';
            break;
        case 2:
            x2='C';
            break;
        case 3:
            x2='D';
            break;
        case 4:
            x2='E';
            break;
        case 5:
            x2='F';
            break;
        case 6:
            x2='G';
            break;
        case 7:
            x2='H';
            break;
        default:
            printf("ERROR: file interface.c function printMove\n");
            printf("Invalid coordinate.\n");
            printf("move.to.x = %d\n", move.to.x);
    }

    printf("%c%d to %c%d, promote to ", x1, move.from.y+1, x2, move.to.y+1);
    printPieceType(move.promoteTo);
    printf("\n");

    return;
}

Move promptMove(Move* moves, int size) {
    printf("Make your move.\n");

    while(1) { 
        int x1, y1, x2, y2;
        scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
        
        Move move;
        move.from.x=x1; move.from.y=y1; move.to.x=x2; move.to.y=y2;

        for(int i=0; i<size; i++) {
            if(move.from.x == moves[i].from.x &&
            move.from.y == moves[i].from.y &&
            move.to.x   == moves[i].to.x   &&
            move.to.y   == moves[i].to.y) {
                return move;
            }
        }
        printf("Invalid move, try again.\n");
        printf("Move: ");
        printMove(move);
    }
}