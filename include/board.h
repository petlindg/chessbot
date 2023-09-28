#include <stdbool.h>

void initPiece(Piece* piece, PieceType pieceType, Color color);

void initPiece_Empty(Piece* piece);

void initBoard(Piece board[8][8]);

bool isOnBoard(Square square);