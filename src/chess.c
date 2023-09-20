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

void initTestBoard1(Piece board[8][8]) {
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            initPiece_Empty(&board[i][j]);
        }
    }
    
    // Initialize black pieces on rank 7 (line 8)
    initPiece(&board[1][7], KING, BLACK);
    initPiece(&board[3][7], PAWN, BLACK);
    initPiece(&board[6][7], BISHOP, BLACK);
    
    // Initialize black pieces on rank 6 (line 7)
    initPiece(&board[0][6], ROOK, BLACK);
    initPiece(&board[4][6], QUEEN, BLACK);
    
    // Initialize black pieces on rank 5 (line 6)
    initPiece(&board[2][5], KNIGHT, BLACK);
    
    // Initialize black pieces on rank 4 (line 5)
    initPiece(&board[7][4], ROOK, BLACK);
    initPiece(&board[5][4], PAWN, BLACK);
    
    // Initialize white pieces on rank 3 (line 4)
    initPiece(&board[3][3], BISHOP, WHITE);
    
    // Initialize white pieces on rank 2 (line 3)
    initPiece(&board[1][2], KING, WHITE);
    
    // Initialize white pieces on rank 1 (line 2)
    initPiece(&board[7][1], QUEEN, WHITE);
    initPiece(&board[4][1], KNIGHT, WHITE);
    
    // Initialize white pieces on rank 0 (line 1)
    initPiece(&board[2][0], PAWN, WHITE);
    initPiece(&board[6][0], ROOK, WHITE);
}

void initTestBoard2(Piece board[8][8]) {
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            initPiece_Empty(&board[i][j]);
        }
    }
    initPiece(&board[3][3], KING, WHITE);
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

void squareToInt(Square square, int* x, int* y) {
    //row A
    *x=0;
    switch(square) {
        case A1:
            *y=0; return;
        case A2:
            *y=1; return;
        case A3:
            *y=2; return;
        case A4:
            *y=3; return;
        case A5:
            *y=4; return;
        case A6:
            *y=5; return;
        case A7:
            *y=6; return;
        case A8:
            *y=7; return;
    }
    //row B
    *x=1;
    switch(square) {
        case B1:
            *y=0; return;
        case B2:
            *y=1; return;
        case B3:
            *y=2; return;
        case B4:
            *y=3; return;
        case B5:
            *y=4; return;
        case B6:
            *y=5; return;
        case B7:
            *y=6; return;
        case B8:
            *y=7; return;
    }
    //row C
    *x=2;
    switch(square) {
        case C1:
            *y=0; return;
        case C2:
            *y=1; return;
        case C3:
            *y=2; return;
        case C4:
            *y=3; return;
        case C5:
            *y=4; return;
        case C6:
            *y=5; return;
        case C7:
            *y=6; return;
        case C8:
            *y=7; return;
    }
    //row D
    *x=3;
    switch(square) {
        case D1:
            *y=0; return;
        case D2:
            *y=1; return;
        case D3:
            *y=2; return;
        case D4:
            *y=3; return;
        case D5:
            *y=4; return;
        case D6:
            *y=5; return;
        case D7:
            *y=6; return;
        case D8:
            *y=7; return;
    }
    //row E
    *x=4;
    switch(square) {
        case E1:
            *y=0; return;
        case E2:
            *y=1; return;
        case E3:
            *y=2; return;
        case E4:
            *y=3; return;
        case E5:
            *y=4; return;
        case E6:
            *y=5; return;
        case E7:
            *y=6; return;
        case E8:
            *y=7; return;
    }
    //row F
    *x=5;
    switch(square) {
        case F1:
            *y=0; return;
        case F2:
            *y=1; return;
        case F3:
            *y=2; return;
        case F4:
            *y=3; return;
        case F5:
            *y=4; return;
        case F6:
            *y=5; return;
        case F7:
            *y=6; return;
        case F8:
            *y=7; return;
    }
    //row G
    *x=6;
    switch(square) {
        case G1:
            *y=0; return;
        case G2:
            *y=1; return;
        case G3:
            *y=2; return;
        case G4:
            *y=3; return;
        case G5:
            *y=4; return;
        case G6:
            *y=5; return;
        case G7:
            *y=6; return;
        case G8:
            *y=7; return;
    }
    //row H
    *x=7;
    switch(square) {
        case H1:
            *y=0; return;
        case H2:
            *y=1; return;
        case H3:
            *y=2; return;
        case H4:
            *y=3; return;
        case H5:
            *y=4; return;
        case H6:
            *y=5; return;
        case H7:
            *y=6; return;
        case H8:
            *y=7; return;
    }
}


void intToSquare(unsigned char x, unsigned char y, Square* square) {
    switch(x) {
        case 0: {
            switch(y) {
                case 0:
                    *square = A1;
                    return;
                case 1:
                    *square = A2;
                    return;
                case 2:
                    *square = A3;
                    return;
                case 3:
                    *square = A4;
                    return;
                case 4:
                    *square = A5;
                    return;
                case 5:
                    *square = A6;
                    return;
                case 6:
                    *square = A7;
                    return;
                case 7:
                    *square = A8;
                    return;
            }
        }
        case 1: {
            switch(y) {
                case 0:
                    *square = B1;
                    return;
                case 1:
                    *square = B2;
                    return;
                case 2:
                    *square = B3;
                    return;
                case 3:
                    *square = B4;
                    return;
                case 4:
                    *square = B5;
                    return;
                case 5:
                    *square = B6;
                    return;
                case 6:
                    *square = B7;
                    return;
                case 7:
                    *square = B8;
                    return;
            }
        }
        case 2: {
            switch(y) {
                case 0:
                    *square = C1;
                    return;
                case 1:
                    *square = C2;
                    return;
                case 2:
                    *square = C3;
                    return;
                case 3:
                    *square = C4;
                    return;
                case 4:
                    *square = C5;
                    return;
                case 5:
                    *square = C6;
                    return;
                case 6:
                    *square = C7;
                    return;
                case 7:
                    *square = C8;
                    return;
            }
        }
        case 3: {
            switch(y) {
                case 0:
                    *square = D1;
                    return;
                case 1:
                    *square = D2;
                    return;
                case 2:
                    *square = D3;
                    return;
                case 3:
                    *square = D4;
                    return;
                case 4:
                    *square = D5;
                    return;
                case 5:
                    *square = D6;
                    return;
                case 6:
                    *square = D7;
                    return;
                case 7:
                    *square = D8;
                    return;
            }
        }
        case 4: {
            switch(y) {
                case 0:
                    *square = E1;
                    return;
                case 1:
                    *square = E2;
                    return;
                case 2:
                    *square = E3;
                    return;
                case 3:
                    *square = E4;
                    return;
                case 4:
                    *square = E5;
                    return;
                case 5:
                    *square = E6;
                    return;
                case 6:
                    *square = E7;
                    return;
                case 7:
                    *square = E8;
                    return;
            }
        }
        case 5: {
            switch(y) {
                case 0:
                    *square = F1;
                    return;
                case 1:
                    *square = F2;
                    return;
                case 2:
                    *square = F3;
                    return;
                case 3:
                    *square = F4;
                    return;
                case 4:
                    *square = F5;
                    return;
                case 5:
                    *square = F6;
                    return;
                case 6:
                    *square = F7;
                    return;
                case 7:
                    *square = F8;
                    return;
            }
        }
        case 6: {
            switch(y) {
                case 0:
                    *square = G1;
                    return;
                case 1:
                    *square = G2;
                    return;
                case 2:
                    *square = G3;
                    return;
                case 3:
                    *square = G4;
                    return;
                case 4:
                    *square = G5;
                    return;
                case 5:
                    *square = G6;
                    return;
                case 6:
                    *square = G7;
                    return;
                case 7:
                    *square = G8;
                    return;
            }
        }
        case 7: {
            switch(y) {
                case 0:
                    *square = H1;
                    return;
                case 1:
                    *square = H2;
                    return;
                case 2:
                    *square = H3;
                    return;
                case 3:
                    *square = H4;
                    return;
                case 4:
                    *square = H5;
                    return;
                case 5:
                    *square = H6;
                    return;
                case 6:
                    *square = H7;
                    return;
                case 7:
                    *square = H8;
                    return;
            }
        }
    }
}

void movePiece_Pawn(Piece board[8][8], unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
    Color color = board[x1][y1].color;
    if(y2-y1==2) { //white pawn double move
        if(isOnBoard(x2-1,y2)) { //check left, set left enPassante right
            if(board[x2-1][y2].pieceType==PAWN &&
               board[x2-1][y2].color!=color) {
                board[x2-1][y2].canEnpassante_right=true;
            }
        }
        if(isOnBoard(x2+1,y2)) {//check right, set right enPassante left
            if(board[x2+1][y2].pieceType==PAWN &&
               board[x2+1][y2].color!=color) {
                board[x2+1][y2].canEnpassante_left=true;
            }
        }
    } else if(y1-y2==2) { //black pawn double move
        if(isOnBoard(x2-1,y2)) { //check left, set left enPassante right
            if(board[x2-1][y2].pieceType==PAWN &&
               board[x2-1][y2].color!=color) {
                board[x2-1][y2].canEnpassante_right=true;
            }
        }
        if(isOnBoard(x2+1,y2)) { //check right, set right enPassante left
            if(board[x2+1][y2].pieceType==PAWN &&
               board[x2+1][y2].color!=color) {
                board[x2+1][y2].canEnpassante_left=true;
            }
        }
    } else if(x1!=x2) {
        if(board[x2][y2].pieceType==EMPTY) {
            if(color==WHITE) {
                initPiece_Empty(&board[x2][y1-1]);
            } else {
                initPiece_Empty(&board[x2][y1+1]);
            }
        }
    } else if(y2==7||y2==0) { //pawn promote
        initPiece(&board[x2][y2], QUEEN, color);
        initPiece_Empty(&board[x1][y1]);
        return;
    }
    board[x2][y2] = board[x1][y1];
    initPiece_Empty(&board[x1][y1]);
    return;
}

void movePiece_Knight(Piece board[8][8], unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
    board[x2][y2] = board[x1][y1];
    initPiece_Empty(&board[x1][y1]);
    return;
}

void movePiece_Bishop(Piece board[8][8], unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
    board[x2][y2] = board[x1][y1];
    initPiece_Empty(&board[x1][y1]);
    return;
}

void movePiece_Rook(Piece board[8][8], unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
    board[x2][y2] = board[x1][y1];
    initPiece_Empty(&board[x1][y1]);

    board[x2][y2].canSpecialMove=false;
    return;
}

void movePiece_Queen(Piece board[8][8], unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
    board[x2][y2] = board[x1][y1];
    initPiece_Empty(&board[x1][y1]);
    return;
}

void movePiece_King(Piece board[8][8], unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
    if(x2-x1==2) {
        board[5][y2]=board[7][y2];
        initPiece_Empty(&board[7][y2]);
        board[5][y2].canSpecialMove=false;
    } else if(x1-x2==2) {
        board[3][y2]=board[0][y2];
        initPiece_Empty(&board[0][y2]);
        board[3][y2].canSpecialMove=false;
    }
    board[x2][y2] = board[x1][y1];
    initPiece_Empty(&board[x1][y1]);

    board[x2][y2].canSpecialMove=false;
    return;
}

void movePiece(Piece board[8][8], Move move) {
    int x1, y1, x2, y2;
    squareToInt(move.from, &x1, &y1);
    squareToInt(move.to, &x2, &y2);

    switch(board[x1][y1].pieceType) {
        case PAWN:
            movePiece_Pawn(board, x1, y1, x2 ,y2);
            break;
        case KNIGHT:
            movePiece_Knight(board, x1, y1, x2 ,y2);
            break;
        case BISHOP:
            movePiece_Bishop(board, x1, y1, x2 ,y2);
            break;
        case ROOK:
            movePiece_Rook(board, x1, y1, x2 ,y2);
            break;
        case QUEEN:
            movePiece_Queen(board, x1, y1, x2 ,y2);
            break;
        case KING:
            movePiece_King(board, x1, y1, x2 ,y2);
            break;
    }
    return;
}


void addMoveToList(unsigned char x1, unsigned char y1,
                   unsigned char x2, unsigned char y2,
                   unsigned char** x1List, unsigned char** y1List,
                   unsigned char** x2List, unsigned char** y2List,
                   int *index, int* size) {
    if(*index>=*size) {
        *size+=10;
        *x1List = (unsigned char*) realloc(*x1List, (*size));
        if(!*x1List) printf("x1List realloc failed");
        *y1List = (unsigned char*) realloc(*y1List, (*size));
        if(!*y1List) printf("y1List realloc failed");
        *x2List = (unsigned char*) realloc(*x2List, (*size));
        if(!*x2List) printf("x2List realloc failed");
        *y2List = (unsigned char*) realloc(*y2List, (*size));
        if(!*y2List) printf("y2List realloc failed");
    }

    (*x1List)[*index]=x1;
    (*y1List)[*index]=y1;
    (*x2List)[*index]=x2;
    (*y2List)[*index]=y2;

    (*index)++;
    return;
}

//checks if a move is valid, returns true if it is, false if it isn't
bool checkMove(Piece board[8][8],
               Color color,
               int x1, int y1,
               int x2, int y2) {
    Piece tmp1 = board[x1][y1];
    Piece tmp2 = board[x2][y2];
    board[x2][y2]=board[x1][y1];
    initPiece_Empty(&board[x1][y1]);
    if(isCheck(board, color)) {
       board[x1][y1]=tmp1;
       board[x2][y2]=tmp2;
       return false; 
    }
    board[x1][y1]=tmp1;
    board[x2][y2]=tmp2;
    return true;
}

bool checkMove_Pawn(Piece board[8][8],
                    Color color,
                    int x1, int y1,
                    int x2, int y2) {
    Piece tmp1 = board[x1][y1];
    Piece tmp2 = board[x2][y2];
    if(x1==x2) {
        board[x2][y2]=board[x1][y1];
        if(isCheck(board, color)) {
            board[x1][y1]=tmp1;
            board[x2][y2]=tmp2;
            return false;
        }
        board[x1][y1]=tmp1;
        board[x2][y2]=tmp2;
        return true;
    } else if(board[x2][y2].color==NONE) {
        if(color==WHITE) {
            Piece tmp3 = board[x2][y2-1];
            board[x2][y2]=board[x1][y1];
            initPiece_Empty(&board[x1][y1]);
            initPiece_Empty(&board[x2][y2-1]);
            if(isCheck(board, color)) {
                board[x1][y1]=tmp1;
                board[x2][y2]=tmp2;
                board[x2][y2-1]=tmp3;
                return false;
            }
            board[x1][y1]=tmp1;
            board[x2][y2]=tmp2;
            board[x2][y2-1]=tmp3;
            return true;
        } //else BLACK
        Piece tmp3 = board[x2][y2+1];
        board[x2][y2]=board[x1][y1];
        initPiece_Empty(&board[x1][y1]);
        initPiece_Empty(&board[x2][y2+1]);
        if(isCheck(board, color)) {
            board[x1][y1]=tmp1;
            board[x2][y2]=tmp2;
            board[x2][y2+1]=tmp3;
            return false;
        }
        board[x1][y1]=tmp1;
        board[x2][y2]=tmp2;
        board[x2][y2+1]=tmp3;
        return true;
    }
    board[x2][y2]=board[x1][y1];
    initPiece_Empty(&board[x1][y1]);
    if(isCheck(board, color)) {
        board[x1][y1]=tmp1;
        board[x2][y2]=tmp2;
        return false;
    }
    board[x1][y1]=tmp1;
    board[x2][y2]=tmp2;
    return true;
}

bool checkMove_Castle(Piece board[8][8],
                      Color color,
                      int x1, int y1,
                      int x2, int y2) {
    if(isCheck(board, color)) {
        return false;
    }
    Piece tmp1 = board[x1][y1];
    if(x1-x2==2) {//castle queen side

        board[3][y1]=board[x1][y1];
        initPiece_Empty(&board[x1][y1]);
        if(isCheck(board, color)) {
            board[x1][y1]=tmp1;
            initPiece_Empty(&board[3][y1]);
            return false;
        }

        Piece tmp2 = board[0][y1];
        board[2][y1]=board[3][y1];
        board[3][y1]=board[0][y1];
        initPiece_Empty(&board[0][y1]);
        if(isCheck(board, color)) {
            board[x1][y1]=tmp1;
            board[0][y1]=tmp2;
            initPiece_Empty(&board[2][y1]);
            initPiece_Empty(&board[3][y1]);
            return false;
        }
        board[x1][y1]=tmp1;
        board[0][y1]=tmp2;
        initPiece_Empty(&board[2][y1]);
        initPiece_Empty(&board[3][y1]);
        return true;
    } else {
        // else kingside
        board[5][y1] = board[x1][y1];
        initPiece_Empty(&board[x1][y1]);
        if (isCheck(board, color)) {
            board[x1][y1] = tmp1;
            initPiece_Empty(&board[5][y1]);
            return false;
        }
        Piece tmp2 = board[7][y1];
        board[6][y1] = board[5][y1];
        board[5][y1] = board[7][y1];
        initPiece_Empty(&board[7][y1]);
        if (isCheck(board, color)) {
            board[x1][y1] = tmp1;
            board[7][y1] = tmp2;
            initPiece_Empty(&board[5][y1]);
            initPiece_Empty(&board[6][y1]);
            return false;
        }
        board[x1][y1] = tmp1;
        board[7][y1] = tmp2;
        initPiece_Empty(&board[5][y1]);
        initPiece_Empty(&board[6][y1]);
        return true;
    }
}

void getMoves_Pawn_White(Piece board[8][8],
                  int x, int y,
                  unsigned char** x1List,
                  unsigned char** y1List,
                  unsigned char** x2List,
                  unsigned char** y2List,
                  int* index,
                  int* size) {
    if(isOnBoard(x, y+1)) {
        if(board[x][y+1].pieceType==EMPTY) {
            if(checkMove_Pawn(board, WHITE, x, y, x, y+1)) {
                addMoveToList(x, y, x, y+1, x1List, y1List, x2List, y2List, index, size);
            }
        }
    }
    if(y==1) {
        if(board[x][y+1].pieceType==EMPTY &&
           board[x][y+2].pieceType==EMPTY) {
            if(checkMove_Pawn(board, WHITE, x, y, x, y+2)) {
                addMoveToList(x, y, x, y+2, x1List, y1List, x2List, y2List, index, size);
            }
       }
    }
    if(isOnBoard(x-1, y+1)) {
        if(board[x][y].canEnpassante_left) {
            if(checkMove_Pawn(board, WHITE, x, y, x-1, y+1)) {
                addMoveToList(x, y, x-1, y+1, x1List, y1List, x2List, y2List, index, size);
            }
            board[x][y].canEnpassante_left=false;
        } else if(board[x-1][y+1].color==BLACK) {
            if(checkMove_Pawn(board, WHITE, x, y, x-1, y+1)) {
                addMoveToList(x, y, x-1, y+1, x1List, y1List, x2List, y2List, index, size);
            }
        }
    }
    if(isOnBoard(x+1, y+1)) {
        if(board[x][y].canEnpassante_right) {
            if(checkMove_Pawn(board, WHITE, x, y, x+1, y+1)) {
                addMoveToList(x, y, x+1, y+1, x1List, y1List, x2List, y2List, index, size);
            }
            board[x][y].canEnpassante_right=false;
        } else if(board[x+1][y+1].color==BLACK) {
            if(checkMove_Pawn(board, WHITE, x, y, x+1, y+1)) { 
                addMoveToList(x, y, x+1, y+1, x1List, y1List, x2List, y2List, index, size);
            }
        }
    }
    return;
}

void getMoves_Pawn_Black(Piece board[8][8],
                  int x, int y,
                  unsigned char** x1List,
                  unsigned char** y1List,
                  unsigned char** x2List,
                  unsigned char** y2List,
                  int* index,
                  int* size) {
    if(isOnBoard(x, y-1)) {
        if(board[x][y-1].pieceType==EMPTY) {
            if(checkMove_Pawn(board, BLACK, x, y, x, y-1)) {
                addMoveToList(x, y, x, y-1, x1List, y1List, x2List, y2List, index, size);
            }
        }
    }
    if(y==6) {
        if(board[x][y-1].pieceType==EMPTY &&
           board[x][y-2].pieceType==EMPTY) {
            if(checkMove_Pawn(board, BLACK, x, y, x, y-2)) {
                addMoveToList(x, y, x, y-2, x1List, y1List, x2List, y2List, index, size);
            }
       }
    }
    if(isOnBoard(x-1, y-1)) {
        if(board[x][y].canEnpassante_left) {
            if(checkMove_Pawn(board, BLACK, x, y, x-1, y-1)) {
                addMoveToList(x, y, x-1, y-1, x1List, y1List, x2List, y2List, index, size);
            }
            board[x][y].canEnpassante_left=false;
        } else if(board[x-1][y-1].color==WHITE) {
            if(checkMove_Pawn(board, BLACK, x, y, x-1, y-1)) {
                addMoveToList(x, y, x-1, y-1, x1List, y1List, x2List, y2List, index, size);
            }
        }
    }
    if(isOnBoard(x+1, y-1)) {
        if(board[x][y].canEnpassante_right) {
            if(checkMove_Pawn(board, BLACK, x, y, x+1, y-1)) {
                addMoveToList(x, y, x+1, y-1, x1List, y1List, x2List, y2List, index, size);
            }
            board[x][y].canEnpassante_right=false;
        } else if(board[x+1][y-1].color==WHITE) {
            if(checkMove_Pawn(board, BLACK, x, y, x+1, y-1)) {
                addMoveToList(x, y, x+1, y-1, x1List, y1List, x2List, y2List, index, size);
            }
        }
    }
    return;
}

void getMoves_Knight(Piece board[8][8],
                  Color color,
                  int x, int y,
                  unsigned char** x1List,
                  unsigned char** y1List,
                  unsigned char** x2List,
                  unsigned char** y2List,
                  int* index,
                  int* size) {

    int xOffsetList[] = {-1, 1, -2, 2, -2, 2, -1, 1};
    int yOffsetList[] = {2, 2, 1, 1, -1, -1, -2, -2};
    int len = 8;
    int x2, y2;

    for(int i=0; i<len; i++) {
        x2 = x+xOffsetList[i];
        y2 = y+yOffsetList[i];
        if(isOnBoard(x2, y2)) {
            if(board[x2][y2].color==EMPTY||board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
            }
        }
    }
}

void getMoves_Diagonal(Piece board[8][8],
                       Color color,
                       int x, int y,
                       unsigned char** x1List,
                       unsigned char** y1List,
                       unsigned char** x2List,
                       unsigned char** y2List,
                       int* index,
                       int* size) {
    int x2, y2;
    
    for(int i=1; i<8; i++) {
        x2=x-i; y2=y-i;
        if(isOnBoard(x2, y2)) {
            if(board[x2][y2].pieceType==EMPTY) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    for(int i=1; i<8; i++) {
        x2=x+i; y2=y-i;
        if(isOnBoard(x2, y2)) {
            if(board[x2][y2].pieceType==EMPTY) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    for(int i=1; i<8; i++) {
        x2=x-i; y2=y+i;
        if(isOnBoard(x2, y2)) {
            if(board[x2][y2].pieceType==EMPTY) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    for(int i=1; i<8; i++) {
        x2=x+i; y2=y+i;
        if(isOnBoard(x2, y2)) {
            if(board[x2][y2].pieceType==EMPTY) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    return;
}

void getMoves_Orthogonal(Piece board[8][8],
                         Color color,
                         int x, int y,
                         unsigned char** x1List,
                         unsigned char** y1List,
                         unsigned char** x2List,
                         unsigned char** y2List,
                         int* index,
                         int* size) {
    int x2, y2;

    y2=y;
    for(int i=1; i<8; i++) {
        x2=x-i;
        if(isOnBoard(x2, y2)) {
            if(board[x2][y2].pieceType==EMPTY) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    for(int i=1; i<8; i++) {
        x2=x+i;
        if(isOnBoard(x2, y2)) {
            if(board[x2][y2].pieceType==EMPTY) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    x2=x;
    for(int i=1; i<8; i++) {
        y2=y-i;
        if(isOnBoard(x2, y2)) {
            if(board[x2][y2].pieceType==EMPTY) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    for(int i=1; i<8; i++) {
        y2=y+i;
        if(isOnBoard(x2, y2)) {
            if(board[x2][y2].pieceType==EMPTY) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                }
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    return;
}

void getMoves_Bishop(Piece board[8][8],
                     Color color,
                     int x, int y,
                     unsigned char** x1List,
                     unsigned char** y1List,
                     unsigned char** x2List,
                     unsigned char** y2List,
                     int* index,
                     int* size) {
    getMoves_Diagonal(board, color, x, y, x1List, y1List, x2List, y2List, index, size);
    return;
}

void getMoves_Rook(Piece board[8][8],
                   Color color,
                   int x, int y,
                   unsigned char** x1List,
                   unsigned char** y1List,
                   unsigned char** x2List,
                   unsigned char** y2List,
                   int* index,
                   int* size) {
    getMoves_Orthogonal(board, color, x, y, x1List, y1List, x2List, y2List, index, size);
    return;
}

void getMoves_Queen(Piece board[8][8],
                    Color color,
                    int x, int y,
                    unsigned char** x1List,
                    unsigned char** y1List,
                    unsigned char** x2List,
                    unsigned char** y2List,
                    int* index,
                    int* size) {
    getMoves_Diagonal(board, color, x, y, x1List, y1List, x2List, y2List, index, size);
    getMoves_Orthogonal(board, color, x, y, x1List, y1List, x2List, y2List, index, size);
    return;
}

void getMoves_King(Piece board[8][8],
                   Color color,
                   int x, int y,
                   unsigned char** x1List,
                   unsigned char** y1List,
                   unsigned char** x2List,
                   unsigned char** y2List,
                   int* index,
                   int* size) {
    int x2, y2;
    for(int i=-1; i<2; i++) {
        for(int j=-1; j<2; j++) {
            x2=x+i; y2=y+j;
            if(isOnBoard(x2, y2)) {
                if(board[x2][y2].color!=color) {
                    if(checkMove(board, color, x, y, x2, y2)) {
                        addMoveToList(x, y, x2, y2, x1List, y1List, x2List, y2List, index, size);
                    }
                }
            }
        }
    }
    if(board[x][y].canSpecialMove) {
        if(board[0][y].pieceType==ROOK&&board[0][y].canSpecialMove) {
            if(board[1][y].color==NONE &&
               board[2][y].color==NONE &&
               board[3][y].color==NONE) {
                if(checkMove_Castle(board, color, x, y, 2, y)) {
                    addMoveToList(x, y, 2, y, x1List, y1List, x2List, y2List, index, size);
                }
            }
        }
        if(board[7][y].pieceType==ROOK&&board[7][y].canSpecialMove) {
            if(board[5][y].color==NONE &&
               board[6][y].color==NONE) {
                if(checkMove_Castle(board, color, x, y, 6, y)) {
                    addMoveToList(x, y, 6, y, x1List, y1List, x2List, y2List, index, size);
                }
            }
        }
    }
}

void intToSquareMoveList(unsigned char* x1List,
                         unsigned char* y1List,
                         unsigned char* x2List,
                         unsigned char* y2List,
                         int index,
                         Move** moveList) {
    (*moveList) = (Move*) malloc(index*sizeof(Move));
    Square square;
    for(int i=0; i<index; i++) {
        intToSquare(x1List[i], y1List[i], &square);
        (*moveList)[i].from = square;
        intToSquare(x2List[i], y2List[i], &square);
        (*moveList)[i].to = square;
    }
    return;
}

void getMoves_FromSquare(Piece board[8][8],
                        Color color,
                        int x, int y,
                        unsigned char** x1List,
                        unsigned char** y1List,
                        unsigned char** x2List,
                        unsigned char** y2List,
                        int* index,
                        int* size) {
    
    switch(board[x][y].pieceType) {
        case PAWN: {
            if(color==WHITE) {
                getMoves_Pawn_White(board, x, y, x1List, y1List, x2List, y2List, index, size);
                break;
            }
            getMoves_Pawn_Black(board, x, y, x1List, y1List, x2List, y2List, index, size);
            break;
        }
        case KNIGHT:
            getMoves_Knight(board, color, x, y, x1List, y1List, x2List, y2List, index, size);
            break;
        case BISHOP:
            getMoves_Bishop(board, color, x, y, x1List, y1List, x2List, y2List, index, size);
            break;
        case ROOK:
            getMoves_Rook(board, color, x, y, x1List, y1List, x2List, y2List, index, size);
            break;
        case QUEEN:
            getMoves_Queen(board, color, x, y, x1List, y1List, x2List, y2List, index, size);
            break;
        case KING:
            getMoves_King(board, color, x, y, x1List, y1List, x2List, y2List, index, size);
            break;
    }
    return;
}


int getMoves(Piece board[8][8], Color color, Move** moveList) {
    int index = 0, size = 10;
    unsigned char *x1List, *y1List, *x2List, *y2List;
    x1List = (unsigned char*)malloc(size);
    y1List = (unsigned char*)malloc(size);
    x2List = (unsigned char*)malloc(size);
    y2List = (unsigned char*)malloc(size);
    for(int j=0; j<8; j++) {
        for(int i=0; i<8; i++) {
            if(board[i][j].color==color) {
                getMoves_FromSquare(board, color, i, j, &x1List, &y1List, &x2List, &y2List, &index, &size);
            }
        }
    }
    intToSquareMoveList(x1List, y1List, x2List, y2List, index, moveList);
    free(x2List);
    free(x1List);
    free(y1List);
    free(y2List);
    return index;
}