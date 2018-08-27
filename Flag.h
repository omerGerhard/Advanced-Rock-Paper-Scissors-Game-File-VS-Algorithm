//author: Omer Gerhard & Itay Fiszman
//class for piece of type flag

#ifndef FLAG_H_
#define FLAG_H_

#include "Piece.h"

class Flag : public Piece204156384{

public:

	Flag(Player * _player) : Piece204156384(PieceType::FlagType , _player){};

	bool isWinning(PieceType type) const override;

	char getRepr() const override{
		return 'F';
	}

	bool canMove() const override{
		return false;
	}
};




#endif /* FLAG_H_ */
