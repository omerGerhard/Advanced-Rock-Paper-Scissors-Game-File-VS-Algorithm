//author: Omer Gerhard & Itay Fiszman
//Move implementation

#ifndef MYMOVE_H_
#define MYMOVE_H_

#include "Move.h"
#include "MyPoint.h"

class MyMove : public Move{

private:
	MyPoint _from;
	MyPoint _to;

public:

	MyMove(MyPoint from, MyPoint to) : _from(from), _to(to){}

	const Point& getFrom() const;

	const Point& getTo() const;
};


#endif /* MYMOVE_H_ */
