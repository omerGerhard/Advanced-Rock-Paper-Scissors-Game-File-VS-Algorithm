

#include <iostream>

#include "MyBoard.h"
#include "Point.h"
#include "Player.h"
#include "Piece.h"


MyBoard::MyBoard(){
	for(int i=0; i<ROWS; i++){
		for(int j=0; j<COLS; j++){
			board[j][i] = NULL;
		}
	}
}

int MyBoard::getPlayer(const Point& pos)const{
	Piece204156384 *content = getBoardContent(pos);
	if(content == NULL){
		return 0;
	}
	return content->getPlayer()->toInt();
}

//one based input
Piece204156384 *MyBoard::getBoardContent(int row,int col) const{
	if(!isInBoard(row,col)){
		return NULL;
	}
	return board[row-1][col-1];
}

//one based input
Piece204156384 *MyBoard::getBoardContent(const Point &p) const{
	return getBoardContent(p.getY(), p.getX());
}

//one based input
void MyBoard::setBoardContent(Piece204156384* piece, int row,int col){
	this->board[row-1][col-1] = piece;
}

//one based input
void MyBoard::setBoardContent(Piece204156384* piece, const Point &p){
	return setBoardContent(piece, p.getY(), p.getX());
}

//one based input
bool MyBoard::isInBoard(int row, int col){
	return (row >= 1 && row <= ROWS && col >= 1 && col <= COLS);
}

//one based input
bool MyBoard::isInBoard(const Point &point){
	return isInBoard(point.getY(),point.getX());
}
