#include "Point.h"

#include <iostream>

/*Constructor*/
Point::Point() :_x(0), _y(0)
{
}

/*Copy Constructor*/
Point::Point(const int x, const int y) : _x(0), _y(0)
{
	this->set_point(x, y);
}
Point::Point(const Point& point): _x(0), _y(0)
{
	this->set_point(point._x,point._y);
}
/*Getters Setters*/
int Point::get_x() const
{
	return this->_x;
}

int Point::get_y() const
{
	return this->_y;
}

Point Point::get_Point() const
{
	Point pnt;
	pnt.set_point(this->get_x(), this->get_y());
	return pnt;
}

void Point::set_x(const int x)
{
	this->_x = x;
}

void Point::set_y(const int y)
{
	this->_y = y;
}

void Point::set_point(const int x, const int y)
{
	this->set_x(x);
	this->set_y(y);
}
void Point::set_point(const Point& tmp_point)
{
	this->set_point(tmp_point.get_x(), tmp_point.get_y());
}
/*Operator = for Point to Point*/
Point& Point::operator=(const Point& p)
{
	this->set_point(p._x, p._y);
	return *this;
}
/*Operator += for Point*/
Point& Point::operator+=(const Point& p)
{
	this->set_x(this->_x + p._x);
	this->set_y(this->_y + p._y);
	return *this;
}
/*Operator + for point*/
Point Point::operator+(const Point& p) const
{
	Point tmp = Point(this->_x + p._x, this->_y + p._y);
	return tmp;
}
/*Boolean operator == for points*/
bool Point::operator==(const Point& rp) const
{
	return this->_x == rp._x && this->_y == rp._y;
}

bool Point::operator!=(const Point& rp) const
{
	return !(this->operator==(rp));
}

bool Point::operator<(const Point& rp) const
{
	double my_tmp = this->_x + this->_y * 0.01;
	double his_tmp = rp._x + rp._y * 0.01;
	return my_tmp < his_tmp;

}


Point Point::convert_direction_to_point(const Direction& dir)
{
	Point p;
	switch (dir)
	{
	case Direction::STAY:
	{
		break;
	}
	case Direction::LEFT:
	{
		p.set_y(-1);
		break;
	}
	case Direction::UP:
	{
		p.set_x(-1);
		break;
	}
	case Direction::RIGHT:
	{
		p.set_y(1);
		break;
	}
	case Direction::DOWN:
	{
		p.set_x(1);
		break;
	}
	}
	return p;
}

std::ostream& operator<<(std::ostream& out,const Point& point)
{
	return out << '(' << point.get_x() << ',' << point.get_y() << ')';
}

std::istream& operator>>(std::istream& in, Point& point)
{
	char a, b, c;
	return in >> a >> point._x >> b >> point._y >> c;
}