#include <stdio.h>

#include <stdlib.h>
#include <stdbool.h>
#include "types.h"
#include "chess.h"
#include "interface.h"

void initPiece_Empty(Piece* piece) {
    piece->pieceType = EMPTY;
    piece->color = NONE;
    return;
}

void initPiece(Piece* piece, PieceType pieceType, Color color) {
    piece->pieceType = pieceType;
    piece->color = color;
    if(pieceType==ROOK||pieceType==KING) {
        piece->canSpecialMove = true;
    }
    if(pieceType==PAWN) {
        piece->canEnpassante_left=false;
        piece->canEnpassante_right=false;
    }
    return;
}

void initBoard(Piece board[8][8]) {
    //init line 8
    initPiece(&board[0][7], ROOK, BLACK);
    initPiece(&board[1][7], KNIGHT, BLACK);
    initPiece(&board[2][7], BISHOP, BLACK);
    initPiece(&board[3][7], QUEEN, BLACK);
    initPiece(&board[4][7], KING, BLACK);
    initPiece(&board[5][7], BISHOP, BLACK);
    initPiece(&board[6][7], KNIGHT, BLACK);
    initPiece(&board[7][7], ROOK, BLACK);
    //init line 7
    for(int i = 0; i<8; i++) {
        initPiece(&board[i][6], PAWN, BLACK);
    }
    //init line 3-6
    for(int i = 0; i<4; i++) {
        for(int j = 0; j<8; j++) {
            initPiece_Empty(&board[j][5-i]);
        }
    }
    //init line 2
    for(int i = 0; i<8; i++) {
        initPiece(&board[i][1], PAWN, WHITE);
    }
    //init line 1
    initPiece(&board[0][0], ROOK, WHITE);
    initPiece(&board[1][0], KNIGHT, WHITE);
    initPiece(&board[2][0], BISHOP, WHITE);
    initPiece(&board[3][0], QUEEN, WHITE);
    initPiece(&board[4][0], KING, WHITE);
    initPiece(&board[5][0], BISHOP, WHITE);
    initPiece(&board[6][0], KNIGHT, WHITE);
    initPiece(&board[7][0], ROOK, WHITE);
    return;
}

void findKing(Piece board[8][8], Color color, Square* kingPos) {
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if(board[i][j].pieceType==KING && board[i][j].color == color) {
                kingPos->x=i;
                kingPos->y=j;
                return;
            }
        }
    }
}

bool isOnBoard(Square square) {
    return (square.x>=0&&square.x<8)&&(square.y>=0&&square.y<8);
}