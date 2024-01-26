#include <stdio.h>

#include <stdlib.h>
#include <stdbool.h>
#include "types.h"
#include "check.h"
#include "interface.h"
#include "move.h"
#include "board.h"

int evalPiece(Piece piece, Color color) {
    int val;
    switch(piece.pieceType) {
        case PAWN:
            val = 1;
            break;
        case BISHOP:
            val = 3;
            break;
        case KNIGHT:
            val = 3;
            break;
        case ROOK:
            val = 5;
            break;
        case QUEEN:
            val = 9;
            break;
        default:
            return 0;
    }
    if(piece.color != color) {
        val *= -1;
    }
    return val;
}

// evaluates gain in favor of color {color}
int evalBoard(Piece board[8][8], Color color) {
    int val = 0;
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            val+=evalPiece(board[i][j], color);
        }
    }
    return val;
}

void initNode(Piece board[8][8],
                   Color playColor,
                   Node* node) {

    Move* moves = malloc(10*sizeof(Move));
    int size;
    if(node->children==NULL) {
        size = getMoves(board, playColor, &moves);
        node->children = malloc(size*sizeof(Node*));
        node->numberOfChildren = size;
        for(int i=0; i<size; i++) {
            node->children[i] = malloc(sizeof(Node));
            node->children[i]->move = moves[i];
            node->children[i]->children = NULL;    //ensure children pointer is null until initialized
            node->children[i]->plays = 0;
            node->children[i]->wins = 0;
        }
    } else {
        printf("ERROR: node double initialization\n");
        char c = getchar();
    }
    free(moves);
    return;
}

void freeTree(Node* node) {
    if(!node->children) {
        free(node);
        return;
    }
    for(int i=0; i<node->numberOfChildren; i++) {
        freeTree(node->children[i]);
    }
    free(node);
    return;
}




int negaMax_r(Piece board[8][8], Color playColor, Color otherColor, int depth) {
    Move* moves = malloc(10*sizeof(Move));
    Piece copiedBoard[8][8];
    int score, max=-9999;
    int size = getMoves(board, playColor, &moves);

    if(!depth) {
        return evalBoard(board, playColor);
    }

    if(!size) {
        if(isCheck(board, playColor)) {
            return -999;
        } else {
            return 0;
        }
    }

    for(int i=0; i<size; i++) {
        copyBoard(board, copiedBoard);
        movePiece(copiedBoard, moves[i]);

        score = -negaMax_r(copiedBoard, otherColor, playColor, depth-1);
        if(score>max) {
            max = score;
        }
    }
    return max;
}

Move negaMax(Piece board[8][8], Color playColor, Color otherColor, int depth) {
    Move* moves = malloc(10*sizeof(Move));
    Move move;
    Piece copiedBoard[8][8];
    int score, max=-9999;
    int size = getMoves(board, playColor, &moves);

    for(int i=0; i<size; i++) {
        printf("%dth move...\n", i);
        copyBoard(board, copiedBoard);
        movePiece(copiedBoard, moves[i]);

        score = -negaMax_r(copiedBoard, otherColor, playColor, depth-1);
        if(score>max) {
            max = score;
            move = moves[i];
        }
    }
    return move;
}