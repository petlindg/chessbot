#include "types.h"
#include "interface.h"
#include "chess.h"
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
    int x1, y1, x2, y2;
    for(int i=0; i<size; i++) {
        squareToInt(moves[i].from, &x1, &y1);
        squareToInt(moves[i].to, &x2, &y2);
        printf("(%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);
    }
    return;
}

Square stringToSquare(char c1, char c2) {
    switch(c1) {
        case 'a': {
            switch(c2) {
                case '1':
                    return A1;
                case '2':
                    return A2;
                case '3':
                    return A3;
                case '4':
                    return A4;
                case '5':
                    return A5;
                case '6':
                    return A6;
                case '7':
                    return A7;
                case '8':
                    return A8;
            }
        }
        case 'b': {
            switch(c2) {
                case '1':
                    return B1;
                case '2':
                    return B2;
                case '3':
                    return B3;
                case '4':
                    return B4;
                case '5':
                    return B5;
                case '6':
                    return B6;
                case '7':
                    return B7;
                case '8':
                    return B8;
            }
        }
        case 'c': {
            switch(c2) {
                case '1':
                    return C1;
                case '2':
                    return C2;
                case '3':
                    return C3;
                case '4':
                    return C4;
                case '5':
                    return C5;
                case '6':
                    return C6;
                case '7':
                    return C7;
                case '8':
                    return C8;
            }
        }
        case 'd': {
            switch(c2) {
                case '1':
                    return D1;
                case '2':
                    return D2;
                case '3':
                    return D3;
                case '4':
                    return D4;
                case '5':
                    return D5;
                case '6':
                    return D6;
                case '7':
                    return D7;
                case '8':
                    return D8;
            }
        }
        case 'e': {
            switch(c2) {
                case '1':
                    return E1;
                case '2':
                    return E2;
                case '3':
                    return E3;
                case '4':
                    return E4;
                case '5':
                    return E5;
                case '6':
                    return E6;
                case '7':
                    return E7;
                case '8':
                    return E8;
            }
        }
        case 'f': {
            switch(c2) {
                case '1':
                    return F1;
                case '2':
                    return F2;
                case '3':
                    return F3;
                case '4':
                    return F4;
                case '5':
                    return F5;
                case '6':
                    return F6;
                case '7':
                    return F7;
                case '8':
                    return F8;
            }
        }
        case 'g': {
            switch(c2) {
                case '1':
                    return G1;
                case '2':
                    return G2;
                case '3':
                    return G3;
                case '4':
                    return G4;
                case '5':
                    return G5;
                case '6':
                    return G6;
                case '7':
                    return G7;
                case '8':
                    return G8;
            }
        }
        case 'h': {
            switch(c2) {
                case '1':
                    return H1;
                case '2':
                    return H2;
                case '3':
                    return H3;
                case '4':
                    return H4;
                case '5':
                    return H5;
                case '6':
                    return H6;
                case '7':
                    return H7;
                case '8':
                    return H8;
            }
        }
    }
}

void printMove(Move move) {
    int x1, y1, x2, y2;
    squareToInt(move.from, &x1, &y1);
    squareToInt(move.to, &x2, &y2);
    printf("(%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);
}

void stringToMove(Move* move, char* str) {
    move->from=stringToSquare(str[0], str[1]);
    move->to=stringToSquare(str[2], str[3]);
    return;
}

void promptMove(Move* move) {
    char input[4];
    scanf("%4s", input);
    stringToMove(move, input);
    for(int i=0; i<4; i++) {
        input[i]=0;
    }
    return;
}