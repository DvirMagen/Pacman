#pragma once
#include <fstream>

enum Signs : char { PACMAN_SIGN = 2, BREADCRUMBS_SIGN = -7, WALLS_SIGN = -79, GHOST_SIGN = 5,
					SPACE_SIGN = ' ', TABOO_AREA = 'x', FREE_AREA = '^'};
enum class Direction { RIGHT, LEFT, UP, DOWN, STAY };
enum My_Ascii_Table : size_t { ESC_ASCII_NUM = 27, ENTER_ASCII_NUM = 13 };
enum Game_Logic : int { PACMAN_SPEED = 100 };

class Point
{
	/*Point - Data Members*/
protected:
	int _x;
	int _y;
public:
	Point(); /*C'tor*/
	Point(const int x, const int y);/*Full C'tor*/
	Point(const Point& point);/*Copy C'tor*/
	/*Getters Setters*/
	int get_x() const;
	int get_y() const;
	Point get_Point() const;
	void set_x(const int x);
	void set_y(const int y);
	void set_point(const int x, const int y);
	void set_point(const Point& tmp_point);
	/*Point Operators Definition */
	Point& operator=(const Point& p);
	Point& operator+=(const Point& p);
	Point operator+(const Point& p) const;
	bool operator==(const Point& rp) const;
	bool operator!=(const Point& rp) const;
	bool operator<(const Point& rp) const;
	friend std::ostream& operator<<(std::ostream& out, const Point& point);
	friend std::istream& operator>>(std::istream& in, Point& point);

	/*Update Point (x, y) by Direction key*/
	static Point convert_direction_to_point(const Direction& dir);
};

