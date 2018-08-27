//author: Omer Gerhard & Itay Fiszman
//Piece base class, piece functionality implemented by the derived classes

#ifndef PIECE_H_
#define PIECE_H_

#include "Point.h"
#include <string>
#include <vector>

using namespace std;

class Player;
class Game;

enum class Log : int;

enum class PieceType : int{
  	JokerType,
  	BombType,
  	RockType,
  	PaperType,
  	ScissorsType,
  	FlagType,
  	NoneType
  };



enum class FightResult : int{
	Win,
	Lose,
	Tie,
	None,
	Invalid
};



class Piece204156384
{
public:


protected:

  PieceType type;

  Point* pos;

  bool captured;

  Player *player;


public:

  Player *getPlayer() const;

  Piece204156384(PieceType _type,Player * _player);

  virtual ~Piece204156384() {
	  if(pos != NULL)
		  delete pos;
  };

  void kill();

  static bool validatePieceChar(char pieceChar);

  virtual bool isWinning(PieceType) const = 0;

  virtual char getRepr() const = 0;

  virtual bool canMove() const = 0;

  bool isCaptured() const;

  void setCaptured(bool isCaptured);

  void setPos(int x, int y);

  Point *getPos() const;

  virtual PieceType getDynamicType() const;

  PieceType getType() const;

  static Piece204156384* pieceFactory(Player * player, PieceType type);

  static PieceType charToPieceType(char);

  static char pieceTypeToChar(PieceType pt);

  char formatRepr() const;

  static int validIndex(const string &toCheck, int length);

};



#endif /* PIECE_H_ */
