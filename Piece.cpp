
#include "Player.h"
#include "Piece.h"


#include "Game.h"

#include "Scissors.h"
#include "Joker.h"
#include "Rock.h"
#include "Bomb.h"
#include "Paper.h"
#include "Flag.h"

#include "MyFightInfo.h"
#include "MyPoint.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;


Piece204156384::Piece204156384(PieceType _type,Player * _player):type(_type), pos(NULL), captured(true), player(_player) {}

char Piece204156384::formatRepr() const{
	if(player->isLower())
		return tolower(getRepr());
	else
		return toupper(getRepr());
}

 Piece204156384* Piece204156384::pieceFactory(Player * player, PieceType type){
	 switch(type){
	 	 case PieceType::ScissorsType:
	 		 return new Scissors(player);
		 case PieceType::JokerType:
			 return new Joker(player);
		 case PieceType::RockType:
			 return new Rock(player);
		 case PieceType::BombType:
			 return new Bomb(player);
		 case PieceType::PaperType:
			 return new Paper(player);
		 case PieceType::FlagType:
			 return new Flag(player);
		 default:
			 return NULL;
	 }
 }

 void Piece204156384::kill(){
	 this->captured = true;
 }

 bool Piece204156384::isCaptured() const{
	 return this->captured;
 }

 void Piece204156384::setCaptured(bool isCaptured){
	 this->captured = isCaptured;
 }

 Point *Piece204156384::getPos() const{
	 return pos;
 }

 Player *Piece204156384::getPlayer() const{
	 return player;
 }

void Piece204156384::setPos(int x,int y){
	delete pos;
	pos = new MyPoint(x,y);
}



PieceType Piece204156384::charToPieceType(char c){
	switch(c){
		case 'R':
			return PieceType::RockType;
		case 'P':
			return PieceType::PaperType;
		case 'S':
			return PieceType::ScissorsType;
		case 'B':
			return PieceType::BombType;
		case 'F':
			return PieceType::FlagType;
		case 'J':
			return PieceType::JokerType;
		default:
			return PieceType::NoneType;
	}
}


char Piece204156384::pieceTypeToChar(PieceType pt){
	switch(pt){
		case PieceType::RockType:
			return 'R';
		case PieceType::PaperType:
			return 'P';
		case PieceType::ScissorsType:
			return 'S';
		case PieceType::BombType:
			return 'B';
		case PieceType::FlagType:
			return 'F';
		case PieceType::JokerType:
			return 'J';
		default:
			return '\0';
	}
}


bool Piece204156384::validatePieceChar(char pieceChar){
	return charToPieceType(pieceChar) != PieceType::NoneType;
}


PieceType Piece204156384::getDynamicType() const{
	return getType();
}

PieceType Piece204156384::getType() const{
	return this->type;
}


int Piece204156384::validIndex(const string &toCheck, int length){
	for(int i=0; i<length; i++){
		if(!('0' <= toCheck[i] && toCheck[i]<='9')){
			return 0;
		}
	}
	return 1;
}

