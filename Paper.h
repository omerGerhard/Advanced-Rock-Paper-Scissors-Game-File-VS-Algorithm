//author: Omer Gerhard & Itay Fiszman
//class for piece of type paper

#ifndef PAPER_H_
#define PAPER_H_



#include "Piece.h"


class Paper : public Piece204156384{

public:

	Paper(Player * _player) : Piece204156384(PieceType::PaperType , _player){};

	bool isWinning(PieceType type) const override;

	char getRepr() const override{
		return 'P';
	}

	bool canMove() const override{
		return true;
	}


};



#endif /* PAPER_H_ */
