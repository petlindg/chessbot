#include <stdbool.h>

#define TRUE 1
#define FALSE 0

typedef enum PieceTypes {
    EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
} PieceType;

typedef enum Colors {
    NONE, WHITE, BLACK
} Color;

typedef struct Squares {
    unsigned char x;
    unsigned char y;
} Square;

typedef struct Moves {
    Square from;
    Square to;
    PieceType promoteTo;
} Move;

typedef struct Pieces {
    Color color;
    PieceType pieceType;
    bool canEnpassante_left;
    bool canEnpassante_right;
    bool canSpecialMove;
} Piece;