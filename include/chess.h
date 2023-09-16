#include <stdbool.h>

void initPiece(Piece* piece, PieceType pieceType, Color color);

void initBoard(Piece board[8][8]);

bool isCheck(Piece board[8][8], Color color);

void movePiece(Piece board[8][8], Move move);

void squareToInt(Square square, int* x, int* y);

int getMoves(Piece board[8][8], Color color, Move* moveList);