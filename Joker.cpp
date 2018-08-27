
#include "Joker.h"
#include <vector>
#include <string>


PieceType Joker::getDynamicType() const {
	if(!dynamicPiece)
		return PieceType::NoneType;
	return this->dynamicPiece->getType();
}


bool Joker::changeType(PieceType type){

	if(type == PieceType::FlagType || type == PieceType::JokerType  || type == PieceType::NoneType)
		return false;

	delete dynamicPiece;
	dynamicPiece = Piece204156384::pieceFactory(player,type);

	return true;
}


