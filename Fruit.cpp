#include "Fruit.h"



Fruit::Fruit(const int x, const int y, const int adder, const int time_on_screen)
	: Fruit(Point(x, y), adder, time_on_screen)
{
	this->set_time_on_screen(time_on_screen);
}


Fruit::Fruit(const Point& init, const int adder,  const int time_on_screen)
	: Point(init), _point_add(adder), _time_on_screen(time_on_screen)
{
	this->_fruit_sign = static_cast<char>(48 + adder);
	this->set_time_on_screen(time_on_screen);
}

int Fruit::get_PointAdd() const
{
	return this->_point_add;
}

char Fruit::get_Fruit_Sign() const
{
	return this->_fruit_sign;
}


int Fruit::get_Fruit_time() const
{
	return this->_time_on_screen;
}

Direction Fruit::get_fruit_dir() const
{
	return this->_fruit_dir;
}

bool Fruit::get_is_fruit_on_screen() const
{
	return this->is_on_screen;
}

void Fruit::set_is_on_screen(bool on_screen)
{
	this->is_on_screen = on_screen;
}

void Fruit::set_PointAdd(int add_score)
{
	this->_point_add = add_score;
}

void Fruit::set_Fruit_Sign(char fruit_char)
{
	this->_fruit_sign = fruit_char;
}

void Fruit::set_fruit_direction(const Direction dir)
{
	this->_fruit_dir = dir;
}

/*Set Fruit time on Screen*/
void Fruit::set_time_on_screen(int time)
{
	if (time <= 0)
		this->is_on_screen = false;
	else
	{
		this->_time_on_screen = time;
		this->is_on_screen = true;
	}
}

/*Reduce Fruit time on Screen*/
void Fruit::reduce_time_on_screen()
{
	this->_time_on_screen--;
}

