
#include "MyFightInfo.h"
#include "Player.h"
#include "Piece.h"
#include <iostream>


const Point& MyFightInfo::getPosition()const{
	return point;
}


char MyFightInfo::getPiece(int player) const{
	return (_playerPiece.getPlayer()->toInt() == player ? Piece204156384::pieceTypeToChar(_playerPiece.getDynamicType()) : Piece204156384::pieceTypeToChar(_opponent.getDynamicType()));
}

int MyFightInfo::getWinner() const{

	bool res1 = _playerPiece.isWinning(_opponent.getDynamicType());
	bool res2 = _opponent.isWinning(_playerPiece.getDynamicType());

	if(res1 && !res2){
		return _playerPiece.getPlayer()->toInt();
	}
	else if(res2 && !res1){
		return _opponent.getPlayer()->toInt();
	}
	else{
		return 0;
	}

}

bool MyFightInfo::IsValid(){
	return _opponent.getPlayer() != _playerPiece.getPlayer();
}
