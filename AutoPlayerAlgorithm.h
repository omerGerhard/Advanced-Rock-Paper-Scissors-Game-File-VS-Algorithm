//author: Omer Gerhard & Itay Fiszman
//algorithm for auto player



#ifndef AUTOPLAYERALGORITHM_H_
#define AUTOPLAYERALGORITHM_H_

#include "PlayerAlgorithm.h"

#include <string>
#include <fstream>
#include <memory>
#include <vector>

#include "Piece.h"
#include "MyBoard.h"

class Player;
class MyMove;
class MyJokerChange;
class MyPoint;


class AutoPlayerAlgorithm : public PlayerAlgorithm{


private:

	typedef struct MyPiece{

		bool valid;
		int player;
		char type;
		bool potentialFlag;
		char dynamicType;

		MyPiece()  : valid(false),player(0), type('\0'), potentialFlag(false), dynamicType('\0'){}
		MyPiece(int player, char type, bool potentialFlag, char dynamicType = '\0') : valid(true),player(player), type(type), potentialFlag(potentialFlag),dynamicType(dynamicType ? dynamicType : type) {}
		void setValid(bool v){valid = v;}
		void setDynamicType(char dynamic) {dynamicType = dynamic;}

	} MyPiece;

	MyPiece pieces[ROWS][COLS];
	double flagScore[ROWS][COLS];
	int player;
	static vector<char> jokerOptions;


	void updateFightData(const FightInfo & fight);

	//calculate the distance between two points of the board.
	//the meaning of distance in this context is the maximal
	//difference between the x coordinates of the two point or the y coordinates
	int calcDistance(MyPoint p1, MyPoint p2);

	//applied only an opponent cells, this function will return the calculated expected chance of this cell to contain a flag
	double getFlagScore(int r, int c);

	//calculate the level of defense on this player's flag
	double getDefenseScore(MyPoint &flag);

	//returning a vector contains the positions of all tools of type 'type' belongs to player '_player' in the current board
	vector<MyPoint> findPiece(PieceType type, int _player);

	unique_ptr<Piece204156384> myPieceToPiece(MyPiece &myPiece);

	void apply(int r, int c, int d, std::function<void(int,int)> func);

	void applyArea(int r, int c, int d, std::function<void(int,int,int)> func);

	void implementMove(const Move& move);

	//todo
	void print();
	void printFlagScore();


public:

	AutoPlayerAlgorithm(int player);

	~AutoPlayerAlgorithm();

	void getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill) override;

	void notifyOnInitialBoard(const Board& b, const std::vector<std::unique_ptr<FightInfo>>& fights) override;

	void notifyOnOpponentMove(const Move& move) override;

	void updateFlagScore(); //updating the chance of each cell in the board to be a flag (relevant only for opponent tools)

	unique_ptr<Move> getMove() override;

	unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested

	void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight

};



#endif /* FILEPLAYERALGORITHM_H_ */


