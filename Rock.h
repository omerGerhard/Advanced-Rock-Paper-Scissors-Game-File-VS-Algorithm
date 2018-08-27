//author: Omer Gerhard & Itay Fiszman
//class for piece of type rock


#ifndef ROCK_H_
#define ROCK_H_



#include "Piece.h"


class Rock : public Piece204156384{

public:

	Rock(Player * _player) : Piece204156384(PieceType::RockType , _player){};

	bool isWinning(PieceType type) const override;

	char getRepr() const override{
		return 'R';
	}

	bool canMove() const override{
		return true;
	}


};

#endif /* ROCK_H_ */
