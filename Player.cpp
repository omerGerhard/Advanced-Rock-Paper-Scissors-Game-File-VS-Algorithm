

#include "Player.h"
#include "Piece.h"
#include "Joker.h"
#include <string>
#include <memory>
#include <vector>
#include "MyFightInfo.h"
#include "PiecePosition.h"
#include "PlayerAlgorithm.h"
#include "MyPoint.h"
#include "Game.h"



Player::Player(PlayerType _type, PlayerAlgorithm *_algo) : score(0), status(Log::Success),type(_type), algo(_algo){

	if(_type == player1){
		lower = false;
	}
	else{
		lower = true;
	}

	int index = 0, count = 0;
	for(count=0;count<ROCKS;count++){
		pieces[index+count] = Piece204156384::pieceFactory(this, PieceType::RockType);
	}
	index += count;
	for(count=0;count<PAPERS;count++){
		pieces[index+count] = Piece204156384::pieceFactory(this, PieceType::PaperType);
	}
	index += count;
	for(count=0;count<SCISSORS;count++){
		pieces[index+count] = Piece204156384::pieceFactory(this, PieceType::ScissorsType);
	}
	index += count;
	for(count=0;count<FLAGS;count++){
		pieces[index+count] = Piece204156384::pieceFactory(this, PieceType::FlagType);
	}
	index += count;
	for(count=0;count<BOMBS;count++){
		pieces[index+count] = Piece204156384::pieceFactory(this, PieceType::BombType);
	}
	index += count;
	for(count=0;count<JOKERS;count++){
		pieces[index+count] = Piece204156384::pieceFactory(this, PieceType::JokerType);
	}

}

Player::~Player(){
	for(int i=0; i<NUM_OF_PIECES; i++){
		delete pieces[i];
	}
}

Log Player::getInitialPositions(Game &game, std::vector<std::unique_ptr<FightInfo>> &fights){
	std::vector<std::unique_ptr<PiecePosition>> positions;
	algo->getInitialPositions(this->toInt(), positions);

	if(!checkCollisions(positions)){
		return Log::PieceCollision;
	}

	if((int)positions.size() == 0){
		return Log::IllegalFormat;
	}

	int countFlags = 0;

	for(size_t j=0; j<positions.size(); j++){

		if(!Piece204156384::validatePieceChar(positions[j]->getPiece())){
			return Log::IllegalFormat;
		}
		if(!MyBoard::isInBoard(positions[j]->getPosition())){
			return Log::OutOfRange;
		}
		Piece204156384 *piece = NULL;
		for(int i=0; i<NUM_OF_PIECES;i++){
		  if(positions[j]->getPiece() == this->pieces[i]->getRepr() && this->pieces[i]->getPos() == NULL){
			  piece = this->pieces[i];

			  piece->setCaptured(false);

			  if(piece->getType() == PieceType::JokerType){

				  Joker* joker = dynamic_cast<Joker *>(piece);
				  if(!joker->changeType(Piece204156384::charToPieceType(positions[j]->getJokerRep()))){
					  return Log::IllegalFormat;
				  }
			  }

			  bool valid = false;
			  unique_ptr<FightInfo> fight = game.setPiecePosition(positions[j]->getPosition(), piece, &valid);
			  if(fight){
				  if(valid){
					  fights.push_back(move(fight));
				  }
				  else{
					  return Log::PieceCollision;
				  }
			  }

			  countFlags += piece->getDynamicType() == PieceType::FlagType;
			  break;
		  }
		}

		if(piece == NULL){
			return Log::PieceOverflow;
		}
	}
	if(countFlags < FLAGS)
		return Log::MissingFlags;

	return Log::Success;
}


bool Player::checkCollisions(std::vector<std::unique_ptr<PiecePosition>>& positions){
	for(size_t i=0; i<positions.size(); i++){
		for(size_t j=i+1; j<positions.size(); j++){
			if(positions[i]->getPosition().getY() == positions[j]->getPosition().getY()
					&& positions[i]->getPosition().getX() == positions[j]->getPosition().getX()){
				return false;
			}
		}
	}
	return true;
}

PlayerType Player::getType() const{
	return type;
}

bool Player::isLower() const{
	return lower;
}

void Player::addPoint(){
	score++;
}

Log Player::getStatus()const{
	return status;
}

void Player::setStatus(Log log){
	this->status = log;
}


bool Player::canMove(const MyBoard &board){
	int movingPieces = 0;
	for(int i=0; i<NUM_OF_PIECES;i++){
		movingPieces += hasMoves(this->pieces[i],board);//!this->pieces[i]->isCaptured() && this->pieces[i]->canMove();
	}
	return movingPieces>0;
}

bool Player::hasMoves(Piece204156384 const* piece, const MyBoard &board){
	if(piece->isCaptured() || !piece->canMove()){
		return false;
	}
	Point *point = piece->getPos();

	if(point == NULL)
		return false;

	int dif [] = {-1,1};

	for(int i=0;i<2; i++){
		Piece204156384 *content = board.getBoardContent(point->getY()+dif[i],point->getX());
		if(!content || content->getPlayer() != this)
			return true;
	}

	for(int i=0;i<2; i++){
		Piece204156384 *content = board.getBoardContent(point->getY(),point->getX()+dif[i]);
		if(!content || content->getPlayer() != this)
			return true;
	}

	return false;

}

bool Player::hasFlags(){
	int capturedFlags = 0;
	for(int i=0; i<NUM_OF_PIECES;i++){
		if(this->pieces[i]->getType() == PieceType::FlagType){
			capturedFlags += this->pieces[i]->isCaptured();
		}
	}
	return capturedFlags < FLAGS;
}

string Player::toString(){
	return (type == player1) ? "player 1" : "player 2";
}

int Player::toInt(){
	return (type == player1) ? 1 : 2;
}
