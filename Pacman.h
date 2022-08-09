#pragma once
#include "Point.h"

class Pacman: public Point
{
	/*Data Members: */
	const Point _pacman_init;
public:
	Pacman();
	Pacman(const int x,const int y);
	Pacman(const Point& init);
	~Pacman(); /*D'ctor*/
	void restart_Pacman();/*Put Pacman in init Position */
};

