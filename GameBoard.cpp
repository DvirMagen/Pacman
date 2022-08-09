#include "GameBoard.h"

#include "io_utills.h"

GameBoard::GameBoard(const std::string file_path, const Ghost_Intelligence ghost_intelligence):
	 _console{}, _count_food(0),_add_point(0),_pacman(nullptr), _fruit(nullptr){
	_file_is_fine = read_console_from_file(file_path);/*Console Board Update*/
	if (!_file_is_fine)
	{
		return;
	}
	/*First scan on console to update signs, pacman position, score and lives position*/
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (this->_console[i][j] == Signs_In_File::LIVES_AND_POINTS_FILE_SIGN) //
			{
				this->taboo_template__in_console(i, j);/*make a taboo area, an area that any creature can't travel in*/
				this->_lives_pos = Point(i, j); /*Update lives and score position*/
				this->_score_pos = Point(i + 1, j);
			}
			else if (this->_console[i][j] == Signs::PACMAN_SIGN)
			{
				this->_pacman = new Pacman(i, j);
				this->_console[i][j] = Signs::SPACE_SIGN;
				this->_valid_pos.push_back(new Point(i, j));
			}
			else if (this->_console[i][j] == Signs::GHOST_SIGN)
			{
				this->_ghosts.push_back(new Ghost(i, j, ghost_intelligence));
				this->_console[i][j] = SPACE_SIGN;
			}
			else if (this->_console[i][j] == Signs::SPACE_SIGN)
			{
				this->_console[i][j] = Signs::BREADCRUMBS_SIGN;
				this->_count_food += 1;
				this->_valid_pos.push_back(new Point(i, j));
			}
			else if (_console[i][j] == FREE_AREA)
			{
				this->_console[i][j] = Signs::SPACE_SIGN;
				this->_valid_pos.push_back(new Point(i, j));
			}
			else if (this->_console[i][j] == Signs::WALLS_SIGN) //
			{
				this->_end_scan_board.set_point(i, j);
			}
		}
	}
}


/*Update console board by file, and return if file is valid*/
bool GameBoard::read_console_from_file(const std::string file_path)
{
	std::ifstream pacman_file;
	int row = 0;
	bool valid_file = true;
	pacman_file.open(file_path, std::ios::out);
	if (!pacman_file)
	{
		return false;
	}
	char ch;
	/*Put taboo signs in all board*/
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			this->_console[i][j] = TABOO_AREA;
		}
	}
	int count_pacman_signs = 0;
	int count_ghost_signs = 0;
	int lives_sign_count = 0;
	while (!pacman_file.eof())
	{
		for (int i = 0; i <= COLS; i++)
		{
			pacman_file.read(&ch, 1);
			if (ch == '\n' || pacman_file.eof())
			{
				break;
			}
			// convert file characters to game characters
			if (ch == Signs_In_File::WALLS_FILE_SIGN)
			{
				this->_console[row][i] = Signs::WALLS_SIGN;
			}
			else if (ch == Signs_In_File::PACMAN_FILE_SIGN)
			{
				this->_console[row][i] = Signs::PACMAN_SIGN;
				count_pacman_signs++;
			}
			else if (ch == Signs::SPACE_SIGN)
			{
				this->_console[row][i] = Signs::SPACE_SIGN;
			}
			else if (ch == Signs_In_File::GHOST_FILE_SIGN)
			{
				this->_console[row][i] = Signs::GHOST_SIGN;
				count_ghost_signs++;
			}
			else if (ch == Signs_In_File::NO_BREADCRUMBS_SIGN)
			{
				this->_console[row][i] = Signs::FREE_AREA;
			}
			else if(ch == Signs_In_File::LIVES_AND_POINTS_FILE_SIGN)
			{
				lives_sign_count++;
				this->_console[row][i] = Signs_In_File::LIVES_AND_POINTS_FILE_SIGN;
			}
			else
			{
				this->_console[row][i] = ch;
				valid_file = false;
			}
		}
		row++;
	}
	/*Check Board Validity */
	if(count_pacman_signs  != 1  || count_ghost_signs > 4 || lives_sign_count != 1)
	{
		valid_file = false;
	}
	pacman_file.close();
	return valid_file;
}

void GameBoard::taboo_template__in_console(const int row, const int col)
{
	for (int i = row; i < row + 2 && i < ROWS; i++)
	{
		for (int j = col; j < col + 20 && j < COLS; j++)
		{
			this->_console[i][j] = Signs::TABOO_AREA;
		}
	}
}

/*D'tor Gameboard*/
GameBoard::~GameBoard()
{
	delete this->_pacman;
	
	for(size_t i = 0 ; i< this->_ghosts.size(); i++)
	{
		if(this->_ghosts[i] != nullptr)
			delete this->_ghosts[i];
	}
	delete this->_fruit;
	for (size_t i = 0; i < this->_valid_pos.size(); i++)
	{
		if (this->_valid_pos[i] != nullptr)
			delete this->_valid_pos[i];
	}
}

void GameBoard::setMode(Mode mode)
{
	this->_game_mode = mode;
}

Mode GameBoard::getMode() const
{
	return this->_game_mode;
}

/*Create new fruits during the game*/
void GameBoard::create_fruit(int time, int point_add, int time_on_screen, Point p)
{
	if(this->getMode() != Mode::LOAD && this->getMode() != Mode::SILENCE)
	{
		if(this->_fruit == nullptr)
		{
			int random_index = rand() % this->_valid_pos.size();/*Get random start position*/
			int random_point_add = rand() % 5 + 5;/*Get Random Score*/
			int random_time = rand() % 10 + 7; /*And get Random time to "live", 7-17 positions*/
			this->_fruit = new Fruit(*this->_valid_pos[random_index], random_point_add, random_time); /*Fruit C'tor*/
			if (this->getMode() == Mode::SAVE)
			{
				Fruit* f = this->_fruit;
				this->_game_doc.append_fruit_move(f->get_fruit_dir(),
										time,f->get_Fruit_time(),f->get_PointAdd(),f->get_Point());
			}
		}
	}
	else
	{
		this->_fruit = new Fruit(p, point_add, time_on_screen); /*Fruit C'tor On Load Mode*/
		this->_fruit->reduce_time_on_screen();
	}
}

void GameBoard::print()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			char ch = this->_console[i][j];
			Color color = Color::WHITE;
			if (ch == Signs::WALLS_SIGN)
				color = Color::LIGHTBLUE;
			else if (ch == Signs::PACMAN_SIGN)
				color = Color::YELLOW;
			else if (ch == Signs::GHOST_SIGN)
				color = Color::RED;
			else if (ch == Signs::BREADCRUMBS_SIGN)
				color = Color::LIGHTMAGENTA;
			else if (ch == Signs::TABOO_AREA)
				ch = Signs::SPACE_SIGN; 
			else
				color = Color::WHITE;
			if(this->getMode() != Mode::SILENCE)
				gotoxy_print(j, i, ch, color);
			
		}
	}
	this->print_pacman();
	for (size_t i = 0; i < this->_ghosts.size(); ++i)
	{
		this->print_ghost(i);
	}
}

// Printing functions during running

void GameBoard::print_score_and_lives(const int score, const int lives) const
{
	if (this->_score_pos != this->_lives_pos) {
		if (this->_game_mode != Mode::SILENCE)
		{
			gotoxy_print(this->_score_pos, "Score: " + std::to_string(score));
			//gotoxy_print(this->_lives_pos, "Lives: " + std::to_string(this->_count_food));
			gotoxy_print(this->_lives_pos, "Lives: " + std::to_string(lives));
		}
	}
}

void GameBoard::print_pacman() const
{
	if(this->_game_mode != Mode::SILENCE )
		gotoxy_print(*this->_pacman, Signs::PACMAN_SIGN, Color::YELLOW);
	
}

void GameBoard::print_ghost(const int ghost_number) const
{
	Color ghost_color = Color::RED;
	if (ghost_number == 0)
		ghost_color = Color::GREEN;
	else if (ghost_number == 1)
		ghost_color = Color::CYAN;
	else if (ghost_number == 2)
		ghost_color = Color::LIGHTRED;
	else if (ghost_number == 3)
		ghost_color = Color::BLUE;
	if(this->_game_mode != Mode::SILENCE)
		gotoxy_print(*this->_ghosts[ghost_number], GHOST_SIGN, ghost_color);
}

void GameBoard::print_fruit() const
{
	if (this->_game_mode != Mode::SILENCE)
		gotoxy_print(*this->_fruit, this->_fruit->get_Fruit_Sign(), Color::LIGHTGREEN);
}

void GameBoard::print_food(const int x, const int y) const
{
	if (this->_game_mode != Mode::SILENCE)
		gotoxy_print(x, y, BREADCRUMBS_SIGN, Color::LIGHTMAGENTA);
}

void GameBoard::reset_and_print_board()
{
	if (this->_game_mode != Mode::SILENCE)
		gotoxy_print(*this->_pacman, this->_console[this->_pacman->get_x()][this->_pacman->get_y()], Color::LIGHTMAGENTA);
	this->_pacman->restart_Pacman(); // reset pacman position
	this->print_pacman();
	for (size_t i = 0; i < this->_ghosts.size(); i++)
	{
		// remove ghost from prev location
		if (this->_game_mode != Mode::SILENCE)
			gotoxy_print(*this->_ghosts[i], this->_console[this->_ghosts[i]->get_x()][this->_ghosts[i]->get_y()], Color::LIGHTMAGENTA);
		this->_ghosts[i]->restart_Ghost(); // reset ghosts position
		this->print_ghost(i);
	}
	this->remove_fruit(); /*destroy fruit if is exist*/
}

/*Move functions*/
void GameBoard::move_pacman(Direction& key_dir, int time)
{
	if (key_dir == Direction::STAY)
	{
		if (this->_game_mode == Mode::SAVE)
			this->_game_doc.append_pacman_move(key_dir, time);
		return;
	}
	Point p_dir = Point::convert_direction_to_point(key_dir);
	Point new_p = *this->_pacman + p_dir;
	//secret tunnel jump
	if (new_p.get_x() < 0 || new_p.get_x() > this->_end_scan_board.get_x() || new_p.get_y() < 0 || new_p.get_y() > this->_end_scan_board.get_y())
		// if player want to go out of the boarder
	{
		if (this->_console[this->_pacman->get_x()][this->_pacman->get_y()] == Signs::SPACE_SIGN ||
			this->_console[this->_pacman->get_x()][this->_pacman->get_y()] == Signs::BREADCRUMBS_SIGN ||
			this->_console[this->_pacman->get_x()][this->_pacman->get_y()] == Signs::FREE_AREA) // valid cell to walk on
			// and should have secret tunnel
		{
			if(this->getMode() != Mode::SILENCE)
				gotoxy_print(*this->_pacman, Signs::SPACE_SIGN);
			Point tmp = Point(*this->_pacman);
			if (new_p.get_y() == -1)
			{
				tmp.set_y(this->_end_scan_board.get_y());
			}
			else if (new_p.get_y() == this->_end_scan_board.get_y() +1)
			{
				tmp.set_y(0);
			}

			if (new_p.get_x() == -1)
			{
				tmp.set_x(this->_end_scan_board.get_x());
			}
			else if (new_p.get_x() == this->_end_scan_board.get_x()+1)
			{
				tmp.set_x(0);
			}
			char c_tmp = this->_console[tmp.get_x()][tmp.get_y()];
			if(c_tmp == Signs::SPACE_SIGN ||
				c_tmp == Signs::BREADCRUMBS_SIGN ||
				c_tmp == Signs::FREE_AREA)
			{
				this->_pacman->set_point(tmp);
				if (this->_console[tmp.get_x()][tmp.get_y()] == Signs::BREADCRUMBS_SIGN)
				{
					this->_add_point += 10; // todo replace 10 with fruit point

					this->_count_food -= 1;
					this->_console[tmp.get_x()][tmp.get_y()] = Signs::SPACE_SIGN;
				}
				if (this->is_fruit_on_screen() && tmp == *this->_fruit)
				{
					this->_add_point += this->_fruit->get_PointAdd() * 10;
					if (this->_game_mode == Mode::SAVE)
						this->_game_doc.append_fruit_move(this->_fruit->get_fruit_dir(), time,
							0, this->_fruit->get_PointAdd(), this->_fruit->get_Point());
					this->remove_fruit();
				}
			}
			else
			{
				key_dir = Direction::STAY;
			}
			this->print_pacman();
			if (this->_game_mode == Mode::SAVE)
				this->_game_doc.append_pacman_move(key_dir, time);
			return;
		}
		key_dir = Direction::STAY;
		return;
	}
	if (this->_console[new_p.get_x()][new_p.get_y()] == Signs::WALLS_SIGN ||
		this->_console[new_p.get_x()][new_p.get_y()] == Signs::TABOO_AREA)
	{
		key_dir = Direction::STAY;
		if (this->_game_mode == Mode::SAVE)
			this->_game_doc.append_pacman_move(key_dir, time);
		return;
	}
	if (this->_console[new_p.get_x()][new_p.get_y()] == Signs::BREADCRUMBS_SIGN)
	{
		this->_add_point += 10; // todo replace 10 with fruit point

		this->_count_food -= 1;
		this->_console[new_p.get_x()][new_p.get_y()] = Signs::SPACE_SIGN;
	}
	if (this->is_fruit_on_screen() && new_p == *this->_fruit)
	{
		this->_add_point += this->_fruit->get_PointAdd() * 10; 
		if (this->_game_mode == Mode::SAVE)
			this->_game_doc.append_fruit_move(this->_fruit->get_fruit_dir(), time,
				0, this->_fruit->get_PointAdd(), this->_fruit->get_Point());
		this->remove_fruit();
	}
	if(this->getMode() != Mode::SILENCE)
		gotoxy_print(*this->_pacman, Signs::SPACE_SIGN);
	this->_pacman->set_point(new_p);
	this->print_pacman();
	if(this->_game_mode == Mode::SAVE)
		this->_game_doc.append_pacman_move(key_dir, time);
}

void GameBoard::move_ghost(const size_t ghost_number, int time)
{
	if (ghost_number < 0 || ghost_number > this->_ghosts.size())
		return;
	if (this->_ghosts[ghost_number] == nullptr)
		return;
	Ghost* g = this->_ghosts[ghost_number];
	Direction dir = g->get_direction();
	Point new_p;
	if(this->_game_mode != Mode::LOAD && this->_game_mode != Mode::SILENCE)
	{
		size_t random_tmp = rand() % 10 + 10; /*For good ghost*/
		/*Ghosts movement*/
		Ghost_Intelligence intelligence = g->get_intelligence();
		if(intelligence == Ghost_Intelligence::BEST)
		{
			best_level_algo(dir, new_p, ghost_number);
		}
		else if (intelligence == Ghost_Intelligence::GOOD)
		{
			if(this->_ghosts[ghost_number]->get_novice_count() <= 0)
			{
				best_level_algo(dir, new_p, ghost_number);
				if (g->get_count_move() % 20 >= random_tmp)
				{
					g->set_novice_count(5);
				}
			}
			else
			{
				novice_level_algo(dir, new_p, ghost_number);
				g->set_novice_count(g->get_novice_count() -1);
			}
		}
		else if (intelligence == Ghost_Intelligence::NOVICE)
		{
			if (g->get_count_move() % 20 == 0)
			{
				dir = Direction::STAY;
				g->set_direction(dir);
			}
			novice_level_algo(dir, new_p, ghost_number);
			g->set_novice_count(g->get_novice_count() - 1);
		}
		g->set_direction(dir);
	}
	else
	{
		new_p = *g + Point::convert_direction_to_point(dir);
	}
	
	if(new_p != *g)
	{
		const Ghost* old_point = g;
		if(this->getMode() != Mode::SILENCE)
			gotoxy_print(*g, this->_console[old_point->get_x()][old_point->get_y()], Color::LIGHTMAGENTA);
		g->set_point(new_p);
		if (this->is_fruit_on_screen() && *g == *this->_fruit)
		{
			if (this->_game_mode == Mode::SAVE)
				this->_game_doc.append_fruit_move(this->_fruit->get_fruit_dir(), time,
					0, this->_fruit->get_PointAdd(), this->_fruit->get_Point());
			this->remove_fruit();
		}
		this->print_ghost(ghost_number);
	}

	g->increase_move();
	if(this->_game_mode == Mode::SAVE)
		this->_game_doc.append_ghost_move(g->get_direction(), time, ghost_number);

}

void GameBoard::set_ghost_dir(const size_t ghost_number, const Direction dir)
{
	if (ghost_number < 0 || ghost_number > this->_ghosts.size())
		return;
	if (this->_ghosts[ghost_number] == nullptr)
		return;
	this->_ghosts[ghost_number]->set_direction(dir);
}

void GameBoard::move_fruit(int time, const Direction load_dir)
{
	if (this->_fruit == nullptr)
		return;
	if(this->_fruit->get_Fruit_time() <= 0 ) /*******************remove fruit********/
	{
		if (this->_game_mode == Mode::SAVE && this->_fruit->get_is_fruit_on_screen())
		{
			this->_game_doc.append_fruit_move(Direction::STAY, time, this->_fruit->get_Fruit_time(),
				this->_fruit->get_PointAdd(), this->_fruit->get_Point());
		}
		this->remove_fruit();
		return;
	}
	Direction dir = this->_fruit->get_fruit_dir();
	Point p_dir;
	Point new_p;
	if (this->getMode() != Mode::LOAD && this->getMode() != Mode::SILENCE)
	{
		do
		{
			if (dir == Direction::STAY)// || (rand()%100) < 10) // is need to change direction 
			{
				int r = rand() % 4;
				switch (r)
				{
				case 0:
					dir = Direction::RIGHT;
					break;
				case 1:
					dir = Direction::LEFT;
					break;
				case 2:
					dir = Direction::UP;
					break;
				case 3:
					dir = Direction::DOWN;
					break;
				}
			}

			p_dir = Point::convert_direction_to_point(dir);
			new_p = *this->_fruit + p_dir;

			if (this->_console[new_p.get_x()][new_p.get_y()] == Signs::WALLS_SIGN ||
				this->_console[new_p.get_x()][new_p.get_y()] == Signs::TABOO_AREA)
			{
				dir = Direction::STAY;
			}
			if (new_p.get_x() <= 0 || new_p.get_x() >= this->_end_scan_board.get_x() ||
				new_p.get_y() <= 0 || new_p.get_y() >= this->_end_scan_board.get_y())
			{
				dir = Direction::STAY;
			}
		} while (dir == Direction::STAY);
		this->_fruit->set_fruit_direction(dir);
	}
	else
	{
		dir = load_dir;
		this->_fruit->set_fruit_direction(dir);
		p_dir = Point::convert_direction_to_point(dir);
		new_p = *this->_fruit + p_dir;

		
	}
	
	const Point old_point = *this->_fruit;
	if(this->getMode() != Mode::SILENCE)
		gotoxy_print(old_point, this->_console[old_point.get_x()][old_point.get_y()], Color::LIGHTMAGENTA);
	this->_fruit->set_point(new_p);
	if (*this->_pacman == *this->_fruit)
	{
		this->_add_point += this->_fruit->get_PointAdd() * 10;
		/*******************remove fruit********/
		if (this->_game_mode == Mode::SAVE)
			this->_game_doc.append_fruit_move(dir, time, 0, this->_fruit->get_PointAdd(), this->_fruit->get_Point());
		this->remove_fruit();
		return;
	}
	for (size_t i = 0; i < this->_ghosts.size(); ++i)
	{
		/*******************remove fruit********/
		if (*this->_ghosts[i] == *this->_fruit)
		{
			if (this->_game_mode == Mode::SAVE)
				this->_game_doc.append_fruit_move(dir, time, 0, this->_fruit->get_PointAdd(), this->_fruit->get_Point());
			this->remove_fruit();
			return;
		}
	}
	this->print_fruit();
	this->_fruit->reduce_time_on_screen();
	if (this->_game_mode == Mode::SAVE)
	{
		Point p = { this->_fruit->get_x(), this->_fruit->get_y() };
		this->_game_doc.append_fruit_move(dir, time, this->_fruit->get_Fruit_time(), 
			this->_fruit->get_PointAdd(),this->_fruit->get_Point());
	}
}

void GameBoard::remove_fruit()
{
	/*If fruit not exist*/
	if (this->_fruit == nullptr) return;
	/*else, fruit exist*/
	if(getMode() != Mode::SILENCE)
		gotoxy_print(*this->_fruit, this->_console[this->_fruit->get_x()][this->_fruit->get_y()], Color::LIGHTMAGENTA);
	if (*this->_pacman == *this->_fruit)
	{
		print_pacman();
	}
	for (size_t i = 0; i < this->_ghosts.size(); ++i)
	{
		if (*this->_ghosts[i] == *this->_fruit)
		{
			print_ghost(i);
		}
	}
	delete this->_fruit; /*******************remove fruit********/
	this->_fruit = nullptr;
}

bool GameBoard::is_fruit_on_screen() const
{
	return this->_fruit != nullptr;
}

/*Game status functions*/
const bool GameBoard::is_file_valid() const
{
	return this->_file_is_fine;
}

bool GameBoard::is_win() const
{
	return this->_count_food == 0;
}

bool GameBoard::is_lose() const
{
	bool is_lose = false;
	for (size_t i = 0; i < this->_ghosts.size(); i++)
	{
		if (this->_ghosts[i]->get_x() == this->_pacman->get_x() && this->_ghosts[i]->get_y() == this->_pacman->get_y())
		{
			if (*this->_ghosts[i] == *this->_pacman)
			{
				is_lose = true;
			}
		}

	}
	return is_lose;
}
 /*More functions to update for the game...*/
int GameBoard::get_Ghost_Size() const
{
	return this->_ghosts.size();
}

int GameBoard::get_and_reset_add_point()
{
	int tmp = this->_add_point;
	this->_add_point = 0;
	return tmp;
}

void GameBoard::best_level_algo(Direction& dir, Point& new_p, int ghost_number)
{
	std::set<Point> s;

	// Traverse the Vector
	for (Point* p : this->_valid_pos) {

		// Insert each element
		// into the Set
		s.insert(*p);
	}
	Direction old_dir = dir;
	dir = this->_ghosts[ghost_number]->BFS_next_move(s, this->_pacman);
	if (dir == Direction::STAY) {
		this->novice_level_algo(old_dir, new_p, ghost_number);
		dir = old_dir;
	}
	else
		new_p = *this->_ghosts[ghost_number] + Point::convert_direction_to_point(dir);
}

void GameBoard::novice_level_algo(Direction& dir, Point& new_p, int ghost_number)
{
	do
	{
		if (dir == Direction::STAY)// || (rand()%100) < 10) // is need to change direction 
		{
			int r = rand() % 4;
			switch (r)
			{
			case 0:
				dir = Direction::RIGHT;
				break;
			case 1:
				dir = Direction::LEFT;
				break;
			case 2:
				dir = Direction::UP;
				break;
			case 3:
				dir = Direction::DOWN;
				break;
			}
		}

		new_p = *this->_ghosts[ghost_number] + Point::convert_direction_to_point(dir);

		if (this->_console[new_p.get_x()][new_p.get_y()] == Signs::WALLS_SIGN ||
			this->_console[new_p.get_x()][new_p.get_y()] == Signs::TABOO_AREA)
		{
			dir = Direction::STAY;
		}
		if (new_p.get_x() <= 0 || new_p.get_x() >= this->_end_scan_board.get_x() ||
			new_p.get_y() <= 0 || new_p.get_y() >= this->_end_scan_board.get_y())
		{
			dir = Direction::STAY;
		}
	} while (dir == Direction::STAY);
	this->_ghosts[ghost_number]->set_direction(dir);
}


void GameBoard::record_to_file(const std::string level) const
{
	if(this->_game_mode != Mode::SAVE)
		return;
	std::string out_fileName = level;
	std::size_t start_pos = out_fileName.find("pacman_");
	std::size_t end_pos = out_fileName.find(".screen");
	out_fileName = out_fileName.substr(start_pos, end_pos - start_pos);
	out_fileName += ".steps";
	std::ofstream outFile(out_fileName);

	outFile << this->_game_doc;

	outFile.close();
}
