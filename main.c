#include <stdio.h>
#include <stdlib.h>
#include "board.c"
#include "interface.c"

void printBoard(unsigned char board[][8]);
void printPiece(unsigned char piece);
void initBoard(unsigned char board[][8]);
void movePiece(unsigned char board[][8], unsigned char posXstart, unsigned char posYstart, unsigned char posXend, unsigned char posYend);
void promptMove(unsigned char board[][8], unsigned char turn);
void getMoves(unsigned char* moves, unsigned char board[][8], unsigned char posX, unsigned char posY);
unsigned char charToCord(char ch);

int main() {

    unsigned char turn = 0; //0 => white's turn, 1 => black's turn

    unsigned char board[8][8];
    initBoard(board);

    while(1) {
        clearScreen();
        printBoard(board);
        promptMove(board, turn);
        turn++;
    }
}