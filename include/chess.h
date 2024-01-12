#include <stdbool.h>

void initPiece(Piece* piece, PieceType pieceType, Color color);

void initPiece_Empty(Piece* piece);

void initBoard(Piece board[8][8]);

bool isOnBoard(Square square);

void findKing(Piece board[8][8], Color color, Square* kingPos);

void copyBoard(Piece boardSrc[8][8], Piece boardDst[8][8]);