
#include "FilePlayerAlgorithm.h"



#include "Game.h"

#include "Piece.h"
#include "Player.h"

#include "Joker.h"
#include "MyPoint.h"
#include "MyPiecePosition.h"

#include "MyMove.h"
#include "MyJokerChange.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>



FilePlayerAlgorithm::FilePlayerAlgorithm(int player) : moveFileStream(NULL){

	if(player == 1){
		positionFile = "player1.rps_board";
		moveFile = "player1.rps_moves";
	}
	else{
		positionFile = "player2.rps_board";
		moveFile = "player2.rps_moves";
	}

	getMoves(moves,jokerChanges);
	movesIter = moves.begin();
	jokerChangesIter = jokerChanges.begin();

}

void storeErrorLine(std::vector<std::unique_ptr<MyMove>>& movesToFill,
		std::vector<std::unique_ptr<MyJokerChange>>& jokerChangesToFill){

	MyPoint dummy = MyPoint(-1,-1);
	movesToFill.push_back(make_unique<MyMove>(dummy,dummy));
	jokerChangesToFill.push_back(make_unique<MyJokerChange>(dummy,0));

}

void FilePlayerAlgorithm::getMoves(std::vector<std::unique_ptr<MyMove>>& movesToFill,
		std::vector<std::unique_ptr<MyJokerChange>>& jokerChangesToFill){

	ifstream* file = getMoveFileStream();

	string line;

	unique_ptr<MyMove> move = nullptr;
	unique_ptr<MyJokerChange> jokerChange = nullptr;

	if(!file){
		storeErrorLine(movesToFill, jokerChangesToFill);
		return;
	}

	while(!file->eof()){
		getline(*file,line);
		while(line == ""){
			getline(*file,line);
			if(file->eof()){
				storeErrorLine(movesToFill, jokerChangesToFill);
				return;
			}
		}

		vector<string> tokens;
		lineToVector(tokens , line);

		if((int)tokens.size() != 8 && (int)tokens.size() != 4){
			storeErrorLine(movesToFill, jokerChangesToFill);
			return;
		}
		for(int i=0; i<4; i++){
			if(!Piece204156384::validIndex(tokens[i],tokens[i].length())){
				storeErrorLine(movesToFill, jokerChangesToFill);
				return;
			}

		}

		MyPoint from = MyPoint(stoi(tokens[0]), stoi(tokens[1]));
		MyPoint to  = MyPoint(stoi(tokens[2]), stoi(tokens[3]));


		if(!MyBoard::isInBoard(from) || !MyBoard::isInBoard(to)){
			storeErrorLine(movesToFill, jokerChangesToFill);
			return;
		}


		move = make_unique<MyMove>(from,to);

		//case of change in joker reprsentation
		if((int)tokens.size() == 8){
			if(tokens[4].length() != 2 || tokens[4][0] != 'J' || tokens[4][1] != ':' || tokens[7].length() != 1){
				storeErrorLine(movesToFill, jokerChangesToFill);
				return;
			}

			for(int i=5; i<7; i++){
				if(!Piece204156384::validIndex(tokens[i],tokens[i].length())){
					storeErrorLine(movesToFill, jokerChangesToFill);
					return;
				}
			}

			MyPoint pos = MyPoint(stoi(tokens[5]), stoi(tokens[6]));

			if(!MyBoard::isInBoard(pos)){
				storeErrorLine(movesToFill, jokerChangesToFill);
				return;
			}

			char changeChar = tokens[7][0];
			if(!Piece204156384::validatePieceChar(changeChar)){
				storeErrorLine(movesToFill, jokerChangesToFill);
				return;
			}


			jokerChange = make_unique<MyJokerChange>(pos, changeChar);

		}

		movesToFill.push_back(std::move(move));
		jokerChangesToFill.push_back(std::move(jokerChange));
	}

	storeErrorLine(movesToFill, jokerChangesToFill);
}



FilePlayerAlgorithm::~FilePlayerAlgorithm(){
	if(moveFileStream)
		moveFileStream->close();
	delete moveFileStream;
}

ifstream* FilePlayerAlgorithm::getMoveFileStream(){
	if(moveFileStream == NULL){
		moveFileStream = new ifstream(moveFile);
		if(!moveFileStream->is_open())
			return NULL;
	}
	return moveFileStream;
}


void FilePlayerAlgorithm::lineToVector(std::vector<std::string> &tokens, std::string line){
	istringstream iss(line);

	while(1){
		string sub;
		iss >> sub;
		if(sub == "")
			break;
		tokens.push_back(sub);
	}
}


void FilePlayerAlgorithm::getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill){

	(void)player;
	ifstream fin(positionFile);

	string line;
	Log log = Log::Success;
	if(fin.is_open()){
		while(!fin.eof()){

			getline(fin, line);
			if(line == "")
				continue;

			vector<string> tokens;
			lineToVector(tokens , line);

			if(tokens.size() != 3 && tokens.size() != 4){
				  log = Log::IllegalFormat;
				  break;
			}

			if(tokens[0].length() != 1){
				  log = Log::IllegalFormat;
				  break;
			}

			char pieceChar = tokens[0][0];

			int x = tryParseInt(tokens[1]),y = tryParseInt(tokens[2]);
			if(x == -1 || y == -1){
				  log = Log::IllegalFormat;
				  break;
			}

			char jokerChar = '#';
			if(pieceChar == 'J'){
				if(tokens.size() != 4 || tokens[3].length() != 1){
					  log = Log::IllegalFormat;
					  break;
				}
				jokerChar = tokens[3][0];
			}

			vectorToFill.push_back(std::make_unique<MyPiecePosition>(MyPoint(x,y),pieceChar, jokerChar));

		}

		if(fin)
		  fin.close();
	}

	else{
		log = Log::IllegalFormat;
	}

	if(log != Log::Success){
		vectorToFill.clear();
	}
}

void FilePlayerAlgorithm::notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights){
	(void) b;
	(void) fights;
	return;
}

void FilePlayerAlgorithm::notifyOnOpponentMove(const Move& move){
	(void)move;
	return;
}

void FilePlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo){
	(void)fightInfo;
	return;
}

unique_ptr<Move> FilePlayerAlgorithm::getMove(){
	unique_ptr<MyMove> move = std::move(*movesIter);
	movesIter++;
	return std::move(move);
}

unique_ptr<JokerChange> FilePlayerAlgorithm::getJokerChange(){
	unique_ptr<MyJokerChange> jokerChange = std::move(*jokerChangesIter);
	jokerChangesIter++;
	return std::move(jokerChange);
}


int FilePlayerAlgorithm::tryParseInt(const string &s){
	for(size_t i=0; i<s.length(); i++){
		if(s[i]>'9' || s[i] < '0'){
			return -1;
		}
	}
	return std::stoi(s);
}





