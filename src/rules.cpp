#include <iostream>
#include <stdio.h>
#include <climits>
#include "rules.h"

using namespace std;

Move::Move (){
	source = destination = Position();
}


Position::Position(int x, int y, int type){
        this->x = x;
        this->y = y;
        this->type = type;
}

Position::Position(){
        this->x = 0;
        this->y = 0;
        this->type = 0;
}

void printMove(Move m){
    cout << "(" << m.source.x << ", " << m.source.y << ")  ->  ";
    cout << "(" << m.destination.x << ", " << m.destination.y  << ")" << endl;
    cout << "Taken: ";
    for (unsigned i = 0; i < m.takenPieces.size(); i++){
        cout << "(" << m.takenPieces[i].x << ", " << m.takenPieces[i].y << "), ";
    }
    cout << endl;
}


Board::Board(){
       for (int i = 0; i < 8; i++) {
           for (int j = 0; j < 8; j++) {
               grid[i][j] = 0;
           }
       }

       // initial black positions
       grid[0][1] = 1;
       grid[0][3] = 1;
       grid[0][5] = 1;
       grid[0][7] = 1;
       grid[1][0] = 1;
       grid[1][2] = 1;
       grid[1][4] = 1;
       grid[1][6] = 1;
       grid[2][1] = 1;
       grid[2][3] = 1;
       grid[2][5] = 1;
       grid[2][7] = 1;

       // initial red positions
       grid[5][0] = 3;
       grid[5][2] = 3;
       grid[5][4] = 3;
       grid[5][6] = 3;
       grid[6][1] = 3;
       grid[6][3] = 3;
       grid[6][5] = 3;
       grid[6][7] = 3;
       grid[7][0] = 3;
       grid[7][2] = 3;
       grid[7][4] = 3;
       grid[7][6] = 3;
}

void Board::printBoard(){
        cout << "   ";
        for (int i = 0; i < 8; i++){
            cout << i << " ";
        }
        cout << "\n";

        for (int i = 0; i < 8; i++){
            cout << i << " |";
            for (int j = 0; j < 8; j++){
            	if(grid[i][j] == 0){
            		cout << ".|";
            	}else{
            		cout << grid[i][j] << "|";
            	}
                if (j == 7)
                    cout << "\n";
            }
        }
    }

bool Board::jumpDownRight(int x, int y){
    //cout << "in jdr" << endl;

    // blacks turn
    if (x + 2 >= 8 || y + 2 >= 8){
        return false;
    }

    if (turnCount){
        return ( grid[y + 2][x + 2]  == 0 &&
        		(grid[y + 1][x + 1] == 3 || grid[y + 1][x + 1] == 4) );
    } else {
        return ( grid[y + 2][x + 2]  == 0 &&
        		(grid[y + 1][x + 1] == 1 || grid[y + 1][x + 1] == 2));
    }
}

bool Board::jumpDownLeft(int x, int y) {
    //cout << "in jdl" << endl;
    if (x - 2 < 0 || y + 2 >= 8){
        return false;
    }

    // blacks turn
    if (turnCount) {
        return (grid[y + 2][x - 2] == 0 &&
        		(grid[y + 1][x - 1] == 3 || grid[y + 1][x - 1] == 4));
    } else {
        return (grid[y + 2][x - 2] == 0 &&
        		(grid[y + 1][x - 1] == 1 || grid[y + 1][x - 1] == 2));
    }
}

bool Board::jumpUpRight(int x, int y) {
	//cout << "in jur" << endl;
    if (x + 2 >= 8 || y - 2 < 0){
        return false;
    }

    // blacks turn
    if (turnCount) {
        return (grid[y - 2][x + 2] == 0
        		&& (grid[y - 1][x + 1] == 3 || grid[y - 1][x + 1] == 4));
    } else {
        return (grid[y - 2][x + 2] == 0 &&
        		(grid[y - 1][x + 1] == 1 || grid[y - 1][x + 1] == 2));
    }
}

bool Board::jumpUpLeft(int x, int y) {
	//cout << "in jul" << endl;
    if (x - 2 < 0 || y - 2 < 0){
        return false;
    }

    // blacks turn
    if (turnCount) {
        return (grid[y - 2][x - 2] == 0
        		&& (grid[y - 1][x - 1] == 3 || grid[y - 1][x - 1] == 4));
    } else {
        return (grid[y - 2][x - 2] == 0 &&
        		(grid[y - 1][x - 1] == 1 || grid[y - 1][x - 1] == 2));
    }
}

// check if the piece it's moving to is blank
// assuming the piece that is being moved is valid for that turn
bool Board::moveDownRight(int x, int y){
    //cout << "in mdr" << endl;
    if (x + 1 >= 8 || y + 1 >= 8){
        return false;
    }
    return (grid[y + 1][x + 1] == 0 && grid[y][x] != 3);
}

bool Board::moveDownLeft(int x, int y){
    //cout << "in mdl" << endl;
    if (x - 1 < 0 || y + 1 >= 8){
        return false;
    }
    return (grid[y + 1][x - 1] == 0 && grid[y][x] != 3);
}

bool Board::moveUpRight(int x, int y){
    //cout << "in mur" << endl;
    if (x + 1 >= 8 || y - 1 < 0){
        return false;
    }
    return (grid[y - 1][x + 1] == 0 && grid[y][x] != 1);
}

bool Board::moveUpLeft(int x, int y){
    //cout << "in mul" << endl;
    if (x - 1 < 0 || y - 1 < 0){
        return false;
    }
    return (grid[y - 1][x - 1] == 0 && grid[y][x] != 1);
}

// 0 == blank
// 1 == black normal
// 2 == black king
// 3 == red normal
// 4 == red king

// returns a vector of all possible moves
// Move m will be updated and passed along until no jumps or moves are possible
// before being called, the source of Move m is set to Position(x,y)
void Board::allPossibleMoves(Position curr, Position source,
		vector<Position> takenPieces, bool firstMove,
		bool &jumps, vector<Move> &possibleMoves) {

    //cout << "curr.x: " << curr.x << " curr.y: " << curr.y << endl;
    bool anyPossibleMoves = false;
    bool noJumps = true;

    /*	 problem is taken is being added to but then when we return
    	from the func it has extra pieces
    */
    if (grid[source.y][source.x] != 3 && jumpDownRight(curr.x, curr.y) ){
        bool repeatJump = false;
        Position toBeJumped(curr.x + 1, curr.y + 1, grid[curr.y+1][curr.x+1]);
        for (Position pos : takenPieces){
            if (pos.x == toBeJumped.x && pos.y == toBeJumped.y){
                repeatJump = true;
                break;
            }
        }

        Position p(curr.x+2, curr.y+2, 0);
        if ( (source.x != p.x || source.y != p.y) && !repeatJump){
            //cout << "adding piece to taken jdr: curr.x+1: " << curr.x+1 << " curr.y+1: " << curr.y+1 << endl;
            vector<Position> tempTaken = takenPieces;
            tempTaken.emplace_back(Position(curr.x+1, curr.y+1, grid[curr.y+1][curr.x+1]));
            allPossibleMoves(p, source, tempTaken, false, jumps, possibleMoves);
            noJumps = false;
            anyPossibleMoves = true;
        }
    }

    if (grid[source.y][source.x] != 3 && jumpDownLeft(curr.x, curr.y)){
        bool repeatJump = false;
        Position toBeJumped(curr.x - 1, curr.y + 1, grid[curr.y+1][curr.x-1]);
        for (Position pos : takenPieces){
            if (pos.x == toBeJumped.x && pos.y == toBeJumped.y){
                repeatJump = true;
                break;
            }
        }

        Position p(curr.x-2, curr.y+2, 0);
        if ( (source.x != p.x || source.y != p.y) && !repeatJump ){
            //cout << "adding piece to taken jdl: curr.x-1: " << curr.x-1 << " curr.y+1: " << curr.y+1 << endl;
            vector<Position> tempTaken = takenPieces;
            tempTaken.emplace_back(Position(curr.x-1, curr.y+1, grid[curr.y+1][curr.x-1]));
            allPossibleMoves(p, source, tempTaken, false, jumps, possibleMoves);
            anyPossibleMoves = true;
            noJumps = false;
        }
    }

    if (grid[source.y][source.x] != 1 && jumpUpRight(curr.x, curr.y )){
        Position toBeJumped(curr.x + 1, curr.y - 1, grid[curr.y-1][curr.x+1]);
        bool repeatJump = false;
        for (Position pos : takenPieces){
            if (pos.x == toBeJumped.x && pos.y == toBeJumped.y){
                repeatJump = true;
                break;
            }
        }

        Position p(curr.x+2, curr.y-2, 0);
        if ((source.x != p.x || source.y != p.y) && !repeatJump){
            //cout << "adding piece to taken jur: curr.x+1: " << curr.x+1 << " curr.y-1: " << curr.y-1 << endl;
            vector<Position> tempTaken = takenPieces;
            tempTaken.emplace_back(Position(curr.x+1, curr.y-1, grid[curr.y-1][curr.x+1]));
            allPossibleMoves(p, source, tempTaken, false, jumps, possibleMoves);
            noJumps = false;
            anyPossibleMoves = true;
        }
    }

    if (grid[source.y][source.x] != 1 && jumpUpLeft(curr.x, curr.y )){
        Position toBeJumped(curr.x - 1, curr.y - 1, grid[curr.y-1][curr.x-1]);
        bool repeatJump = false;
        for (Position pos : takenPieces){
            if (pos.x == toBeJumped.x && pos.y == toBeJumped.y){
                repeatJump = true;
                break;
            }
        }

        Position p(curr.x-2, curr.y-2,0);
        if ( (source.x != p.x || source.y != p.y) && !repeatJump){
            //cout << "adding piece to taken jul: curr.x-1: " << curr.x-1 << " curr.y-1: " << curr.y-1 << endl;
            vector<Position> tempTaken = takenPieces;

            tempTaken.emplace_back(Position(curr.x-1, curr.y-1, grid[curr.y-1][curr.x-1]));
            allPossibleMoves(p, source, tempTaken, false, jumps, possibleMoves);
            anyPossibleMoves = true;
            noJumps = false;
        }
    }

    // check for non-jumps here because jumps are mandatory if possible
    if (noJumps && firstMove && !jumps){
        if (moveDownRight(curr.x, curr.y)){
            Move m;
            m.source = source;
            m.destination = Position(curr.x + 1, curr.y + 1, 0);
            possibleMoves.push_back(m);
            anyPossibleMoves = true;
        }

        if (moveDownLeft(curr.x, curr.y)){
            Move m;
            m.source = source;
            m.destination = Position(curr.x - 1, curr.y + 1, 0);
            possibleMoves.push_back(m);
            anyPossibleMoves = true;
        }

        if (moveUpRight(curr.x, curr.y)){
            Move m;
            m.source = source;
            m.destination = Position(curr.x + 1, curr.y - 1, 0);
            possibleMoves.push_back(m);
            anyPossibleMoves = true;
        }

        if (moveUpLeft(curr.x, curr.y)){
            Move m;
            m.source = source;
            m.destination = Position(curr.x - 1, curr.y - 1, 0);
            possibleMoves.push_back(m);
            anyPossibleMoves = true;
        }
    }

    // no more possible moves
    //cout << "!anyPossibleMoves: " << !anyPossibleMoves << endl;

    if (!anyPossibleMoves && (curr.x != source.x || curr.y != source.y)){
    	if(!jumps){
    		possibleMoves.clear();
    		jumps = true;
    	}
        Position destination(curr.x, curr.y, 0);
        Move m;
        m.source = source;
        m.destination = destination;
        m.takenPieces = takenPieces;
        possibleMoves.push_back(m);
        //cout << "adding move to possibleMoves" << endl;
        //printMove(m);
        return;
    }
}

// turn is true if black, false if red
vector<Move> Board::allMoves(bool turn){
    vector<Move> possibleMoves;
	bool jumps = false;
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
	    		if(turn && (grid[j][i] == 1 || grid[j][i] == 2)){
	    			Move m;
	    			m.source = Position(i,j, grid[j][i]);
	    			allPossibleMoves(m.source,m.source, m.takenPieces,
	    					true, jumps, possibleMoves);
	    		}else if(!turn && (grid[j][i] == 3 || grid[j][i] == 4) ){
	    			Move m;
	    			m.source = Position(i,j, grid[j][i]);
	    			allPossibleMoves(m.source,m.source, m.takenPieces,
	    					true, jumps, possibleMoves);

	    		}
	    }
	}
	return possibleMoves;
}

bool Board::gameOver(){
	return (allMoves(turnCount).empty());
}

void Board::applyMove(Move m){
	history.push(m);
	for(Position p: m.takenPieces){
		grid[p.y][p.x] = 0;
	}
	grid[m.destination.y][m.destination.x] = grid[m.source.y][m.source.x];
	grid[m.source.y][m.source.x] = 0;
	if(m.destination.y == 0 && grid[m.destination.y][m.destination.x] == 3){
		grid[m.destination.y][m.destination.x] = 4;
		m.king = true;
	}
	else if(m.destination.y == 7 && grid[m.destination.y][m.destination.x] == 1){
		grid[m.destination.y][m.destination.x] = 2;
		m.king = true;
	}
	turnCount = !turnCount;
}

void Board::undo(){
	Move m = history.top();
	grid[m.source.y][m.source.x] = m.source.type;
	grid[m.destination.y][m.destination.x] = 0;
	for(Position p: m.takenPieces){
		grid[p.y][p.x] = p.type;
	}
	if(m.king){
		if(m.destination.y == 0){
			grid[m.source.y][m.source.x] = 3;
		}else{
			grid[m.source.y][m.source.x] = 1;
		}
		m.king = false;
	}
	history.pop();
	turnCount = !turnCount;
}

int Board::evaluate(bool player){ //black is true, false is red
	int value1 = 0, value2 = 0;
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(grid[j][i] == 1 || grid[j][i] == 2){
				//value1 += defense(i, j, true);		//defense value
				value1 += (i > 3 ) ? (3 - (i%4)) : i;

				if(grid[j][i] == 1){
					value1 += 2 + j;
					if( j == 0){
						value1 += 10;
					}
				}else{
					value1 += 20;
				}
			}else if(grid[j][i] != 0){
				//turnCount = false;
				//value2 -= defense(i, j, false);		//defense value
				value2 -= (i > 3 ) ? (3 - (i%4)) : i;

				if(grid[j][i] == 3){
					value2 -= 2 + (7-j);
					if( j == 7){
						value2 -= 10;
					}
				}else{
					value2 -= 20;
				}
			}
		}
	}
	int ans = value1+value2;
	return player ? ans : -1*ans;
}

int Board::defense(int x, int y, bool player){
	int value = 0;
	if(player && y < 7 && grid[y][x] == 1 && y > 0){	//1 | 2
		if(x > 0 && (grid[y-1][x-1] == 1 || grid[y-1][x-1] == 2)){
			value += 2;
		}
		if(x < 7 && (grid[y-1][x+1] == 1 || grid[y-1][x+1] == 2)){
			value += 2;
		}
	}else if(!player && y < 7 && grid[y][x] == 3 && y > 0){
		if(x > 0 && (grid[y+1][x-1] == 3 || grid[y+1][x-1] == 4)){
			value += 2;
		}
		if(x < 7 && (grid[y+1][x+1] == 4 || grid[y+1][x+1] == 3)){
			value += 2;
		}
	}
	return value;
}

node Board::alphaBeta (int depth, bool maximizingPlayer, int alpha, int beta){
	vector<Move> moves = allMoves(maximizingPlayer);
	Move bestMove;
	if(depth == 0 || moves.empty()){
		return node(bestMove, evaluate(maximizingPlayer));
	}

	if(maximizingPlayer){
		for(Move m: moves){
			applyMove(m);
			int result = alphaBeta(depth-1, false, alpha, beta).value;
			if(result > alpha){
				alpha = result;
				bestMove = m;
			}
			undo();
			if(alpha >= beta){
				break;
			}
		}
		return node(bestMove, alpha);
	}else{
		for(Move m: moves){
			applyMove(m);
			int result = alphaBeta(depth-1, true, alpha, beta).value;
			if(result < beta){
				beta = result;
				bestMove = m;
			}
			undo();
			if(alpha >= beta){
				break;
			}
		}
		return node(bestMove, beta);
	}
}

Move Board::bestMove(){
	node n = alphaBeta(8, true, INT_MIN, INT_MAX);
	return n.bestMove;
}

void Board::setBoard(const BoardMV &bo){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			grid[i][j] = bo.grid[7-i][7-j];
		}
	}
}



