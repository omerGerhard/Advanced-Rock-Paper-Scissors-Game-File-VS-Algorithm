//author: Omer Gerhard & Itay Fiszman
//Point implementation

#ifndef BOARDPOINT_H_
#define BOARDPOINT_H_

#include "Point.h"


class MyPoint: public Point{
	int x;
	int y;


public:

	~MyPoint(){}

	MyPoint(int x, int y) : x(x) , y(y){};

	int getX()const override{
		return x;
	}

	int getY()const override{
		return y;
	}

/*	void setPos(int x, int y){
		this->x = x;
		this->y = y;
	}*/

};


#endif /* BOARDPOINT_H_ */
