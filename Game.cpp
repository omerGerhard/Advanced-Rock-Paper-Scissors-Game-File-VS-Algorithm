

#include "Game.h"
#include "Player.h"
#include "Piece.h"
#include "MyFightInfo.h"
#include "Joker.h"
#include "FilePlayerAlgorithm.h"
#include "AutoPlayerAlgorithm.h"
#include "MyBoard.h"
#include "MyPoint.h"

#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <utility>



using std::unique_ptr;

#define OUTPUT_FILE "rps.output"

Game::Game(PlayerAlgorithm * algorithms[2]) : turn(0),numOfTurns(-1),num_turns_without_a_fight(0){
	board = MyBoard();
	this->players[0] = new Player(player1, algorithms[0]);
	this->players[1] = new Player(player2, algorithms[1]);
}


Game::~Game(){
	  delete players[0];
	  delete players[1];
}



void Game::runGame(){
	positioning();

	while(!isGameOver()){
		numOfTurns++;

		unique_ptr<Move> move = players[this->turn]->getImpAlgorithm()->getMove();
		unique_ptr<JokerChange> jokerChange = players[this->turn]->getImpAlgorithm()->getJokerChange();

		this->makeTurn(std::move(move), std::move(jokerChange));
		changeTurn();
	}

	Player *winner = calculateWinner();

	if (winner)
		winner->addPoint();

	consoleErrorMessage(*players[0]);
	consoleErrorMessage(*players[1]);

	createOutputFile(winner);
}

void Game::positioning(){

	std::vector<unique_ptr<FightInfo>> fights;

	players[0]->setStatus(players[0]->getInitialPositions(*this, fights));
	players[1]->setStatus(players[1]->getInitialPositions(*this, fights));

	if(this->players[this->turn]->getStatus() == Log::Success && !this->players[this->turn]->hasFlags())
		this->players[this->turn]->setStatus(Log::CapturedFlag);

	else if(this->players[this->turn]->getStatus() == Log::Success && !this->players[this->turn]->canMove(board))
		this->players[this->turn]->setStatus(Log::CantMove);

	if(this->players[getOpponent(this->turn)]->getStatus() == Log::Success && !this->players[getOpponent(this->turn)]->hasFlags())
		this->players[getOpponent(this->turn)]->setStatus(Log::CapturedFlag);

	players[0]->getImpAlgorithm()->notifyOnInitialBoard(board,fights);
	players[1]->getImpAlgorithm()->notifyOnInitialBoard(board,fights);

}



bool Game::isGameOver() const{
	Log log1 = players[0]->getStatus();
	Log log2 = players[1]->getStatus();

	if(!isSuccess(log1) || !isSuccess(log2))
		return true;
	if(num_turns_without_a_fight > 100)
		return true;
	return false;
}

void Game::consoleErrorMessage(Player &player){
	Log status = player.getStatus();
	switch(status){
		case Log::MissingFlags:
			printf("Flags are missing for %s\n",player.toString().c_str());
			break;
		case Log::IllegalFormat:
			printf("Illegal input format for %s\n",player.toString().c_str());
			break;
		case Log::OutOfRange:
			printf("Piece position by %s out of legal range\n",player.toString().c_str());
			break;
		case Log::PieceCollision:
			printf("Piece position by %s collides with another piece of its own\n",player.toString().c_str());
			break;
		case Log::PieceOverflow:
			printf("%s positioned too many pieces of same type\n",player.toString().c_str());
			break;
		default:
			break;
	}
}

void Game::createOutputFile(const Player* winner){
	ofstream outfile(OUTPUT_FILE);

	if(!outfile){
		cout << "Error: Can't open output file." << endl;
		return;
	}

	int winnerUI = 0;
	if(winner){
		winnerUI = getPlayerIndex(winner)+1;
	}
	outfile << "Winner: " << winnerUI << endl;
	outfile << "Reason: ";

	if(numOfTurns == -1){

		if(winner == NULL){
			if(players[0]->getStatus() == Log::CapturedFlag && players[1]->getStatus() == Log::CapturedFlag){
				outfile << "A tie - all flags are eaten by both players in the positioning";
			}
			else
				outfile << "Bad Positioning input for both players" << endl;// - " << players[0]->toString() << ": line " <<players[0]->getStatus().lineError << ", " << players[1]->toString() << ": line " << players[1]->getStatus().lineError;
		}

		else{
			Player *loser = players[getOpponent(getPlayerIndex(winner))];
			if(loser->getStatus() == Log::CapturedFlag)
				outfile << "All flags of the opponent are captured";
			else if(winnerUI==2 && (players[0]->getStatus() == Log::CantMove))
				outfile << "All moving PIECEs of the opponent are eaten" ;
			else
				outfile << "Bad Positioning input for " << loser->toString() << endl;
		}
	}

	else{
		if (winner == NULL){
			if(players[0]->getStatus() == Log::CapturedFlag && players[0]->getStatus() == Log::CapturedFlag && players[0]->getStatus() == Log::CapturedFlag && players[1]->getStatus() == Log::CapturedFlag){
				outfile << "A tie - all flags are eaten by both players in the positioning" << endl;
			}
			else if(num_turns_without_a_fight > 100){
				outfile << "A tie - more then 100 moves passed without a fight" << endl;
			}
		}
		else{
			Player *loser = players[getOpponent(getPlayerIndex(winner))];
			Log status = loser->getStatus();
			switch(status){
				case Log::CantMove:
					outfile << "All moving PIECEs of the opponent are eaten" << endl;
					break;
				case Log::CapturedFlag:
					outfile << "All flags of the opponent are captured" << endl;
					break;
				default:
					outfile << "Bad Moves input for " << loser->toString() << endl;
			}
		}
	}

	outfile << endl;

	printBoard(outfile);

	outfile.close();

}

void Game::printBoard(ofstream &outfile){
	for(int row=1; row<=ROWS; row++){
		for(int col=1; col<=COLS; col++){
			Piece204156384 * piece = board.getBoardContent(row,col);
			if(piece){
				outfile << piece->formatRepr();
			}
			else{
				outfile << " ";
			}
		}
		outfile << endl;
	}
}

Player *Game::calculateWinner() const{
	Log log1 = this->players[0]->getStatus();
	Log log2 = this->players[1]->getStatus();

	if(isSuccess(log1) && !isSuccess(log2)){
		return players[0];
	}
	else if(!isSuccess(log1) && isSuccess(log2)){
		return players[1];
	}
	else if(!isSuccess(log1) && !isSuccess(log2)){
		return NULL;
	}
	//tie in case of no fight for more then 100 moves
	else if(num_turns_without_a_fight>100){
		return NULL;
	}
	return NULL;

}

bool Game::isSuccess(Log log){
	return log == Log::Success;
}

int Game::getPlayerIndex(const Player *player) const{
	if(player->getType() == player1)
		return 0;
	return 1;
}

void Game::changeTurn(){
	turn = getOpponent(this->turn);
}

int Game::getOpponent(int playerIndex) const{
	return (playerIndex+1)%2;
}




std::unique_ptr<MyFightInfo> Game::setPiecePosition(Point const &pos, Piece204156384 *piece, bool* valid){

	if(piece->getPos())
		board.setBoardContent(NULL,*(piece->getPos()));

	piece->setPos(pos.getX(),pos.getY());

	Piece204156384 *opponent = board.getBoardContent(pos);


	if(opponent == NULL){
		board.setBoardContent(piece,pos);
		*valid = true;
		return nullptr;
	}

	std::unique_ptr<MyFightInfo> fight = std::make_unique<MyFightInfo>(*(opponent->getPos()), *piece, *opponent);

	if(!fight->IsValid()){
		*valid = false;
		return move(fight);
	}

	Piece204156384 *winner;
	int res = fight->getWinner();


	if(res == 0){
		piece->kill();
		opponent->kill();
		winner = NULL;
	}
	else if(res == piece->getPlayer()->toInt()){
		opponent->kill();
		winner = piece;
	}
	else{
		piece->kill();
		winner = opponent;
	}

	board.setBoardContent(winner,pos);
	*valid = true;
	return move(fight);
}


void Game::makeTurn(std::unique_ptr<Move> move, std::unique_ptr<JokerChange> jokerChange){

	if(!this->players[getOpponent(this->turn)]->canMove(board)){
		this->players[getOpponent(this->turn)]->setStatus(Log::CantMove);
		return;
	}

	if(move->getFrom().getX() == -1){
		players[this->turn]->setStatus(Log::IllegalFormat);
		return;
	}

	Piece204156384 *piece = board.getBoardContent(move->getFrom());

	if(!piece || piece->getPlayer()->getType() != players[turn]->getType() || !piece->canMove()){
		players[this->turn]->setStatus(Log::IllegalFormat);
		return;
	}


	if(!isOneStepMove(move->getTo(), piece)){
		players[this->turn]->setStatus(Log::IllegalFormat);
		return;
	}

	bool valid = false;
	std::unique_ptr<MyFightInfo> fight = setPiecePosition(move->getTo(),piece, &valid);
	if(!valid){
		players[this->turn]->setStatus(Log::IllegalFormat);
		return;
	}
	if(fight != nullptr){
		players[turn]->getImpAlgorithm()->notifyFightResult(*fight.get());
		num_turns_without_a_fight=0;
	}
	else{
		num_turns_without_a_fight++;
	}

	if(!this->players[getOpponent(this->turn)]->canMove(board)){
		this->players[getOpponent(this->turn)]->setStatus(Log::CantMove);
	}

	else if(!this->players[getOpponent(this->turn)]->hasFlags()){
		this->players[getOpponent(this->turn)]->setStatus(Log::CapturedFlag);
	}

	if(jokerChange != nullptr){
		PieceType changeTo = PieceType::NoneType;
		Piece204156384 *joker = board.getBoardContent(jokerChange->getJokerChangePosition());
		if(joker == NULL || joker->getPlayer()->getType() != players[turn]->getType() || joker->getType() != PieceType::JokerType){
			this->players[(this->turn)]->setStatus(Log::IllegalFormat);
			return;
		}
		else{
			Joker* isJoker = dynamic_cast<Joker*>(joker);
			changeTo = Piece204156384::charToPieceType(jokerChange->getJokerNewRep());
			if(!isJoker->changeType(changeTo)){
				this->players[(this->turn)]->setStatus(Log::IllegalFormat);
				return;
			}
		}

		if(changeTo == PieceType::BombType && this->players[(this->turn)]->getStatus() == Log::CantMove){
			this->players[(this->turn)]->setStatus(Log::CantMove);
			return;
		}
	}

	this->players[(this->turn)]->setStatus(Log::Success);

	players[getOpponent(turn)]->getImpAlgorithm()->notifyOnOpponentMove(*move.get());
	if(fight != nullptr){
		players[getOpponent(turn)]->getImpAlgorithm()->notifyFightResult(*fight.get());
	}

}



bool Game::isOneStepMove(int tarRow, int tarCol, Piece204156384 *piece){
	if(!MyBoard::isInBoard(tarRow,tarCol))
		return false;
	int rowDiff = abs(piece->getPos()->getY() - tarRow);
	int colDiff = abs(piece->getPos()->getX() - tarCol);
	if((rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1))
		return true;
	return false;
}

bool Game::isOneStepMove(const Point& p, Piece204156384 *piece){
	return isOneStepMove(p.getY(),p.getX(),piece);
}



