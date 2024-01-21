int evalBoard(Piece board[8][8], Color color);

Move getBestMove(Piece board[8][8],
                  Color playColor,
                  int depth);

Move MCTS(Piece board[8][8],
          Color playColor,
          int iterations,
          Node** node);

void freeTree(Node* node);

void initNode(Piece board[8][8],
                   Color playColor,
                   Node* node);