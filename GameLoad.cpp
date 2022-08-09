#include "GameLoad.h"
#include <sstream>
#include <string>

void GameLoad::read_steps_file(std::string file_name)
{
	std::string line;
	std::string sign_data;
	//std::string timer_data;
	std::string dir_data;
	//std::string index_data;
	std::string fruit_point_data;
	std::string fruit_appearance_data;
	std::ifstream load_file;
	Direction dir = Direction::STAY;
	int timer;
	int index;
	char fruit_sign;
	int row = 0;
	bool valid_file = true;
	load_file.open(file_name, std::ios::out);
	if (!load_file)
	{
		return;
	}
	while (std::getline(load_file, line))
	{
		std::stringstream data(line);
		data >> sign_data;
		data >> timer;
		data >> dir_data;
		setDirection(dir_data[0], dir);
		if (sign_data == "@")
		{
			this->append_pacman_move(dir, timer);
		}
		if(sign_data == "&")
		{
			data >> index;
			this->append_ghost_move(dir, timer, index);
		}
		if (sign_data == "F")
		{
			data >> fruit_sign;
			data >> fruit_point_data;
			Point p = { 0, 0 };
			setPoint(fruit_point_data, p);
			data >> sign_data;
			data >> fruit_appearance_data;
			bool on_screen = true;
			if (fruit_appearance_data == "Disappear")
				on_screen = false;
			this->append_fruit_move(dir, timer,on_screen, fruit_sign, p);
		}
	}
}
void GameLoad::setDirection(char dir_sign, Direction& dir)
{
	switch (dir_sign)
	{
	case 'R':
	{
		dir = Direction::RIGHT;
		break;
	}
	case 'L':
	{
		dir = Direction::LEFT;
		break;
	}
	case 'U':
	{
		dir = Direction::UP;
		break;
	}
	case 'D':
	{
		dir = Direction::DOWN;
		break;
	}
	default:
	{
		dir = Direction::STAY;
		break;
	}
	}
}

void GameLoad::setPoint(std::string str_point, Point& p)
{
	char x_t, x_i, y_t, y_i;
	int _x_t, _x_i, _y_t, _y_i;
	int x = 0, y = 0;
	if(str_point[2] == ',')
	{
		x_i = str_point[1];
		_x_i = x_i - '0';
		x = _x_i;
	}
	if (str_point[3] == ',')
	{
		x_t = str_point[1];
		x_i = str_point[2];
		_x_t = x_t - '0';
		_x_i = x_i - '0';
		_x_t = _x_t * 10;
		x = _x_t + _x_i;
	}
	if (str_point[4] == ')')
	{
		y_i = str_point[3];
		_y_i = y_i - '0';
		y = _y_i;
	}
	if (str_point[5] == ')' && str_point[3] == ',' && str_point.size() > 5)
	{
		y_i = str_point[4];
		_y_i = y_i - '0';
		y = _y_i;
	}
	if (str_point[5] == ')' && str_point[2] == ',' && str_point.size() > 5)
	{
		y_t = str_point[3];
		y_i = str_point[4];
		_y_t = y_t - '0';
		_y_i = y_i - '0';
		_y_t = _y_t * 10;
		y = _y_t + _y_i;
	}
	if (str_point[6] == ')'  && str_point.size() > 6)
	{
		y_t = str_point[4];
		y_i = str_point[5];
		_y_t = y_t - '0';
		_y_i = y_i - '0';
		_y_t = _y_t * 10;
		y = _y_t + _y_i;
	}
	p.set_point(x, y);
}
