
#include "MyPiecePosition.h"
#include "Piece.h"
#include <string>


MyPiecePosition::~MyPiecePosition(){}

const Point& MyPiecePosition::getPosition()const {
	return _point;
}

char MyPiecePosition::getPiece()const { // R, P, S, B, J or F
	return _pieceSymbol;
}

char MyPiecePosition::getJokerRep()const { // ONLY for Joker: R, P, S or B -- non-Joker may return ‘#’
	return _jokerRep;
}
