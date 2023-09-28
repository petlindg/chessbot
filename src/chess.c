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

bool isCheck_Horizontal(Piece board[8][8], Color color, Square kingPos) {
    //check left of king
    for(int i=kingPos.x-1; i>=0; i--) {
        if((board[i][kingPos.y].pieceType==ROOK ||
           board[i][kingPos.y].pieceType==QUEEN) &&
          (board[i][kingPos.y].color != color))
            return true;
        if(board[i][kingPos.y].pieceType!=EMPTY) break;
    }
    //check right of king
    for(int i=kingPos.x+1; i<8; i++) {
        if((board[i][kingPos.y].pieceType==ROOK ||
           board[i][kingPos.y].pieceType==QUEEN) &&
          (board[i][kingPos.y].color != color))
            return true;
        if(board[i][kingPos.y].pieceType!=EMPTY) break;
    }
    //check down of king
    for(int j=kingPos.y-1; j>=0; j--) {
        if((board[kingPos.x][j].pieceType==ROOK ||
           board[kingPos.x][j].pieceType==QUEEN) &&
          (board[kingPos.x][j].color != color))
            return true;
        if(board[kingPos.x][j].pieceType!=EMPTY) break;
    }
    //check up of king
    for(int j=kingPos.y+1; j<8; j++) {
        if((board[kingPos.x][j].pieceType==ROOK ||
           board[kingPos.x][j].pieceType==QUEEN) &&
          (board[kingPos.x][j].color != color))
            return true;
        if(board[kingPos.x][j].pieceType!=EMPTY) break;
    }
    return false;
}

bool isCheck_Vertical(Piece board[8][8], Color color, Square kingPos) {
    //check down left of king
    for(int i=kingPos.x-1, j=kingPos.y-1; i>=0&&j>=0; i--, j--) {
        if((board[i][j].pieceType==BISHOP ||
           board[i][j].pieceType==QUEEN) &&
          (board[i][j].color != color)) {
            return true;
            }
        if(board[i][j].pieceType!=EMPTY) break;
    }
    //check down right of king
    for(int i=kingPos.x+1, j=kingPos.y-1; i<8&&j>=0; i++, j--) {
        if((board[i][j].pieceType==BISHOP ||
           board[i][j].pieceType==QUEEN) &&
          (board[i][j].color != color)) {
            return true;
            }
        if(board[i][j].pieceType!=EMPTY) break;
    }
    //check up left of king
    for(int i=kingPos.x-1, j=kingPos.y+1; i>=0&&j<8; i--, j++) {
        if((board[i][j].pieceType==BISHOP ||
           board[i][j].pieceType==QUEEN) &&
          (board[i][j].color != color)) {
            return true;
            }
        if(board[i][j].pieceType!=EMPTY) break;
    }
    //check up right of king
    for(int i=kingPos.x+1, j=kingPos.y+1; i<8&&j<8; i++, j++) {
        if((board[i][j].pieceType==BISHOP ||
           board[i][j].pieceType==QUEEN) &&
          (board[i][j].color != color)) {
            return true;
            }
        if(board[i][j].pieceType!=EMPTY) break;
    }
    return false;
}

bool isCheck_Pawn(Piece board[8][8], Color color, Square kingPos) {
    Square threat;
    if(color==WHITE) {
        threat.x=kingPos.x-1;
        threat.y=kingPos.y+1;
        if(isOnBoard(threat)) {
            if(board[threat.x][threat.y].pieceType==PAWN &&
               board[threat.x][threat.y].color==BLACK) {
                return true;
            }
        }
        threat.x=kingPos.x+1;
        if(isOnBoard(threat)) {
            if(board[threat.x][threat.y].pieceType==PAWN &&
               board[threat.x][threat.y].color==BLACK) {
                return true;
            }
        }
        return false;
    }
    //else color==BLACK;
    threat.x=kingPos.x-1;
    threat.y=kingPos.y-1;
    if(isOnBoard(threat)) {
        if(board[threat.x][threat.y].pieceType==PAWN &&
           board[threat.x][threat.y].color==WHITE) {
            return true;
        }
    }
    threat.x=kingPos.x+1;
    if(isOnBoard(threat)) {
        if(board[threat.x][threat.y].pieceType==PAWN &&
           board[threat.x][threat.y].color==WHITE) {
            return true;
        }
    }
    return false;
}

bool isCheck_Knight(Piece board[8][8], Color color, Square kingPos) {

    int xOffsetList[] = {-1, 1, -2, 2, -2, 2, -1, 1};
    int yOffsetList[] = {2, 2, 1, 1, -1, -1, -2, -2};
    int len = 8;
    Square threat;

    for(int i = 0; i<len; i++) {
        threat.x=kingPos.x+xOffsetList[i];
        threat.y=kingPos.y+yOffsetList[i];
        if(isOnBoard(threat)) {
            if(board[threat.x][threat.y].pieceType==KNIGHT &&
               board[threat.x][threat.y].color!=color) {
                return true;
            }
        }
    }

    return false;
}

bool isCheck_King(Piece board[8][8], Color color, Square kingPos) {
    Square threat;
    for(int i=kingPos.x-1; i<kingPos.x+2; i++) {
        for(int j=kingPos.y-1; j<kingPos.y+2; j++) {
            threat.x=i;
            threat.y=j;
            if(isOnBoard(threat)) {
                if(board[threat.x][threat.y].pieceType==KING &&
                   board[threat.x][threat.y].color!=color) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool isCheck(Piece board[8][8], Color color) {
    Square kingPos;
    findKing(board, color, &kingPos);
    if(isCheck_Horizontal(board, color, kingPos)) return true;
    if(isCheck_Vertical(board, color, kingPos)) return true;
    if(isCheck_Pawn(board, color, kingPos)) return true;
    if(isCheck_Knight(board, color, kingPos)) return true;
    if(isCheck_King(board, color, kingPos)) return true;
    return false;
}