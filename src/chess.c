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
          (board[i][j].color != color)) {
            return true;
            }
        if(board[i][j].pieceType!=EMPTY) break;
    }
    //check down right of king
    for(int i=kingPosX+1, j=kingPosY-1; i<8&&j>=0; i++, j--) {
        if((board[i][j].pieceType==BISHOP ||
           board[i][j].pieceType==QUEEN) &&
          (board[i][j].color != color)) {
            return true;
            }
        if(board[i][j].pieceType!=EMPTY) break;
    }
    //check up left of king
    for(int i=kingPosX-1, j=kingPosY+1; i>=0&&j<8; i--, j++) {
        if((board[i][j].pieceType==BISHOP ||
           board[i][j].pieceType==QUEEN) &&
          (board[i][j].color != color)) {
            return true;
            }
        if(board[i][j].pieceType!=EMPTY) break;
    }
    //check up right of king
    for(int i=kingPosX+1, j=kingPosY+1; i<8&&j<8; i++, j++) {
        if((board[i][j].pieceType==BISHOP ||
           board[i][j].pieceType==QUEEN) &&
          (board[i][j].color != color)) {
            return true;
            }
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
    } //else color==BLACK;
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
                initPiece_Empty(&board[x2][y2-1]);
            } else {
                initPiece_Empty(&board[x2][y2+1]);
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
    //printf("x:%d, y:%d\n", move.from.x, move.from.y);
    switch(board[move.from.x][move.from.y].pieceType) {
        case PAWN:
            movePiece_Pawn(board, move.from.x, move.from.y, move.to.x ,move.to.y);
            break;
        case KNIGHT:
            movePiece_Knight(board, move.from.x, move.from.y, move.to.x ,move.to.y);
            break;
        case BISHOP:
            movePiece_Bishop(board, move.from.x, move.from.y, move.to.x ,move.to.y);
            break;
        case ROOK:
            movePiece_Rook(board, move.from.x, move.from.y, move.to.x ,move.to.y);
            break;
        case QUEEN:
            movePiece_Queen(board, move.from.x, move.from.y, move.to.x ,move.to.y);
            break;
        case KING:
            movePiece_King(board, move.from.x, move.from.y, move.to.x ,move.to.y);
            break;
    }
    return;
}


void addMoveToList(unsigned char x1, unsigned char y1,
                   unsigned char x2, unsigned char y2,
                   Move** moves,
                   int *index, int* size) {
    if(*index>=*size) {
        *size+=10;
        (*moves) = (Move*) realloc(*moves, (sizeof(Move)*(*size)));
        if(!*moves) printf("moves realloc failed");
    }
    if(!isOnBoard(x2,y2)) {
        printf("x1:%d, y1:%d, x2:%d, y2:%d\nindex:%d, size:%d\n",x1,y1,x2,y2,*index,*size);
    }
    (*moves)[*index].from.x=x1;
    (*moves)[*index].from.y=y1;
    (*moves)[*index].to.x=x2;
    (*moves)[*index].to.y=y2;
    (*index)++;
    return;
}

void addMoveToList_Promotion(unsigned char x1, unsigned char y1,
                   unsigned char x2, unsigned char y2,
                   PieceType pieceType,
                   Move** moves,
                   int *index, int* size) {
    if(*index>=*size) {
        *size+=10;
        *moves = (Move*) realloc(*moves, (sizeof(Move)*(*size)));
        if(!*moves) printf("moves realloc failed");
    }
    (*moves)[*index].from.x=x1;
    (*moves)[*index].from.y=y1;
    (*moves)[*index].to.x=x2;
    (*moves)[*index].to.y=y2;
    (*moves)[*index].promoteTo=pieceType;

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
        initPiece_Empty(&board[x1][y1]);
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
                  Move** moves,
                  int* index,
                  int* size) {
    if(isOnBoard(x, y+1)) {
        if(board[x][y+1].pieceType==EMPTY) {
            if(checkMove_Pawn(board, WHITE, x, y, x, y+1)) {
                addMoveToList(x, y, x, y+1, moves, index, size);
            }
        }
    }
    if(y==1) {
        if(board[x][y+1].pieceType==EMPTY &&
           board[x][y+2].pieceType==EMPTY) {
            if(checkMove_Pawn(board, WHITE, x, y, x, y+2)) {
                addMoveToList(x, y, x, y+2, moves, index, size);
            }
       }
    }
    if(isOnBoard(x-1, y+1)) {
        if(board[x][y].canEnpassante_left) {
            if(checkMove_Pawn(board, WHITE, x, y, x-1, y+1)) {
                addMoveToList(x, y, x-1, y+1, moves, index, size);
            }
            board[x][y].canEnpassante_left=false;
        } else if(board[x-1][y+1].color==BLACK) {
            if(checkMove_Pawn(board, WHITE, x, y, x-1, y+1)) {
                addMoveToList(x, y, x-1, y+1, moves, index, size);
            }
        }
    }
    if(isOnBoard(x+1, y+1)) {
        if(board[x][y].canEnpassante_right) {
            if(checkMove_Pawn(board, WHITE, x, y, x+1, y+1)) {
                addMoveToList(x, y, x+1, y+1, moves, index, size);
            }
            board[x][y].canEnpassante_right=false;
        } else if(board[x+1][y+1].color==BLACK) {
            if(checkMove_Pawn(board, WHITE, x, y, x+1, y+1)) { 
                addMoveToList(x, y, x+1, y+1, moves, index, size);
            }
        }
    }
    return;
}

void getMoves_Pawn_Black(Piece board[8][8],
                  int x, int y,
                  Move** moves,
                  int* index,
                  int* size) {
    if(isOnBoard(x, y-1)) {
        if(board[x][y-1].pieceType==EMPTY) {
            if(checkMove_Pawn(board, BLACK, x, y, x, y-1)) {
                addMoveToList(x, y, x, y-1, moves, index, size);
            }
        }
    }
    if(y==6) {
        if(board[x][y-1].pieceType==EMPTY &&
           board[x][y-2].pieceType==EMPTY) {
            if(checkMove_Pawn(board, BLACK, x, y, x, y-2)) {
                addMoveToList(x, y, x, y-2, moves, index, size);
            }
       }
    }
    if(isOnBoard(x-1, y-1)) {
        if(board[x][y].canEnpassante_left) {
            if(checkMove_Pawn(board, BLACK, x, y, x-1, y-1)) {
                addMoveToList(x, y, x-1, y-1, moves, index, size);
            }
            board[x][y].canEnpassante_left=false;
        } else if(board[x-1][y-1].color==WHITE) {
            if(checkMove_Pawn(board, BLACK, x, y, x-1, y-1)) {
                addMoveToList(x, y, x-1, y-1, moves, index, size);
            }
        }
    }
    if(isOnBoard(x+1, y-1)) {
        if(board[x][y].canEnpassante_right) {
            if(checkMove_Pawn(board, BLACK, x, y, x+1, y-1)) {
                addMoveToList(x, y, x+1, y-1, moves, index, size);
            }
            board[x][y].canEnpassante_right=false;
        } else if(board[x+1][y-1].color==WHITE) {
            if(checkMove_Pawn(board, BLACK, x, y, x+1, y-1)) {
                addMoveToList(x, y, x+1, y-1, moves, index, size);
            }
        }
    }
    return;
}

void getMoves_Pawn(Piece board[8][8],
                   Color color,
                   int x, int y,
                   Move** moves,
                   int* index,
                   int* size) {
    if(color==WHITE) {
        getMoves_Pawn_White(board, x, y, moves, index, size);
    } else {
        getMoves_Pawn_Black(board, x, y, moves, index, size);
    }
}

void getMoves_Knight(Piece board[8][8],
                  Color color,
                  int x, int y,
                  Move** moves,
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
                    addMoveToList(x, y, x2, y2, moves, index, size);
                }
            }
        }
    }
}

void getMoves_Diagonal(Piece board[8][8],
                       Color color,
                       int x, int y,
                       Move** moves,
                       int* index,
                       int* size) {
    int x2, y2;
    
    for(int i=1; i<8; i++) {
        x2=x-i; y2=y-i;
        if(isOnBoard(x2, y2)) {
            if(board[x2][y2].pieceType==EMPTY) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, moves, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, moves, index, size);
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
                    addMoveToList(x, y, x2, y2, moves, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, moves, index, size);
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
                    addMoveToList(x, y, x2, y2, moves, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, moves, index, size);
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
                    addMoveToList(x, y, x2, y2, moves, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, moves, index, size);
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
                         Move** moves,
                         int* index,
                         int* size) {
    int x2, y2;

    y2=y;
    for(int i=1; i<8; i++) {
        x2=x-i;
        if(isOnBoard(x2, y2)) {
            if(board[x2][y2].pieceType==EMPTY) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, moves, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, moves, index, size);
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
                    addMoveToList(x, y, x2, y2, moves, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, moves, index, size);
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
                    addMoveToList(x, y, x2, y2, moves, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, moves, index, size);
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
                    addMoveToList(x, y, x2, y2, moves, index, size);
                }
            } else if(board[x2][y2].color!=color) {
                if(checkMove(board, color, x, y, x2, y2)) {
                    addMoveToList(x, y, x2, y2, moves, index, size);
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
                     Move** moves,
                     int* index,
                     int* size) {
    getMoves_Diagonal(board, color, x, y, moves, index, size);
    return;
}

void getMoves_Rook(Piece board[8][8],
                   Color color,
                   int x, int y,
                   Move** moves,
                   int* index,
                   int* size) {
    getMoves_Orthogonal(board, color, x, y, moves, index, size);
    return;
}

void getMoves_Queen(Piece board[8][8],
                    Color color,
                    int x, int y,
                    Move** moves,
                    int* index,
                    int* size) {
    getMoves_Diagonal(board, color, x, y, moves, index, size);
    getMoves_Orthogonal(board, color, x, y, moves, index, size);
    return;
}

void getMoves_King(Piece board[8][8],
                   Color color,
                   int x, int y,
                   Move** moves,
                   int* index,
                   int* size) {
    int x2, y2;
    for(int i=-1; i<2; i++) {
        for(int j=-1; j<2; j++) {
            x2=x+i; y2=y+j;
            if(isOnBoard(x2, y2)) {
                if(board[x2][y2].color!=color) {
                    if(checkMove(board, color, x, y, x2, y2)) {
                        addMoveToList(x, y, x2, y2, moves, index, size);
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
                    addMoveToList(x, y, 2, y, moves, index, size);
                }
            }
        }
        if(board[7][y].pieceType==ROOK&&board[7][y].canSpecialMove) {
            if(board[5][y].color==NONE &&
               board[6][y].color==NONE) {
                if(checkMove_Castle(board, color, x, y, 6, y)) {
                    addMoveToList(x, y, 6, y, moves, index, size);
                }
            }
        }
    }
}

void getMoves_FromSquare(Piece board[8][8],
                        Color color,
                        int x, int y,
                        Move** moves,
                        int* index,
                        int* size) {
    
    switch(board[x][y].pieceType) {
        case PAWN:
            getMoves_Pawn(board, color, x, y, moves, index, size);
            break;
        case KNIGHT:
            getMoves_Knight(board, color, x, y, moves, index, size);
            break;
        case BISHOP:
            getMoves_Bishop(board, color, x, y, moves, index, size);
            break;
        case ROOK:
            getMoves_Rook(board, color, x, y, moves, index, size);
            break;
        case QUEEN:
            getMoves_Queen(board, color, x, y, moves, index, size);
            break;
        case KING:
            getMoves_King(board, color, x, y, moves, index, size);
            break;
    }
    return;
}


int getMoves(Piece board[8][8], Color color, Move** moves) {
    int index = 0, size = 10;
    for(int j=0; j<8; j++) {
        for(int i=0; i<8; i++) {
            if(board[i][j].color==color) {
                getMoves_FromSquare(board, color, i, j, moves, &index, &size);
            }
        }
    }
    return index;
}