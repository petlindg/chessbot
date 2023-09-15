#include <stdio.h>

#include <stdbool.h>
#include <stdlib.h>

#include "types.h"
#include "chess.h"
#include "interface.h"

int main(){
    Piece board[8][8];
    initBoard(board);
    printBoard(board);
    if(isCheck(board, WHITE)) printf("white in check\n");
    if(isCheck(board, BLACK)) printf("black in check\n");
}