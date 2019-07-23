#ifndef RULES_H
#define RULES_H

#include <vector>
#include <stack>
#include "mv.h"

using namespace std;

/*
 *  A king cannot move out of the King Row until the next turn.
 *
 */

class Position {
public:
    int x;
    int y;
    int type;	// 0 == blank
    			// 1 == black normal
    			// 2 == black king
    			// 3 == red normal
    			// 4 == red king
    Position(int x, int y, int type);
    Position();
};


class Move {
public:
    Position source;
    Position destination;
    vector<Position> takenPieces;
    bool king = false;

    Move ();
};

class node{
public:
	Move bestMove;
	int value;

	node(Move m, int x):bestMove(m), value(x){};
};

class Board{
private:
	// checking if piece it's moving to is blank, and the piece it
	// is jumping over is of the opposite color
	// assuming the piece that is being moved is valid for that turn
	bool jumpDownRight(int x, int y);
	bool jumpDownLeft(int x, int y);
	bool jumpUpRight(int x, int y);
	bool jumpUpLeft(int x, int y);

	// check if the piece it's moving to is blank
	// assuming the piece that is being moved is valid for that turn
	bool moveDownRight(int x, int y);
	bool moveDownLeft(int x, int y);
	bool moveUpRight(int x, int y);
	bool moveUpLeft(int x, int y);

    // returns a vector of all possible moves
    // Move m will be updated and passed along until no jumps or moves are possible
    // before being called, the source of Move m is set to Position(x,y)
    void allPossibleMoves(Position curr, Position source,
    		vector<Position> takenPieces, bool firstMove,
			bool &jumps, vector<Move> &possibleMoves);

    int defense(int x, int j, bool player);
    node alphaBeta (int depth, bool maximizingPlayer, int alpha, int beta);

public:
    // row, column
    // y, x
    int grid [8][8];
    bool turnCount = false;
    stack<Move> history;

    // 0 == blank
    // 1 == black normal
    // 2 == black king
    // 3 == red normal
    // 4 == red king
    Board();
    void printBoard();
    vector<Move> allMoves(bool turn);
    bool gameOver();
    void undo();
    void applyMove(Move m);
    int evaluate(bool player); //black is true, false is red
    Move bestMove();
    void setBoard(const BoardMV &bo);
};



void printMove(Move m);

#endif
