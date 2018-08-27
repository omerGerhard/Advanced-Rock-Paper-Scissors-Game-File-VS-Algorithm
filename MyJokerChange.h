//author: Omer Gerhard & Itay Fiszman
//JokerChange implementation

#ifndef MYJOKERCHANGE_H_
#define MYJOKERCHANGE_H_

#include "JokerChange.h"
#include "MyPoint.h"

class MyJokerChange : public JokerChange{
private:
	MyPoint _jokerPos;
	char _jokerRep;

public:
	MyJokerChange(MyPoint pos, char newRep) : _jokerPos(pos), _jokerRep(newRep){}

	const Point& getJokerChangePosition()const ;

	char getJokerNewRep()const ; // R, P, S or B (but NOT J and NOT F)
};


#endif /* MYJOKERCHANGE_H_ */
