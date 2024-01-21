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
    if(node->children==0) {
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

//returns 0 if tie, 1 if white win and 2 if black wins
int MCTS_r(Piece board[8][8],
            Color playColor,
            Color otherColor,
            Node* node, 
            int ttl) {

    node->plays+=2;

    if(!node->children) {
        initNode(board, playColor, node);
    }
    if(node->numberOfChildren==0) {
        if(playColor == WHITE) {
            node->wins+=2;
            return 1;
        } else if(playColor == BLACK) {
            return 2;
        }
    } else if(ttl<=0) {
        node->wins++;
        return 0;
    } else {
        Piece copiedBoard[8][8];
        copyBoard(board, copiedBoard);
        int r = rand()%node->numberOfChildren;
        movePiece(copiedBoard, node->children[r]->move);
        int a = MCTS_r(copiedBoard, otherColor, playColor, node->children[r], ttl-1);
        if(a==1) {
            node->wins+=2;
        } else if(a==0) {
            node->wins++;
        }
        return a;
    }
}

Move MCTS(Piece board[8][8],
          Color playColor,
          int iterations,
          Node** node) {
    
    int r;
    Color otherColor;
    if(playColor==WHITE) {
        otherColor = BLACK;
    } else {
        otherColor = WHITE;
    }
    
    for(int i=0; i<iterations; i++) {
        MCTS_r(board, playColor, otherColor, *node, 400);
    }

    int playIndex;
    float playValue, curr;
    Move move;
    if(playColor==WHITE) {
        playValue = 0;
        for(int i=0; i<(*node)->numberOfChildren; i++) {
            if((*node)->children[i]->plays) {
                curr = (float)(*node)->children[i]->wins/(float)(*node)->children[i]->plays;
                if(curr>playValue) {
                    playValue=curr;
                    playIndex = i;
                }
            }
        }
    } else {
        playValue = 1;
        for(int i=0; i<(*node)->numberOfChildren; i++) {
            if((*node)->children[i]->plays) {
                curr = (float)(*node)->children[i]->wins/(float)(*node)->children[i]->plays;
                if(curr<playValue) {
                    playValue=curr;
                    playIndex = i;
                }
            }
        }
    }

    for(int i=0; i<(*node)->numberOfChildren; i++) {
        if(i!=playIndex) {
            freeTree((*node)->children[i]);
        }
    }

    *node = (*node)->children[playIndex];
    move = (*node)->move;
    return move;
}

//returns true if it finds a path with higher evaluation
bool getBestMove_r(Piece board[8][8],
                   Color playColor,
                   Color otherColor,
                   int depth,
                   int* eval) {

    //printf("eval:%d depth:%d\n", *eval, depth);
    
    if(depth<=0) {
        int newEval = evalBoard(board, playColor);
        //printBoard(board);
        //printf("eval:%d newEval:%d\n", *eval, newEval);
        if(newEval>*eval) {
            *eval=newEval;
            return true;
        }
        return false;
    }

    Piece copiedBoard[8][8];
    Move* moves = malloc(10*sizeof(Move));
    int size = getMoves(board, playColor, &moves);

    if(!size) {
        free(moves);
        return false;
    }

    //printMoves(moves, size);
    for(int i=0; i<size; i++) {
        copyBoard(board, copiedBoard);
        movePiece(copiedBoard, moves[i]);
        //printBoard(copiedBoard);
        
        *eval=-*eval;
        bool newBestMove = getBestMove_r(copiedBoard, otherColor, playColor, depth-1, eval);
        *eval=-*eval;

        if(newBestMove) {
            free(moves);
            return true;
        }
    }
    free(moves);
    return false;
}

// find best move brute force depth first
// bool color 0 white 1 black
Move getBestMove(Piece board[8][8],
                  Color playColor,
                  int depth,
                  Move* moves,
                  int size) {

    Piece copiedBoard[8][8];
    Move move;
    int eval = 2000;

    Color otherColor;
    if(playColor==WHITE) {
        otherColor=BLACK;
    } else {
        otherColor=WHITE;
    }

    for(int i=0; i<size; i++) {
        copyBoard(board, copiedBoard);
        movePiece(copiedBoard, moves[i]);

        eval = -eval;
        bool newBestMove = getBestMove_r(copiedBoard, otherColor, playColor, depth-1, &eval);
        eval = -eval;

        if(newBestMove) {
            printf("i:%d\n", i);
            printMove(moves[i]);
            move=moves[i];
            /*
            printf("i:%d\n", i);
            printf("playcolor:");
            if(playColor=WHITE){
                printf("WHITE\n");
            }
            else {
                printf("BLACK\n");
            }
            printf("eval:%d\n", eval);

            printf("size:%d\n", size);
            printf("move:");
            printMove(move);

            int c = getchar();
            */
        }
    }
    free(moves);
    return move;
}