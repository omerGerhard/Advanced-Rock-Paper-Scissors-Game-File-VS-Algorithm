//author: Omer Gerhard & Itay Fiszman
//board implementation


#ifndef MYBOARD_H_
#define MYBOARD_H_


#define ROWS 10
#define COLS 10

#include "Board.h"
class Piece204156384;
class Point;

class MyBoard : public Board{

private:

	Piece204156384* board[ROWS][COLS];

public:

	MyBoard();

	int getPlayer(const Point& pos)const override;

	//returns a pointer to the piece located in row 'row' and col 'col' in the current buard.
	//if this cell is empty, returns NULL
	Piece204156384* getBoardContent(int row,int col) const;

	//overload for the above method with parameter Point instead of row and col
	Piece204156384 *getBoardContent(const Point &p) const;

	void setBoardContent(Piece204156384* piece, int row,int col);

	void setBoardContent(Piece204156384* piece, const Point &p);

	//returns true if the the position ('row','col') is a part of the game board. otherwise, return false
	static bool isInBoard(int row, int col);

	//overload for the above method with parameter Point instead of row and col
	static bool isInBoard(const Point &point);


};



#endif /* MYBOARD_H_ */
