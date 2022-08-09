#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>


#include "Fruit.h"
#include "GameDoc.h"
#include "Ghost.h"
#include "Pacman.h"
#include "Point.h"
#include "GameDoc.h"

#define ROWS 25
#define COLS 80

enum class Mode { LOAD, SAVE, SILENCE, SIMPLE };
enum Signs_In_File : char { PACMAN_FILE_SIGN = '@', GHOST_FILE_SIGN = '$', NO_BREADCRUMBS_SIGN = '%', 
	WALLS_FILE_SIGN = '#',  LIVES_AND_POINTS_FILE_SIGN = '&' };



class GameBoard
{
	/*Data Members: */
	char _console[ROWS][COLS];
	bool _file_is_fine = true;
	int _count_food;
	int _add_point;
	Point _end_scan_board; /*The right bottom char in Game Board*/
	Pacman* _pacman;
	std::vector<Ghost*> _ghosts;
	Fruit* _fruit;
	std::vector<Point*> _valid_pos;
	Point _lives_pos;
	Point _score_pos;
	Mode _game_mode = Mode::SIMPLE;
	GameDoc _game_doc;
	

public:
	GameBoard(const std::string file_path, const Ghost_Intelligence ghost_intelligence);// Constructor
	~GameBoard();
	void setMode(Mode mode);
	Mode getMode() const;
	void create_fruit(int time, int point_add = 0, int time_on_screen = 0, Point p = { 0,0 });
	/*Print functions */
	void print();
	void print_score_and_lives(const int score,const int lives) const;
	void print_pacman() const;
	void print_ghost(const int ghost_number) const;
	void print_fruit() const;
	void print_food(const int x, const int y) const;
	/*
	 *	Reset the position of Pacman and ghosts
	 */
	void reset_and_print_board();
	/*Move creachers on board */
	void move_pacman(Direction& key_dir, int time);
	void move_ghost(const size_t ghost_number, int time);
	void set_ghost_dir(const size_t ghost_number, const Direction dir);
	void move_fruit(int time, const Direction load_dir=Direction::STAY);
	void remove_fruit();

	bool is_fruit_on_screen() const;
	/**
	Return false if game continue, else true if win
	 */
	const bool is_file_valid() const;
	bool is_win() const;
	bool is_lose() const;
	int get_Ghost_Size() const;
	int get_and_reset_add_point();
	void best_level_algo(Direction& dir, Point& new_p, int ghost_number);
	void novice_level_algo(Direction& dir, Point& new_p, int ghost_number);
	void record_to_file(const std::string level) const; /*Doc function */
	
private:
	bool read_console_from_file(std::string file_path);
	void taboo_template__in_console(const int row,const int col);
};

