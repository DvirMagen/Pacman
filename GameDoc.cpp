#include "GameDoc.h"

void GameDoc::append_pacman_move(Direction dir, int timer)
{
	if (!this->_pacman_doc.empty() && this->_pacman_doc.back().dir == dir) return;
	
	Pacman_Doc pac_node = { dir, timer };
	this->_pacman_doc.push_back(pac_node);
}

void GameDoc::append_ghost_move(Direction dir, int timer, int index)
{
	//if (!this->_ghost_doc.empty())
	//{
	//	for (auto it = this->_ghost_doc.rbegin(); it != this->_ghost_doc.rend(); ++it)
	//	{
	//		if (it->dir == dir && it->index == index)
	//		{
	//			return;
	//		}
	//		else if (it->index == index)
	//		{
	//			break;
	//		}
	//	}
	//}
	Ghost_Doc ghost_node = {{dir, timer}, index };
	this->_ghost_doc.push_back(ghost_node);
}

void GameDoc::append_fruit_move(Direction dir, int timer, int time_on_screen, int sign, Point p)
{
	if (!this->_fruit_doc.empty() && this->_fruit_doc.back().dir == dir && this->_fruit_doc.back().time_on_screen == time_on_screen) return;
	
	Fruit_Doc fruit_node = {{dir, timer},sign , time_on_screen, p };
	this->_fruit_doc.push_back(fruit_node);
}

bool GameDoc::get_pacman_move(const int timer, Direction& dir)const
{
	for (auto pacman_node : this->_pacman_doc)
	{
		if(pacman_node.timer == timer)
		{
			dir = pacman_node.dir;
			return true;
		}
	}
	return false;
}

bool GameDoc::get_ghost_move(const int timer, Direction& dir, int index) const
{
	for (auto ghost_node : this->_ghost_doc)
	{
		if (ghost_node.timer == timer && index == ghost_node.index)
		{
			dir = ghost_node.dir;
			return true;
		}
	}
	return false;
}


bool GameDoc::get_fruit_move(const int timer, Direction& dir, int& fruit_sign, Point& p, int& time_on_screen)
{
	for (size_t i = 0; i < this->_fruit_doc.size(); ++i)
	{
		auto fruit_node = this->_fruit_doc[i];
		if (fruit_node.timer == timer && fruit_node.got == false)
		{
			dir = fruit_node.dir;
			fruit_sign = fruit_node.sign;
			p = fruit_node.p;
			time_on_screen = fruit_node.time_on_screen;
			this->_fruit_doc[i].got = true;
			return true;
		}
	}
	return false;
}

void GameDoc::clear_vectors()
{
	this->_pacman_doc.clear();
	this->_ghost_doc.clear();
	this->_fruit_doc.clear();
}

/*Operator << into file according to my format*/
std::ostream& operator<<(std::ostream& out, const GameDoc& gd)
{
	int timer = 0;
	size_t i = 0, j = 0, k = 0;
	
	while(i < gd._pacman_doc.size() || j < gd._ghost_doc.size() || k < gd._fruit_doc.size())
	{
		int choice = 0;
		Direction dir = Direction::STAY;
		char ch_dir = 'S';
		if (i < gd._pacman_doc.size() && timer > gd._pacman_doc[i].timer)
		{
			timer = gd._pacman_doc[i].timer;
			dir = gd._pacman_doc[i].dir;
			choice = 1;
		}
		if(j < gd._ghost_doc.size() && timer > gd._ghost_doc[j].timer)
		{
			timer = gd._ghost_doc[j].timer;
			dir = gd._ghost_doc[j].dir;
			choice = 2;
		}
		if(k < gd._fruit_doc.size() && timer > gd._fruit_doc[k].timer)
		{
			timer = gd._fruit_doc[k].timer;
			dir = gd._fruit_doc[k].dir;
			choice = 3;
		}

		switch (dir)
		{
			case Direction::STAY:
				ch_dir = 'S';
				break;
			case Direction::UP:
				ch_dir = 'U';
				break;
			case Direction::DOWN:
				ch_dir = 'D';
				break;
			case Direction::LEFT:
				ch_dir = 'L';
				break;
			case Direction::RIGHT:
				ch_dir = 'R';
				break;
		}
		
		
		if (choice == 1)
		{
			out <<'@' << ' ' << gd._pacman_doc[i].timer << ' ' << ch_dir << std::endl;
			++i;
		}
		else if (choice == 2)
		{
			out << '&' << ' ' << gd._ghost_doc[j].timer << ' ' << ch_dir << ' ' << gd._ghost_doc[j].index << std::endl;
			++j;
		}
		else if(choice == 3)
		{
			out<< 'F' << ' ' << gd._fruit_doc[k].timer << ' ' << ch_dir << ' ' << gd._fruit_doc[k].sign << ' '
					<< gd._fruit_doc[k].p << ' ' << gd._fruit_doc[k].time_on_screen << std::endl;
			++k;
		}

		++timer;
	}

	return out;
}

Direction getDirection(char dir_sign)
{
	switch (dir_sign)
	{
	case 'R':
	{
		return Direction::RIGHT;
		
	}
	case 'L':
	{
		return Direction::LEFT;
		
	}
	case 'U':
	{
		return Direction::UP;
		
	}
	case 'D':
	{
		return Direction::DOWN;
		break;
	}
	default:
	{
		return Direction::STAY;
		
	}
	}
}

/*Operator >> into gameDoc according to my format*/
void operator>>(std::istream& in, GameDoc& gd)
{
	while (!in.eof()) 
	{
		char type;
		int timer;
		char dir_char;
		in >> type >> timer >> dir_char;
		switch (type)
		{
			case '@':
			{
				gd.append_pacman_move(getDirection(dir_char), timer);
				break;
			}
			case '&':
			{
				int index;
				in >> index;
				gd.append_ghost_move(getDirection(dir_char), timer, index);
				break;
			}
			case 'F':
			{
				int sign;
				Point p;
				int time_appear;
				in >> sign >> p >> time_appear;
				gd.append_fruit_move(getDirection(dir_char), timer, time_appear , sign, p);
				break;
			}
		}
	}	
}
