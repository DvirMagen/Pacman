#include "Pacman.h"

Pacman::Pacman():
	Point()
{
}

Pacman::Pacman(const int x, const int y):
	Point(x,y), _pacman_init(x,y)
{
}

Pacman::Pacman(const Point& init):
	Point(init), _pacman_init(init)
{
}

Pacman::~Pacman()
{
}

void Pacman::restart_Pacman()
{
	this->set_point(this->_pacman_init.get_x(),this->_pacman_init.get_y());
}
