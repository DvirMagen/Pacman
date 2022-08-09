#pragma once
#include <ostream>
#include <vector>
#include "Point.h"

class GameDoc
{
	/*Nested structs for vectors to different Creatures*/
	typedef struct pacman_node
	{
		Direction dir;
		int timer;
		bool got = false;
	}Pacman_Doc;

	typedef struct ghost_node: Pacman_Doc
	{
		int index;
	}Ghost_Doc;
	
	typedef struct fruit_node: Pacman_Doc
	{
		int sign;
		int time_on_screen;
		Point p = {0,0};
	}Fruit_Doc;

	std::vector<Pacman_Doc> _pacman_doc = {};
	std::vector<Ghost_Doc> _ghost_doc = {};
	std::vector<Fruit_Doc> _fruit_doc = {};
public:
	/*Evrey step of any Creature on game board enter to vector according to his type*/
	void append_pacman_move(Direction dir, int timer);
	void append_ghost_move(Direction dir, int timer, int index);
	void append_fruit_move(Direction dir, int timer, int time_on_screen, int sign,Point p);

	// get pacman move put in to the value
	// return true if there is a pacman move in timer 'timer' else return false
	/*Evrey Creature on game board get his next step from vector according to his type*/
	bool get_pacman_move(const int timer, Direction& dir) const;
	bool get_ghost_move(const int timer, Direction& dir, int index) const;
	bool get_fruit_move(const int timer, Direction& dir, int& fruit_sign, Point& p, int& time_on_screen);
	void clear_vectors();
	/*Operators: */
	friend std::ostream& operator<<(std::ostream& out, const GameDoc& gd); /*Operator << into file according to my format*/
	friend void operator>>(std::istream& in, GameDoc& gd); /*Operator >> into gameDoc according to my format*/
	
};

