#include <stdbool.h>

/*
Pawn = X001
Knight = X010
Bishop = X011
Rook = X100
Queen = X101
King = X110
White = 0XXX
Black = 1XXX
Empty = XXX0
*/

unsigned char charToCord(char ch) {
    if(ch<'A') {            //ASCII for numericals are lower than 'A'
        return 8-(ch-48);   // 8-ch-48
    } else if (ch<'a') {    //ASCII for lower case letters are lower than 'a'
        return ch-65;
    } else return ch-97;
}

void movePiece(unsigned char board[][8], unsigned char posXstart, unsigned char posYstart, unsigned char posXend, unsigned char posYend) {
    board[posXend][posYend]=board[posXstart][posYstart];
    board[posXstart][posYstart] = 0;
}

void initBoard(unsigned char board[][8]) {
    board[0][0]=0x0C; board[1][0]=0x0A; board[2][0]=0x0B; board[3][0]=0x0D; board[4][0]=0x0E; board[5][0]=0x0B; board[6][0]=0x0A; board[7][0]=0x0C;
    for(int i=0; i<8; i++) board[i][1]=0x09;
    for(int i=0; i<8; i++) board[i][2]=0x00;
    for(int i=0; i<8; i++) board[i][3]=0x00;
    for(int i=0; i<8; i++) board[i][4]=0x00;
    for(int i=0; i<8; i++) board[i][5]=0x00;
    for(int i=0; i<8; i++) board[i][6]=0x01;
    board[0][7]=0x04; board[1][7]=0x02; board[2][7]=0x03; board[3][7]=0x05; board[4][7]=0x06; board[5][7]=0x03; board[6][7]=0x02; board[7][7]=0x04;
}

void getMoves(unsigned char* moves, unsigned char board[][8], unsigned char posX, unsigned char posY) {
    
}

bool tryMove(unsigned char board[][8], unsigned char posXstart, unsigned char posYstart, unsigned char posXend, unsigned char posYend) {
    unsigned char moves[27];
    getMoves(moves, board, posXstart, posYstart);
    
}