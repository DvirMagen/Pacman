#pragma once
#include "Point.h"

/*The Fruit move on the screen in random path and random valid places*/
class Fruit: public Point
{ /*Fruit Data Members: */
	int _point_add;/*Random*/
	char _fruit_sign;  
	bool is_on_screen = false;
	int _time_on_screen; /*Random*/
	Direction _fruit_dir = Direction::STAY;
public:
	Fruit(const int x, const int y,const int adder, const int _time_on_screen);
	Fruit(const Point& init, const int adder, const int _time_on_screen);
	/*Getters Setters*/
	int get_PointAdd() const;
	char get_Fruit_Sign() const;
	int get_Fruit_time() const;
	Direction get_fruit_dir() const;
	bool get_is_fruit_on_screen() const;
	void set_is_on_screen(bool on_screen);
	void set_PointAdd(int add_score);
	void set_Fruit_Sign(char fruit_char);
	void set_fruit_direction(const Direction dir);
	void set_time_on_screen(int time);
	/*time--*/
	void reduce_time_on_screen();
	
};

