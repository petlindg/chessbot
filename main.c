#include <stdio.h>
#include <stdlib.h>

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

void printBoard(unsigned char board[8][8]);
void printPiece(unsigned char piece);
void initBoard(unsigned char board[8][8]);
void movePiece(unsigned char board[8][8], unsigned char posXstart, unsigned char posYstart, unsigned char posXend, unsigned char posYend);
void promptMove(unsigned char board[8][8], unsigned char turn);
unsigned char charToCord(char ch);

int main() {

    unsigned char turn = 0; //0 => white's turn, 1 => black's turn

    unsigned char board[8][8];
    initBoard(&board[0][0]);

    while(1) {
        system("clear");
        system("cls");
        printBoard(&board[0][0]);
        promptMove(&board[0][0], turn);
        turn++;
    }
     
}

unsigned char charToCord(char ch) {
    if(ch<'A') {            //ASCII for numericals are lower than 'A'
        return 8-(ch-48);   // 8-ch-48
    } else if (ch<'a') {    //ASCII for lower case letters are lower than 'a'
        return ch-65;
    } else return ch-97;
}

void promptMove(unsigned char board[8][8], unsigned char turn) {
    unsigned char j;
        if(turn%2) printf("It's black's turn. Make your move.");
        else printf("It's white's turn. Make your move.");

        char input[8];
        scanf("%7[^\n]%*c", input);

        unsigned char pos[4];

        j = 0;
        for(unsigned char i = 0; i<5; i++) {
            unsigned char tmp = charToCord(input[i]);
            if(tmp<8) {
                pos[j] = tmp;
                j++;
            }
        }
        movePiece(&board[0][0], pos[0], pos[1], pos[2], pos[3]);
}

void movePiece(unsigned char board[8][8], unsigned char posXstart, unsigned char posYstart, unsigned char posXend, unsigned char posYend) {
    board[posXend][posYend]=board[posXstart][posYstart];
    board[posXstart][posYstart] = 0;
}

void printBoard(unsigned char board[8][8]) {
    printf("%s", "-------------------------------\n\n");
        for(int i = 0; i<8; i++) {
            printf("%d  ", 8-i);
            for(int j = 0; j<8; j++) {
            printf(" ");
            printPiece(board[j][i]);
            }
            printf("\n");
        }
    printf("\n    A  B  C  D  E  F  G  H\n");
    printf("-------------------------------\n");
}

void printPiece(unsigned char piece) {
    piece &= piece & 0x0F;
    //print piece
    switch(piece) {
        //empty square
        case 0x0:
            printf("..");
            break;
        //white pieces
        case 0x01:
            printf("wP");
            break;
        case 0x02:
            printf("wk");
            break;
        case 0x03:
            printf("wB");
            break;
        case 0x04:
            printf("wR");
            break;
        case 0x05:
            printf("wQ");
            break;
        case 0x06:
            printf("wK");
            break;
        //black pieces
        case 0x09:
            printf("bP");
            break;
        case 0x0A:
            printf("bk");
            break;
        case 0x0B:
            printf("bB");
            break;
        case 0x0C:
            printf("bR");
            break;
        case 0x0D:
            printf("bQ");
            break;
        case 0x0E:
            printf("bK");
            break;
    }

}

void initBoard(unsigned char board[8][8]) {
    board[0][0]=0x0C; board[1][0]=0x0A; board[2][0]=0x0B; board[3][0]=0x0D; board[4][0]=0x0E; board[5][0]=0x0B; board[6][0]=0x0A; board[7][0]=0x0C;
    for(int i=0; i<8; i++) board[i][1]=0x09;
    for(int i=0; i<8; i++) board[i][2]=0x00;
    for(int i=0; i<8; i++) board[i][3]=0x00;
    for(int i=0; i<8; i++) board[i][4]=0x00;
    for(int i=0; i<8; i++) board[i][5]=0x00;
    for(int i=0; i<8; i++) board[i][6]=0x01;
    board[0][7]=0x04; board[1][7]=0x02; board[2][7]=0x03; board[3][7]=0x05; board[4][7]=0x06; board[5][7]=0x03; board[6][7]=0x02; board[7][7]=0x04;
}
