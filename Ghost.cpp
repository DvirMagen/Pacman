#include "Ghost.h"

#include "io_utills.h"

Ghost::Ghost():
	Ghost(0,0)
{
}

Ghost::Ghost(const int x, const int y, const Ghost_Intelligence intel):
	Ghost(Point(x,y), intel)
{
}

Ghost::Ghost(const Point& init, const Ghost_Intelligence intel) :
	Point(init), _ghost_init(init), _intelligence(intel)
{
}

Ghost::~Ghost()
{
}

void Ghost::restart_Ghost()
{
	this->set_point(this->_ghost_init.get_x(), this->_ghost_init.get_y());
	this->_ghost_dir = Direction::STAY;
}

Direction Ghost::get_direction() const
{
	return this->_ghost_dir;
}

size_t Ghost::get_count_move() const
{
	return this->_count_move;
}

void Ghost::increase_move()
{
	this->_count_move++;
}

size_t Ghost::get_novice_count() const
{
	return this->_novice_count;
}

void Ghost::set_novice_count(size_t _novice_count)
{
	this->_novice_count = _novice_count;
}

Ghost_Intelligence Ghost::get_intelligence() const
{
	return this->_intelligence;
}

void Ghost::set_direction(const Direction dir)
{
	this->_ghost_dir = dir;
}
/*Chasing Algorithm*/
Direction Ghost::BFS_next_move(const std::set<Point> valid_points, const Point* target)const
{
	// queue (FIFO) for run on all valid positions, for travel
	std::queue<Point> q;
	q.push(*this); /*set ghost start position*/

	std::set<Point> points_visited; /*data structure for all points we search*/
	points_visited.insert(*this); /*push current point*/

	std::map<Point, Point> prev; /*Data structure for backtrace*/
	while (!q.empty()) /*while we have points to move on*/
	{
		Point node = q.front(); /*give me current position*/
		q.pop();
		if (node == *target) /* if pacman was found */
		{
			Point tmp = prev[node];
			//Color crand = (Color)(rand() % 16);
			while (tmp != *this) 
			{
				/*go to previous positions from the target, back to start ghost position*/
				//gotoxy_print(tmp, '%', crand);
				node = tmp;
				tmp = prev[node];
			}
			
			//gotoxy_print(node, '%', (Color)(rand() % 16));
			if (node.get_x() != this->get_x())
			{
				if (node.get_x() < this->get_x())
				{
					return Direction::UP;
				}
				else if (node.get_x() > this->get_x())
				{
					return Direction::DOWN;
				}
			}
			else if (node.get_y() != this->get_y())
			{
				if (node.get_y() < this->get_y())
				{
					return Direction::LEFT;
				}
				else if (node.get_y() > this->get_y())
				{
					return Direction::RIGHT;
				}
			}
			return Direction::STAY;
		}
		/*All the possible positions from node*/
		std::vector<Point> neighbours;
		neighbours.push_back(node + convert_direction_to_point(Direction::UP));
		neighbours.push_back(node + convert_direction_to_point(Direction::DOWN));
		neighbours.push_back(node + convert_direction_to_point(Direction::LEFT));
		neighbours.push_back(node + convert_direction_to_point(Direction::RIGHT));
		/*For each possible move...*/
		for (Point neighbour : neighbours)
		{
			/*check if is a valid point that i never been there*/
			if(valid_points.count(neighbour) > 0 && points_visited.count(neighbour) == 0)
			{
				q.push(neighbour); /*push it to queue of valid positions*/
				points_visited.insert(neighbour); /*push it to set of have been positions*/
				prev[neighbour] = node; /*tell that the next point came from node*/
			}
		}
	}
	return Direction::STAY;
}
