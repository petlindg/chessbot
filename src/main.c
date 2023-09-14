#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "chess.h"
#include "interface.h"

int main(){
    Piece board[8][8];
    initBoard(board);
    printBoard(board);
}