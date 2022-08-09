#pragma once
#include "Point.h"
#include <vector>
#include <set>
#include <queue>
#include <map>

enum class Ghost_Intelligence : int { BEST = 0, GOOD = 1, NOVICE = 2 };

class Ghost: public Point
{
	/*Data Members: */
	const Point _ghost_init;
	size_t _count_move = 0;
	size_t _novice_count = 0;
	Direction _ghost_dir = Direction::STAY;
	Ghost_Intelligence _intelligence = Ghost_Intelligence::BEST; /*Not Relevant right now..*/
public:
	Ghost();
	Ghost(const int x, const int y, Ghost_Intelligence intel = Ghost_Intelligence::BEST);
	Ghost(const Point& init, Ghost_Intelligence intel = Ghost_Intelligence::BEST);
	~Ghost();
	void restart_Ghost();
	Direction get_direction() const;
	size_t get_count_move() const;
	void increase_move();
	size_t get_novice_count()const;
	void set_novice_count(size_t _novice_count);
	Ghost_Intelligence get_intelligence() const;
	void set_direction(const Direction dir);
	Direction BFS_next_move(const std::set<Point> valid_points, const Point* target)const;
};

