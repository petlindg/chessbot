#include <stdio.h>
#include <stdlib.h>

void clearScreen() {
    system("clear");
    system("cls");
}

void promptMove(unsigned char board[][8], unsigned char turn) {
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
        while(1) {
            if(tryMove(board, pos[0], pos[1], pos[2], pos[3])) return;
            printf("That move is invalid.\n");
        }
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

void printBoard(unsigned char board[][8]) {
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