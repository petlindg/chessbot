int evalBoard(Piece board[8][8], Color color);

void getBestMove(Piece board[8][8],
                  Color playColor,
                  Color otherColor,
                  int depth,
                  int* evalBest,
                  int* evalCurrent,
                  Move* moveBest,
                  Move* moveCurrent);