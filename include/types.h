#include <stdbool.h>
#define DEPTH 3

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

typedef struct MCTSNode {
    int wins;       // for white
    int plays;
    Move move;
    struct MCTSNode** children;
    int numberOfChildren;
} Node;

typedef struct States {
    Piece board[8][8];
    Color playColor;
    Color oppColor;
    Move* moveList;
    int moveListSize;
    int moveCount;
    Move move;
} State;
