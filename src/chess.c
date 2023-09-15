#include <stdio.h>

#include <stdbool.h>
#include "types.h"
#include "chess.h"

void initPiece_Empty(Piece* piece) {
    piece->pieceType = EMPTY;
    return;
}

void initPiece(Piece* piece, PieceType pieceType, Color color) {
    piece->pieceType = pieceType;
    piece->color = color;
    piece->canCastle = 1;
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

void findKing(Piece board[8][8], Color color, int* kingPosX, int* kingPosY) {
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if(board[i][j].pieceType==KING && board[i][j].color == color) {
                *kingPosX = i;
                *kingPosY = j;
                return;
            }
        }
    }
}

bool isOnBoard(int x, int y) {
    return (x>=0&&x<8)&&(y>=0&&y<8);
}

bool isCheck_Horizontal(Piece board[8][8], Color color, int kingPosX, int kingPosY) {
    //check left of king
    for(int i=kingPosX-1; i>=0; i--) {
        if((board[i][kingPosY].pieceType==ROOK ||
           board[i][kingPosY].pieceType==QUEEN) &&
          (board[i][kingPosY].color != color))
            return true;
        if(board[i][kingPosY].pieceType!=EMPTY) break;
    }
    //check right of king
    for(int i=kingPosX+1; i<8; i++) {
        if((board[i][kingPosY].pieceType==ROOK ||
           board[i][kingPosY].pieceType==QUEEN) &&
          (board[i][kingPosY].color != color))
            return true;
        if(board[i][kingPosY].pieceType!=EMPTY) break;
    }
    //check down of king
    for(int j=kingPosY-1; j>=0; j--) {
        if((board[kingPosX][j].pieceType==ROOK ||
           board[kingPosX][j].pieceType==QUEEN) &&
          (board[kingPosX][j].color != color))
            return true;
        if(board[kingPosX][j].pieceType!=EMPTY) break;
    }
    //check up of king
    for(int j=kingPosY+1; j<8; j++) {
        if((board[kingPosX][j].pieceType==ROOK ||
           board[kingPosX][j].pieceType==QUEEN) &&
          (board[kingPosX][j].color != color))
            return true;
        if(board[kingPosX][j].pieceType!=EMPTY) break;
    }
    return false;
}

bool isCheck_Vertical(Piece board[8][8], Color color, int kingPosX, int kingPosY) {
    //check down left of king
    for(int i=kingPosX-1, j=kingPosY-1; i>=0&&j>=0; i--, j--) {
        if((board[i][j].pieceType==BISHOP ||
           board[i][j].pieceType==QUEEN) &&
          (board[i][j].color != color))
            return true;
        if(board[i][j].pieceType!=EMPTY) break;
    }
    //check down right of king
    for(int i=kingPosX+1, j=kingPosY-1; i<8&&j>=0; i++, j--) {
        if((board[i][j].pieceType==BISHOP ||
           board[i][j].pieceType==QUEEN) &&
          (board[i][j].color != color))
            return true;
        if(board[i][j].pieceType!=EMPTY) break;
    }
    //check up left of king
    for(int i=kingPosX-1, j=kingPosY+1; i>=0&&j<8; i--, j++) {
        if((board[i][j].pieceType==BISHOP ||
           board[i][j].pieceType==QUEEN) &&
          (board[i][j].color != color))
            return true;
        if(board[i][j].pieceType!=EMPTY) break;
    }
    //check up right of king
    for(int i=kingPosX+1, j=kingPosY+1; i<8&&j<8; i++, j++) {
        if((board[i][j].pieceType==BISHOP ||
           board[i][j].pieceType==QUEEN) &&
          (board[i][j].color != color))
            return true;
        if(board[i][j].pieceType!=EMPTY) break;
    }
    return false;
}

bool isCheck_Pawn(Piece board[8][8], Color color, int kingPosX, int kingPosY) {
    if(color==WHITE) {
        if(isOnBoard(kingPosX-1, kingPosY+1)) {
            if(board[kingPosX-1][kingPosY+1].pieceType==PAWN &&
               board[kingPosX-1][kingPosY+1].color==BLACK) {
                return true;
            }
        }
        if(isOnBoard(kingPosX+1, kingPosY+1)) {
            if(board[kingPosX+1][kingPosY+1].pieceType==PAWN &&
               board[kingPosX+1][kingPosY+1].color==BLACK) {
                return true;
            }
        }
        return false;
    }
    if(isOnBoard(kingPosX-1, kingPosY-1)) {
        if(board[kingPosX-1][kingPosY-1].pieceType==PAWN &&
           board[kingPosX-1][kingPosY-1].color==WHITE) {
            return true;
        }
    }
    if(isOnBoard(kingPosX+1, kingPosY-1)) {
        if(board[kingPosX+1][kingPosY-1].pieceType==PAWN &&
           board[kingPosX+1][kingPosY-1].color==WHITE) {
            return true;
        }
    }
    return false;
}

bool isCheck_Knight(Piece board[8][8], Color color, int kingPosX, int kingPosY) {
    if(isOnBoard(kingPosX-1, kingPosY+2)) {
        if(board[kingPosX-1][kingPosY+2].pieceType==KNIGHT &&
           board[kingPosX-1][kingPosY+2].color!=color) {
            return true;
        }
    }
    if(isOnBoard(kingPosX+1, kingPosY+2)) {
        if(board[kingPosX+1][kingPosY+2].pieceType==KNIGHT &&
           board[kingPosX+1][kingPosY+2].color!=color) {
            return true;
        }
    }
    if(isOnBoard(kingPosX-2, kingPosY+1)) {
        if(board[kingPosX-2][kingPosY+1].pieceType==KNIGHT &&
           board[kingPosX-2][kingPosY+1].color!=color) {
            return true;
        }
    }
    if(isOnBoard(kingPosX+2, kingPosY+1)) {
        if(board[kingPosX+2][kingPosY+1].pieceType==KNIGHT &&
           board[kingPosX+2][kingPosY+1].color!=color) {
            return true;
        }
    }
    if(isOnBoard(kingPosX-2, kingPosY-1)) {
        if(board[kingPosX-2][kingPosY-1].pieceType==KNIGHT &&
           board[kingPosX-2][kingPosY-1].color!=color) {
            return true;
        }
    }
    if(isOnBoard(kingPosX+2, kingPosY-1)) {
        if(board[kingPosX+2][kingPosY-1].pieceType==KNIGHT &&
           board[kingPosX+2][kingPosY-1].color!=color) {
            return true;
        }
    }
    if(isOnBoard(kingPosX-1, kingPosY-2)) {
        if(board[kingPosX-1][kingPosY-2].pieceType==KNIGHT &&
           board[kingPosX-1][kingPosY-2].color!=color) {
            return true;
        }
    }
    if(isOnBoard(kingPosX+1, kingPosY-2)) {
        if(board[kingPosX+1][kingPosY-2].pieceType==KNIGHT &&
           board[kingPosX+1][kingPosY-2].color!=color) {
            return true;
        }
    }
    return false;
}

bool isCheck_King(Piece board[8][8], Color color, int kingPosX, int kingPosY) {
    for(int i=kingPosX-1; i<kingPosX+2; i++) {
        for(int j=kingPosY-1; j<kingPosY+2; j++) {
            if(isOnBoard(i,j)) {
                if(board[i][j].pieceType==KING &&
                   board[i][j].color!=color) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool isCheck(Piece board[8][8], Color color) {
    int kingPosX, kingPosY;
    findKing(board, color, &kingPosX, &kingPosY);
    if(isCheck_Horizontal(board, color, kingPosX, kingPosY)) return true;
    if(isCheck_Vertical(board, color, kingPosX, kingPosY)) return true;
    if(isCheck_Pawn(board, color, kingPosX, kingPosY)) return true;
    if(isCheck_Knight(board, color, kingPosX, kingPosY)) return true;
    if(isCheck_King(board, color, kingPosX, kingPosY)) return true;
    return false;
}