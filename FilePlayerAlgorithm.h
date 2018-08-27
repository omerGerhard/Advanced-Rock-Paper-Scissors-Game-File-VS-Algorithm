//author: Omer Gerhard & Itay Fiszman
//algorithm for file player



#ifndef FILEPLAYERALGORITHM_H_
#define FILEPLAYERALGORITHM_H_

#include "PlayerAlgorithm.h"

#include <string>
#include <fstream>
#include <memory>
#include <vector>

class Player;
class MyMove;
class MyJokerChange;

class FilePlayerAlgorithm : public PlayerAlgorithm{

private:
	std::string positionFile;
	std::string moveFile;
	std::ifstream* moveFileStream;
	std::vector<std::unique_ptr<MyMove>> moves;
	std::vector<std::unique_ptr<MyJokerChange>> jokerChanges;
	std::vector<std::unique_ptr<MyMove>>::iterator movesIter;
	std::vector<std::unique_ptr<MyJokerChange>>::iterator jokerChangesIter;

	std::ifstream* getMoveFileStream();

	//parses the player's move file into two vectors - vector of moves and vector of joker changes
	//by reading line by line from the file and parses each line according to it's content.
	//if a line in the moves file contains only a move and not a joker change, this method will
	//add the move into the vector 'movesToFill' and will add nullptr to the vector 'jokerChangesToFill'
	void getMoves(std::vector<std::unique_ptr<MyMove>>& movesToFill,
			std::vector<std::unique_ptr<MyJokerChange>>& jokerChangesToFill);

	//checks if the string 's' representing an integer
	int tryParseInt(const std::string &s);

	//parses a line from a file to a vector of strings
	void lineToVector(std::vector<std::string> &tokens, std::string line);

public:

	FilePlayerAlgorithm(int player);

	~FilePlayerAlgorithm();

	void getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill) override;

	void notifyOnInitialBoard(const Board& b, const std::vector<std::unique_ptr<FightInfo>>& fights) override;

	void notifyOnOpponentMove(const Move& move) override;

	unique_ptr<Move> getMove() override;

	unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested

	void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight

};



#endif /* FILEPLAYERALGORITHM_H_ */
