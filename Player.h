//author: Omer Gerhard & Itay Fiszman
//Player class, representing a game player, responsible for the player's pieces and score


#ifndef PLAYER_H_
#define PLAYER_H_

#define ROCKS 2
#define PAPERS 5
#define SCISSORS 1
#define BOMBS 2
#define JOKERS 2
#define FLAGS 1

#define NUM_OF_PIECES ROCKS + PAPERS + SCISSORS + BOMBS + JOKERS + FLAGS


#include <fstream>
#include <vector>
#include <memory>

using namespace std;

class Piece204156384;
class PlayerAlgorithm;
class Game;
class MyBoard;
class FightInfo;
class PiecePosition;

enum class Log : int{
	Success,
	IllegalFormat,
	PieceOverflow,
	PieceCollision,
	OutOfRange,
	MissingFlags,
	CapturedFlag,
	CantMove
};

enum PlayerType {
	player1,
	player2,
	dummyPlayer
};


class Player{

private:
	bool lower;
	int score;
	Log status;
	PlayerType type;
	PlayerAlgorithm *algo;

	bool hasMoves(Piece204156384 const* piece, const MyBoard &board);

	bool checkCollisions(std::vector<std::unique_ptr<PiecePosition>>& positions);

public:
	Log getInitialPositions(Game &game, vector<unique_ptr<FightInfo>> &fights);

	Piece204156384 *pieces[NUM_OF_PIECES];

	Player(PlayerType _type, PlayerAlgorithm *algo);

	~Player();

	Log getStatus() const;

	void setStatus(Log log);

	PlayerAlgorithm* getImpAlgorithm(){
		return algo;
	}

	PlayerType getType() const;


	bool isLower() const;

	void addPoint();

	bool canMove(const MyBoard &board);

	bool hasFlags();

	string toString();

	int toInt();

};

#endif /* PLAYER_H_ */
