//author: Omer Gerhard & Itay Fiszman
//FightInfo implementation

#ifndef MYFIGHTINFO_H_
#define MYFIGHTINFO_H_

class Piece204156384;
class Player;
class Point;
#include "FightInfo.h"

class MyFightInfo : public FightInfo{

	Point& point;
	Piece204156384& _playerPiece;
	Piece204156384& _opponent;


public:
	MyFightInfo(Point& fightPos, Piece204156384& playerPiece, Piece204156384& opponent) : point(fightPos), _playerPiece(playerPiece), _opponent(opponent){}

	~MyFightInfo() {}

	const Point& getPosition()const override;

	virtual char getPiece(int player) const override; // R, P, S, B or F (but NOT J)

	int getWinner() const override; // 0 - both lost, 1 - player 1 won, 2 - player 2 won

	bool IsValid();

};




#endif /* MYFIGHTINFO_H_ */
