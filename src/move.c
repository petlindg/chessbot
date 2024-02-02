#include <stdio.h> //used for error messages

#include <stdlib.h>
#include <stdbool.h>
#include "types.h"
#include "board.h"
#include "check.h"

//libraries only used for debugging
#include "interface.h"

void movePiece_Pawn(Piece board[8][8], unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
    Color color = board[x1][y1].color;
    Square sq;
    sq.y=y2;
    if(y2-y1==2) { //white pawn double move
        sq.x=x2-1;
        if(isOnBoard(sq)) { //check left, set left enPassante right
            if(board[sq.x][sq.y].pieceType==PAWN &&
               board[sq.x][sq.y].color!=color) {
                board[sq.x][sq.y].canEnpassante_right=true;
            }
        }
        sq.x=x2+1;
        if(isOnBoard(sq)) { //check left, set left enPassante right
            if(board[sq.x][sq.y].pieceType==PAWN &&
               board[sq.x][sq.y].color!=color) {
                board[sq.x][sq.y].canEnpassante_left=true;
            }
        }
    } else if(y1-y2==2) { //black pawn double move
        sq.x=x2-1;
        if(isOnBoard(sq)) { //check left, set left enPassante right
            if(board[sq.x][sq.y].pieceType==PAWN &&
               board[sq.x][sq.y].color!=color) {
                board[sq.x][sq.y].canEnpassante_right=true;
            }
        }
        sq.x=x2+1;
        if(isOnBoard(sq)) { //check left, set left enPassante right
            if(board[sq.x][sq.y].pieceType==PAWN &&
               board[sq.x][sq.y].color!=color) {
                board[sq.x][sq.y].canEnpassante_left=true;
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


void addMove(Move* move, Move** moves, int *index, int* size) {
    if(*index>=*size) {
        *size+=10;
        (*moves) = (Move*) realloc(*moves, (sizeof(Move)*(*size)));
        if(!*moves) {
            printf("moves realloc failed");
            exit(0);
        }
    }

    (*moves)[*index]=*move;

    (*index)++;
    return;
}


bool checkMove_Pawn(Piece board[8][8],
                    Color color,
                    Move* move) {
    unsigned char x1 = move->from.x;
    unsigned char y1 = move->from.y;
    unsigned char x2 = move->to.x;
    unsigned char y2 = move->to.y;
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
                      Move* move) {
    if(isCheck(board, color)) {
        return false;
    }
    unsigned char x1 = move->from.x;
    unsigned char y1 = move->from.y;
    unsigned char x2 = move->to.x;
    unsigned char y2 = move->to.y;

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

bool checkMove_Default(Piece board[8][8],
                        Color color,
                        Move* move) {
    unsigned char x1 = move->from.x;
    unsigned char y1 = move->from.y;
    unsigned char x2 = move->to.x;
    unsigned char y2 = move->to.y;
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


//checks if a move is valid, returns true if it is, false if it isn't
bool checkMove(Piece board[8][8],
               Color color,
               Move* move) {

    PieceType type = board[move->from.x][move->from.y].pieceType;
    if(type==PAWN) {
        return checkMove_Pawn(board, color, move);
    }
    if(type==KING&&(move->from.x-move->to.x==2||move->to.x-move->from.x==2)) {
        return checkMove_Castle(board, color, move);
    }
    else {
        return checkMove_Default(board, color, move);
    }
}


void maybeAddMove(Piece board[8][8], Move* move, Color color, Move** moves, int* index, int* size) {
    if(checkMove(board, color, move)) {
        addMove(move, moves, index, size);
    }
    return;
}

void getMoves_Pawn_White(Piece board[8][8],
                  Square from,
                  Move** moves,
                  int* index,
                  int* size) {
    Move move;
    move.from = from;

    //Add default moves and promotion through default moves
    move.to.x=from.x;
    move.to.y=from.y+1;
    if(isOnBoard(move.to)) {
        if(board[move.to.x][move.to.y].pieceType==EMPTY) {
            if(checkMove(board, WHITE, &move)) {
                if(move.to.y==7) {
                    move.promoteTo=KNIGHT;
                    addMove(&move, moves, index, size);
                    move.promoteTo=BISHOP;
                    addMove(&move, moves, index, size);
                    move.promoteTo=ROOK;
                    addMove(&move, moves, index, size);
                    move.promoteTo=QUEEN;
                    addMove(&move, moves, index, size);
                } else {
                    addMove(&move, moves, index, size);
                }
            }
        }
    }
    //Add take moves left
    move.to.x=from.x-1;
    if(isOnBoard(move.to)) {
        if(board[move.from.x][move.from.y].canEnpassante_left) {
            maybeAddMove(board, &move, WHITE, moves, index, size);
            board[move.from.x][move.from.y].canEnpassante_left=false;
        } else if(board[move.to.x][move.to.y].color==BLACK) {
            if(checkMove(board, WHITE, &move)) {
                if(move.to.y==7) {
                    move.promoteTo=KNIGHT;
                    addMove(&move, moves, index, size);
                    move.promoteTo=BISHOP;
                    addMove(&move, moves, index, size);
                    move.promoteTo=ROOK;
                    addMove(&move, moves, index, size);
                    move.promoteTo=QUEEN;
                    addMove(&move, moves, index, size);
                } else {
                    addMove(&move, moves, index, size);
                }
            }
        }
    }
    //Add take moves right
    move.to.x=from.x+1;
    if(isOnBoard(move.to)) {
        if(board[move.from.x][move.from.y].canEnpassante_right) {
            maybeAddMove(board, &move, WHITE, moves, index, size);
            board[move.from.x][move.from.y].canEnpassante_right=false;
        } else if(board[move.to.x][move.to.y].color==BLACK) {
            if(checkMove(board, WHITE, &move)) {
                if(move.to.y==7) {
                    move.promoteTo=KNIGHT;
                    addMove(&move, moves, index, size);
                    move.promoteTo=BISHOP;
                    addMove(&move, moves, index, size);
                    move.promoteTo=ROOK;
                    addMove(&move, moves, index, size);
                    move.promoteTo=QUEEN;
                    addMove(&move, moves, index, size);
                } else {
                    addMove(&move, moves, index, size);
                }
            }
        }
    }
    //Add double moves
    move.to.x=move.from.x;
    move.to.y=from.y+2;
    if(move.from.y==1) {
        if(board[move.to.x][move.to.y-1].pieceType==EMPTY &&
           board[move.to.x][move.to.y].pieceType==EMPTY) {
            maybeAddMove(board, &move, WHITE, moves, index, size);
       }
    }
    return;
}

void getMoves_Pawn_Black(Piece board[8][8],
                  Square from,
                  Move** moves,
                  int* index,
                  int* size) {
    Move move;
    move.from = from;

    //Add default moves and promotion through default moves
    move.to.x=move.from.x;
    move.to.y=move.from.y-1;
    if(isOnBoard(move.to)) {
        if(board[move.to.x][move.to.y].pieceType==EMPTY) {
            if(checkMove(board, BLACK, &move)) {
                if(move.to.y==0) {
                    move.promoteTo=KNIGHT;
                    addMove(&move, moves, index, size);
                    move.promoteTo=BISHOP;
                    addMove(&move, moves, index, size);
                    move.promoteTo=ROOK;
                    addMove(&move, moves, index, size);
                    move.promoteTo=QUEEN;
                    addMove(&move, moves, index, size);
                } else {
                    addMove(&move, moves, index, size);
                }
            }
        }
    }
    //Add take moves left
    move.to.x=move.from.x-1;
    if(isOnBoard(move.to)) {
        if(board[move.from.x][move.from.y].canEnpassante_left) {
            maybeAddMove(board, &move, BLACK, moves, index, size);
            board[move.from.x][move.from.y].canEnpassante_left=false;
        } else if(board[move.to.x][move.to.y].color==WHITE) {
            if(checkMove(board, BLACK, &move)) {
                if(move.to.y==0) {
                    move.promoteTo=KNIGHT;
                    addMove(&move, moves, index, size);
                    move.promoteTo=BISHOP;
                    addMove(&move, moves, index, size);
                    move.promoteTo=ROOK;
                    addMove(&move, moves, index, size);
                    move.promoteTo=QUEEN;
                    addMove(&move, moves, index, size);
                } else {
                    addMove(&move, moves, index, size);
                }
            }
        }
    }
    //Add take moves right
    move.to.x=from.x+1;
    if(isOnBoard(move.to)) {
        if(board[move.from.x][move.from.y].canEnpassante_right) {
            maybeAddMove(board, &move, BLACK, moves, index, size);
            board[move.from.x][move.from.y].canEnpassante_right=false;
        } else if(board[move.to.x][move.to.y].color==WHITE) {
            if(checkMove(board, BLACK, &move)) {
                if(move.to.y==0) {
                    move.promoteTo=KNIGHT;
                    addMove(&move, moves, index, size);
                    move.promoteTo=BISHOP;
                    addMove(&move, moves, index, size);
                    move.promoteTo=ROOK;
                    addMove(&move, moves, index, size);
                    move.promoteTo=QUEEN;
                    addMove(&move, moves, index, size);
                } else {
                    addMove(&move, moves, index, size);
                }
            }
        }
    }
    //Add double moves
    move.to.x=move.from.x;
    move.to.y=from.y-2;
    if(move.from.y==6) {
        if(board[move.to.x][move.to.y+1].pieceType==EMPTY &&
           board[move.to.x][move.to.y].pieceType==EMPTY) {
            maybeAddMove(board, &move, BLACK, moves, index, size);
       }
    }
    return;
}

void getMoves_Pawn(Piece board[8][8],
                   Color color,
                   Square from,
                   Move** moves,
                   int* index,
                   int* size) {
    if(color==WHITE) {
        getMoves_Pawn_White(board, from, moves, index, size);
    } else {
        getMoves_Pawn_Black(board, from, moves, index, size);
    }
}

void getMoves_Knight(Piece board[8][8],
                  Color color,
                  Square from,
                  Move** moves,
                  int* index,
                  int* size) {

    int xOffsetList[] = {-1, 1, -2, 2, -2, 2, -1, 1};
    int yOffsetList[] = {2, 2, 1, 1, -1, -1, -2, -2};
    int len = 8;
    
    Move move;
    move.from = from;

    for(int i=0; i<len; i++) {
        move.to.x = move.from.x+xOffsetList[i];
        move.to.y = move.from.y+yOffsetList[i];
        if(isOnBoard(move.to)) {
            if(board[move.to.x][move.to.y].color==EMPTY ||
               board[move.to.x][move.to.y].color!=color) {
                maybeAddMove(board, &move, color, moves, index, size);
            }
        }
    }
}

void getMoves_Diagonal(Piece board[8][8],
                       Color color,
                       Square from,
                       Move** moves,
                       int* index,
                       int* size) {
    Move move;
    move.from = from;

    //Add moves down left diagonal
    for(int i=1; i<8; i++) {
        move.to.x=move.from.x-i;
        move.to.y=move.from.y-i;
        if(isOnBoard(move.to)) {
            if(board[move.to.x][move.to.y].pieceType==EMPTY) {
                maybeAddMove(board, &move, color, moves, index, size);
            } else if(board[move.to.x][move.to.y].color!=color) {
                maybeAddMove(board, &move, color, moves, index, size);
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    //Add moves down right diagonal
    for(int i=1; i<8; i++) {
        move.to.x=move.from.x+i;
        move.to.y=move.from.y-i;
        if(isOnBoard(move.to)) {
            if(board[move.to.x][move.to.y].pieceType==EMPTY) {
                maybeAddMove(board, &move, color, moves, index, size);
            } else if(board[move.to.x][move.to.y].color!=color) {
                maybeAddMove(board, &move, color, moves, index, size);
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    //Add moves up left diagonal
    for(int i=1; i<8; i++) {
        move.to.x=move.from.x-i;
        move.to.y=move.from.y+i;
        if(isOnBoard(move.to)) {
            if(board[move.to.x][move.to.y].pieceType==EMPTY) {
                maybeAddMove(board, &move, color, moves, index, size);
            } else if(board[move.to.x][move.to.y].color!=color) {
                maybeAddMove(board, &move, color, moves, index, size);
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    //Add moves up right diagonal
    for(int i=1; i<8; i++) {
        move.to.x=move.from.x+i;
        move.to.y=move.from.y+i;
        if(isOnBoard(move.to)) {
            if(board[move.to.x][move.to.y].pieceType==EMPTY) {
                maybeAddMove(board, &move, color, moves, index, size);
            } else if(board[move.to.x][move.to.y].color!=color) {
                maybeAddMove(board, &move, color, moves, index, size);
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
                         Square from,
                         Move** moves,
                         int* index,
                         int* size) {
    Move move;
    move.from = from;

    // Add horizontal moves
    move.to.y=move.from.y;
    for(int i=1; i<8; i++) {
        move.to.x=move.from.x-i;
        if(isOnBoard(move.to)) {
            if(board[move.to.x][move.to.y].pieceType==EMPTY) {
                maybeAddMove(board, &move, color, moves, index, size);
            } else if(board[move.to.x][move.to.y].color!=color) {
                maybeAddMove(board, &move, color, moves, index, size);
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    for(int i=1; i<8; i++) {
        move.to.x=move.from.x+i;
        if(isOnBoard(move.to)) {
            if(board[move.to.x][move.to.y].pieceType==EMPTY) {
                maybeAddMove(board, &move, color, moves, index, size);
            } else if(board[move.to.x][move.to.y].color!=color) {
                maybeAddMove(board, &move, color, moves, index, size);
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    //Add vertical moves
    move.to.x=move.from.x;
    for(int i=1; i<8; i++) {
        move.to.y=move.from.y-i;
        if(isOnBoard(move.to)) {
            if(board[move.to.x][move.to.y].pieceType==EMPTY) {
                maybeAddMove(board, &move, color, moves, index, size);
            } else if(board[move.to.x][move.to.y].color!=color) {
                maybeAddMove(board, &move, color, moves, index, size);
                break;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    for(int i=1; i<8; i++) {
        move.to.y=move.from.y+i;
        if(isOnBoard(move.to)) {
            if(board[move.to.x][move.to.y].pieceType==EMPTY) {
                maybeAddMove(board, &move, color, moves, index, size);
            } else if(board[move.to.x][move.to.y].color!=color) {
                maybeAddMove(board, &move, color, moves, index, size);
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
                     Square from,
                     Move** moves,
                     int* index,
                     int* size) {
    getMoves_Diagonal(board, color, from, moves, index, size);
    return;
}

void getMoves_Rook(Piece board[8][8],
                   Color color,
                   Square from,
                   Move** moves,
                   int* index,
                   int* size) {
    getMoves_Orthogonal(board, color, from, moves, index, size);
    return;
}

void getMoves_Queen(Piece board[8][8],
                    Color color,
                    Square from,
                    Move** moves,
                    int* index,
                    int* size) {
    getMoves_Diagonal(board, color, from, moves, index, size);
    getMoves_Orthogonal(board, color, from, moves, index, size);
    return;
}

void getMoves_King(Piece board[8][8],
                   Color color,
                   Square from,
                   Move** moves,
                   int* index,
                   int* size) {
    Move move;
    move.from = from;

    //adds default king moves
    for(int i=-1; i<2; i++) {
        for(int j=-1; j<2; j++) {
            move.to.x=move.from.x+i;
            move.to.y=move.from.y+j;
            if(isOnBoard(move.to)) {
                if(board[move.to.x][move.to.y].color!=color) {
                    maybeAddMove(board, &move, color, moves, index, size);
                }
            }
        }
    }
    //adds castling moves
    if(board[move.from.x][move.from.y].canSpecialMove) {
        if(board[0][move.from.y].pieceType==ROOK&&board[0][move.from.y].canSpecialMove) {
            if(board[1][move.from.y].color==NONE &&
               board[2][move.from.y].color==NONE &&
               board[3][move.from.y].color==NONE) {
                move.to.x=2;
                move.to.y=move.from.y;
                maybeAddMove(board, &move, color, moves, index, size);
            }
        }
        if(board[7][move.from.y].pieceType==ROOK&&board[7][move.from.y].canSpecialMove) {
            if(board[5][move.from.y].color==NONE &&
               board[6][move.from.y].color==NONE) {
                move.to.x=6;
                move.to.y=move.from.y;
                maybeAddMove(board, &move, color, moves, index, size);
            }
        }
    }

    return;
}

void getMoves_FromSquare(Piece board[8][8],
                        Color color,
                        Square from,
                        Move** moves,
                        int* index,
                        int* size) {
    
    switch(board[from.x][from.y].pieceType) {
        case PAWN:
            getMoves_Pawn(board, color, from, moves, index, size);
            break;
        case KNIGHT:
            getMoves_Knight(board, color, from, moves, index, size);
            break;
        case BISHOP:
            getMoves_Bishop(board, color, from, moves, index, size);
            break;
        case ROOK:
            getMoves_Rook(board, color, from, moves, index, size);
            break;
        case QUEEN:
            getMoves_Queen(board, color, from, moves, index, size);
            break;
        case KING:
            getMoves_King(board, color, from, moves, index, size);
            break;
    }
    return;
}


int getMoves(Piece board[8][8], Color color, Move** moves) {
    int index = 0, size = 10;
    Square square;
    for(int j=0; j<8; j++) {
        for(int i=0; i<8; i++) {
            square.x=i; square.y=j;
            if(board[i][j].color==color) {
                getMoves_FromSquare(board, color, square, moves, &index, &size);
            }
        }
    }
    return index;
}