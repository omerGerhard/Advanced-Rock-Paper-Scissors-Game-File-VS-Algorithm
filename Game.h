//author: Omer Gerhard & Itay Fiszman
//game manager, responsible for running the game flow, controlling the player's turn, writing to the console and output files


#ifndef GAME_H_
#define GAME_H_

#include "MyBoard.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>



class Player;
class PlayerAlgorithm ;
class Piece204156384;
class Point;
class Joker;
class MyFightInfo;
class JokerChange;
class Move;

enum class FightResult : int;
enum class Log : int;
enum class PieceType : int;

class Game
{
	int turn;
	int numOfTurns;
	int num_turns_without_a_fight;
	MyBoard board;
	Player *players[2];

	void changeTurn();

	int getPlayerIndex(const Player *player) const;

	int getOpponent(int) const;

	void positioning();

	bool isGameOver() const;

	void createOutputFile(const Player* winner);

	Player *calculateWinner() const;

	void printBoard(std::ofstream&);

	static bool isSuccess(Log);

	bool isOneStepMove(int x, int y, Piece204156384 *piece);

	bool isOneStepMove(const Point& p, Piece204156384 *piece);

public:


	Game(PlayerAlgorithm *algorithms[2]);
	~Game();

	void runGame();

	std::unique_ptr<MyFightInfo> setPiecePosition(Point const& pos, Piece204156384 *piece, bool* valid);

	void makeTurn(std::unique_ptr<Move> move, std::unique_ptr<JokerChange> jokerChange);

	void consoleErrorMessage(Player &player);

};













#endif /* GAME_H_ */
