


#include "AutoPlayerAlgorithm.h"
#include "MyPoint.h"
#include "MyPiecePosition.h"
#include "Piece.h"
#include "MyMove.h"
#include "MyJokerChange.h"
#include "Joker.h"
#include <algorithm>
#include <math.h>
#include <memory>
#include <vector>
#include <time.h>

#define SPHERE 4



vector<char> AutoPlayerAlgorithm::jokerOptions = {'R','B','S','P'};

AutoPlayerAlgorithm::AutoPlayerAlgorithm(int player){
	this->player = player;
	srand(time(NULL));

}

AutoPlayerAlgorithm::~AutoPlayerAlgorithm(){

}



void AutoPlayerAlgorithm::getInitialPositions(int player,
											std::vector<std::unique_ptr<PiecePosition>>& vectorToFill){

	vector<char> v = {'R','R','P','P','P','P','P','S','B','B','J','J'};
	std::random_shuffle(v.begin(),v.end());

	int row = rand() % ROWS; //random int between 0 to 9
	int col = rand() % COLS; //random int between 0 to 9
	pieces[row][col] = MyPiece(player,'F',false);

	double prob = 1;

	applyArea(row,col, ROWS, [&](int d, int y, int x){

		if(y == row && x == col)
			return;

		prob = pow(2,-d);


		if(!MyBoard::isInBoard(y+1,x+1) || v.size() == 0){
			return;
		}
		int p = (int)(1/prob);
		if((rand() % p) == 0){
			char type = v.back();
			v.pop_back();
			char joker = type;
			if(type=='J'){
				int i = rand() % (int)jokerOptions.size();
				joker = jokerOptions[i];
			}
			pieces[y][x] = MyPiece(player,type,false,joker);
		}
	});

	for(int i=0; i<ROWS;i++){
		for(int j=0;j<COLS;j++){
			if(pieces[i][j].valid){
				vectorToFill.push_back(make_unique<MyPiecePosition>(MyPoint(j+1,i+1),pieces[i][j].type,pieces[i][j].dynamicType));
			}
		}
	}
}

void AutoPlayerAlgorithm::print(){

	std::cout << endl;
	std::cout << endl;

	for(int row=0; row<ROWS; row++){
		std::cout << row << "|";
		for(int col=0; col<COLS; col++){

			if(pieces[row][col].valid){
				if(pieces[row][col].player == player)
					std::cout << pieces[row][col].type;
				else
					std::cout << (char)('a'+(pieces[row][col].type - 'A'));
			}
			else{
				std::cout << " ";
			}
		}
		std::cout << endl;
	}
}

void AutoPlayerAlgorithm::printFlagScore(){

	std::cout << endl;
	std::cout << endl;

	for(int row=0; row<ROWS; row++){
		std::cout << row << "| ";
		for(int col=0; col<COLS; col++){
			std::cout << flagScore[row][col] << " ";
		}
		std::cout << endl;
	}
}




void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board& b, const std::vector<std::unique_ptr<FightInfo>>& fights){

	for(int i=0;i<ROWS;i++){
		for(int j=0; j<COLS;j++){
			int boardPlayer = b.getPlayer(MyPoint(j+1,i+1));
			if(boardPlayer && boardPlayer != this->player){
				pieces[i][j] = MyPiece(boardPlayer, 'X',true);
			}
		}
	}

	for(int i=0; i<(int)fights.size(); i++){
		updateFightData(*fights[i].get());
	}

	updateFlagScore();

}


void AutoPlayerAlgorithm::updateFlagScore(){
	double sum = 0;
	for (int r=0; r<ROWS; r++){
		for(int c=0; c<COLS;c++){
			flagScore[r][c] = getFlagScore(r,c);
			sum += flagScore[r][c];
		}
	}

	if(sum){
		//normalize
		for (int r=0; r<ROWS; r++){
			for(int c=0; c<COLS;c++){
				flagScore[r][c] /= sum;
			}
		}
	}


}


void AutoPlayerAlgorithm::implementMove(const Move& move){
	int tarRow = move.getTo().getY()-1 , tarCol = move.getTo().getX()-1;
	int srcRow = move.getFrom().getY()-1 , srcCol = move.getFrom().getX()-1;
	MyPiece temp = pieces[srcRow][srcCol];
	pieces[srcRow][srcCol] = MyPiece();
	if(!pieces[tarRow][tarCol].valid || temp.player == player){
		pieces[tarRow][tarCol] = temp;
		pieces[tarRow][tarCol].potentialFlag = false;
	}

	updateFlagScore();
}


void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move& move){
	implementMove(move);
}


void AutoPlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo){
	updateFightData(fightInfo);
	updateFlagScore();
}


void AutoPlayerAlgorithm::updateFightData(const FightInfo & fight){
	int winnerPlayer = fight.getWinner();
	int row = fight.getPosition().getY()-1 , col = fight.getPosition().getX()-1 ;
	if(winnerPlayer == 0){
		pieces[row][col] = MyPiece();
		return;
	}
	char winnerPiece = fight.getPiece(winnerPlayer);
	if(winnerPlayer != player)
		pieces[row][col] = MyPiece(winnerPlayer,winnerPiece,false,winnerPiece);
}

/// zero base
double AutoPlayerAlgorithm::getFlagScore(int r, int c){

	double score = 0;

	if(pieces[r][c].valid && pieces[r][c].player != player && pieces[r][c].potentialFlag){

		applyArea(r,c,min(ROWS,SPHERE),[&](int d, int y, int x) {

			if(MyBoard::isInBoard(y+1,x+1) && (pieces[y][x].valid && pieces[y][x].player != player)){
				score+=pow(2,-d);
			}
		});

		apply(r,c,1,[&] (int y, int x) {
			if(!MyBoard::isInBoard(y+1,x+1)){
				score+=0.5;
			}
		});
	}


	return score;
}

void AutoPlayerAlgorithm::applyArea(int r, int c, int d, std::function<void(int,int,int)> func){

	for(int dist=0; dist <= d;dist++){
		apply(r,c,dist,[=] (int y, int x) {
			func(dist,y,x);
		});

	}
}

void AutoPlayerAlgorithm::apply(int r, int c, int d, std::function<void(int,int)> func){


	if(d==0){
		func(r,c);
		return;
	}

	int r1,c1;

	//right
	r1 = r -d, c1 = c-d;
	for(int d1=0;d1<2*d;d1++){
		func(r1, c1+d1);
	}

	//down
	r1 = r -d, c1 = c+d;
	for(int d1=0;d1<2*d;d1++){
		func(r1+d1, c1);
	}

	//left
	r1 = r +d, c1 = c+d;
	for(int d1=0;d1<2*d;d1++){
		func(r1, c1-d1);
	}

	//up
	r1 = r + d, c1 = c-d;
	for(int d1=0;d1<2*d;d1++){
		func(r1-d1, c1);
	}
}


unique_ptr<Piece204156384> AutoPlayerAlgorithm::myPieceToPiece(MyPiece &myPiece){
	Piece204156384 *piece = Piece204156384::pieceFactory(NULL, Piece204156384::charToPieceType(myPiece.type));
	if(myPiece.type == 'J'){
		Joker *joker = dynamic_cast<Joker *>(piece);
		joker->changeType(Piece204156384::charToPieceType(myPiece.dynamicType));
		piece = joker;
	}
	return unique_ptr<Piece204156384>(std::move(piece));
}


unique_ptr<Move> AutoPlayerAlgorithm::getMove(){

	MyMove bestMove (MyPoint(-1,-1),MyPoint(-1,-1));
	double bestScore = -1;

	MyPoint myFlag = findPiece(PieceType::FlagType,player)[0];

	double flagDefenceScore = getDefenseScore(myFlag);

	for(int row=0; row<ROWS; row++){
		for(int col=0; col<COLS; col++){
			if(pieces[row][col].valid && pieces[row][col].player == player){
				if(!myPieceToPiece(pieces[row][col])->canMove()){
					continue;
				}

				apply(row, col, 1, [&](int y, int x){
					if(!MyBoard::isInBoard(y+1,x+1)){
						return;
					}
					if(y !=row && x != col)
						return;

					if(pieces[y][x].player == player){
						return;
					}

					double tempScore = 0;

					MyPoint currentPoint = MyPoint(x+1,y+1);

					double distBefore = calcDistance(currentPoint, myFlag);

					applyArea(y,x,2,[&](int d, int y1, int x1){
						if(MyBoard::isInBoard(y1+1,x1+1)){

							double myScore;

							double distAfter = calcDistance(MyPoint(x1+1,y1+1), myFlag);
							double tempDefenseScore = (flagDefenceScore - pow(2,-distBefore) + pow(2,-distAfter))/6;

							if(pieces[y1][x1].valid){
								if(pieces[y1][x1].player == player){
									return;
								}

								if(pieces[y1][x1].potentialFlag){
									if(flagScore[y1][x1] >= 0.4){
										myScore = 1;
									}
									else{
										myScore = tempDefenseScore*(1+flagScore[y1][x1]);
									}
								}
								else{
									if(myPieceToPiece(pieces[row][col])->isWinning(Piece204156384::charToPieceType(pieces[y1][x1].type))){
										myScore = tempDefenseScore*1.5;
									}
									else{
										myScore = 0;
									}
								}
							}
							else{
								myScore = tempDefenseScore*0.5;
							}

							tempScore = max(tempScore, myScore/(d+1));
						}
					});

					if(tempScore > bestScore){
						bestScore = tempScore;
						bestMove = MyMove(MyPoint(col+1,row+1),MyPoint(currentPoint.getX(),currentPoint.getY()));
					}
				});
			}

		}
	}

	implementMove(bestMove);
	return make_unique<MyMove>(bestMove);
}


vector<MyPoint> AutoPlayerAlgorithm::findPiece(PieceType type, int _player){
	vector<MyPoint> tools;
	for(int row=0; row<ROWS; row++){
		for(int col=0; col<COLS; col++){
			if(pieces[row][col].valid && pieces[row][col].player == _player && pieces[row][col].type == Piece204156384::pieceTypeToChar(type)){
				tools.push_back(MyPoint(col+1,row+1));
			}
		}
	}
	return tools;
}


double AutoPlayerAlgorithm::getDefenseScore(MyPoint &flag){
	double defense = 0;
	int myFlagCol = flag.getX()-1, myFlagRow = flag.getY()-1;
	applyArea(myFlagRow, myFlagCol,2,[&](int d, int y, int x){

		if(!MyBoard::isInBoard(y+1,x+1) || !pieces[y][x].valid || (y == myFlagRow && x == myFlagCol) )
			return;

		if(pieces[y][x].player == player)
			defense += pow(2,-d);

	});

	return defense;
}


unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange(){

	MyPoint bestPos= MyPoint(-1,-1);
	char bestChange = 'R';
	double bestScore = 0;

	vector<MyPoint> myJokers = findPiece(PieceType::JokerType, player);

	if(myJokers.size() == 0)
		return nullptr;

	//iterate the jokers, and find the best joker change, if there is one
	for(auto &point : myJokers){
		int row = point.getY()-1, col = point.getX()-1;
		apply(row,col,1,[&](int y, int x){
			if(row != y && col != x)
				return;

			if(MyBoard::isInBoard(y+1,x+1)){
				if(pieces[y][x].valid && pieces[y][x].player != player){

					int tempScore = 0;
					char tempChange = 'R';

					if(pieces[y][x].potentialFlag){
						//if the opponent is a potential flag and the current joker rep cant move
						if(!myPieceToPiece(pieces[row][col])->canMove() && flagScore[y][x] >= 0.3){
							tempScore = flagScore[y][x];
							tempChange = 'R';
						}
					}
					else if(Piece204156384::charToPieceType(pieces[y][x].type) != PieceType::NoneType){
						//check if we can beat the opponent
						if(!myPieceToPiece(pieces[row][col])->isWinning(Piece204156384::charToPieceType(pieces[y][x].type))
						|| myPieceToPiece(pieces[y][x])->isWinning(Piece204156384::charToPieceType(pieces[row][col].type))){
							for(size_t i=0; i<jokerOptions.size(); i++){
								if(jokerOptions[i] == pieces[row][col].type){
									continue;
								}
								//if we can kill the opponent, without him killing us
								if(unique_ptr<Piece204156384>(Piece204156384::pieceFactory(NULL,Piece204156384::charToPieceType(jokerOptions[i])))->isWinning(Piece204156384::charToPieceType(pieces[y][x].type))
										&& !myPieceToPiece(pieces[y][x])->isWinning(Piece204156384::charToPieceType(jokerOptions[i]))){
									tempChange = jokerOptions[i];
									tempScore = 0.5;
								}
							}
						}
					}

					if(tempScore > bestScore){
						bestScore = tempScore;
						bestChange = tempChange;
						bestPos = point;
					}
				}
			}
		});

	}

	if(bestScore > 0){
		return make_unique<MyJokerChange>(bestPos, bestChange);
	}
	return nullptr;

}

int AutoPlayerAlgorithm::calcDistance(MyPoint p1, MyPoint p2){
	return max(abs(p1.getX()-p2.getX()),abs(p1.getY()-p2.getY()));
}
