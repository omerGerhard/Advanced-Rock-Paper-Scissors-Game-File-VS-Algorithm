//author: Omer Gerhard & Itay Fiszman
//PiecePosition implementation

#ifndef MYPIECEPOSITION_H_
#define MYPIECEPOSITION_H_


#include "PiecePosition.h"
#include "MyPoint.h"
#include <iostream>



class MyPiecePosition : public PiecePosition{

private:
	//Piece *_piece;
	MyPoint _point;
	char _pieceSymbol;
	char _jokerRep;

public:

	//ctor for MyPiecePosition
	MyPiecePosition(MyPoint point, char pieceSymbol, char jokerRep = '#'): _point(point), _pieceSymbol(pieceSymbol), _jokerRep(jokerRep){}

	//dtor for MyPiecePosition
	~MyPiecePosition() override;

	const Point& getPosition()const override;

	char getPiece()const override;

	char getJokerRep()const override;
};


#endif /* MYPIECEPOSITION_H_ */
