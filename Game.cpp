#include "Game.h"


Game::Game(): _score(0), _lives(3)
{
	// initialize score and lives 
}


Game_status Game::start_game(const std::string level, const Ghost_Intelligence ghost_intelligence, Mode mode, Load_Silent_status& load_silent)
{
	int timer = 0;
	int finish_game = 0;
	Game_status status;
	GameDoc game_doc;
	system("CLS");
	this->_game_board = new GameBoard(level, ghost_intelligence);
	if(!this->_game_board->is_file_valid())
	{
		return Game_status::FILE_INVALID;
	}
	//print board + extra
	this->_game_board->setMode(mode);
	this->_game_board->print();

	this->_game_board->print_score_and_lives(this->_score, this->_lives);

	int x = 1;
	int y = 1;
	char ch = ' ';

	Direction key_dir = Direction::STAY;
	bool flag_ghost_move = false;
	int fruit_timer = 0; /*fruit tempo*/
	int fruit_not_on_screen_timer = rand() % 30 + 90; /*Count how match time pass until fruit create*/

	if (mode == Mode::LOAD || mode == Mode::SILENCE)
	{
		std::string in_fileName = level;
		std::size_t start_pos = in_fileName.find("pacman_");
		std::size_t end_pos = in_fileName.find(".screen");
		in_fileName = in_fileName.substr(start_pos, end_pos - start_pos);
		in_fileName += ".steps";
		std::ifstream inFile(in_fileName);
		game_doc.clear_vectors();
		inFile >> game_doc;
		inFile.close();
	}
	
	while (true)
	{
		// esc (Ascii 27) ends the loop
		while ((mode == Mode::LOAD || mode == Mode::SILENCE) || !_kbhit() || (ch = _getch()) != ESC_ASCII_NUM)
		{
			if (mode == Mode::LOAD || mode == Mode::SILENCE)
			{
				game_doc.get_pacman_move(timer, key_dir);

				this->_game_board->move_pacman(key_dir, timer);
				flag_ghost_move = true;

			}
			else
			{
				switch (ch)
				{
					case 'a':case 'A': // Left
					{
						key_dir = Direction::LEFT;
						flag_ghost_move = true;
						break;
					}
					case 'w':case 'W': // Up
					{
						key_dir = Direction::UP;
						flag_ghost_move = true;
						break;
					}
					case 'd':case 'D': // Right
					{
						key_dir = Direction::RIGHT;
						flag_ghost_move = true;
						break;
					}
					case 's':case 'S': // Stay
					{
						key_dir = Direction::STAY;
						flag_ghost_move = true;
						break;
					}
					case 'x':case 'X': // Down
					{
						key_dir = Direction::DOWN;
						flag_ghost_move = true;
						break;
					}
				}
				this->_game_board->move_pacman(key_dir, timer);
			}
			
			
			if (this->_game_board->is_win())
			{
				if (mode == Mode::SAVE)
					this->_game_board->record_to_file(level);
				// print WIN SCREEN and return to menu
				// call here to func print screen wining
				if (mode != Mode::LOAD && mode != Mode::SILENCE)
					print_winning_screen();
				delete this->_game_board;
				finish_game = timer;
				status = Game_status::WIN;
				if (mode == Mode::SAVE)
					result_doc(level, mode, status, timer, pacman_burn_vector);
				
				if (mode == Mode::SILENCE || mode == Mode::LOAD)
				{
					if (checkSilenceMode(level, status, timer, pacman_burn_vector))
					{
						if (mode == Mode::SILENCE)
						{
							load_silent = Load_Silent_status::SILENT_SUCCED;
						}
						if (mode == Mode::LOAD)
						{
							load_silent = Load_Silent_status::LOAD_SUCCEED;
						}
					}
					else
					{
						
						if (mode == Mode::SILENCE)
						{
							load_silent = Load_Silent_status::SILENT_FAILED;
						}
						if (mode == Mode::LOAD)
						{
							load_silent = Load_Silent_status::LOAD_FAILED;
						}
					}
				}
				return Game_status::WIN;
			}
			if (this->_game_board->is_lose()) // If hit a ghost,  pacman failed in this round, 
			{
				this->_game_board->reset_and_print_board(); //return pacman and the ghosts to the first place
				pacman_burn_vector.push_back(timer);
				flag_ghost_move = false;
				key_dir = Direction::STAY;
				ch = ' ';
				this->_lives--;
				if (this->_lives <= 0) // LOSE check
				{
					if (mode != Mode::LOAD && mode != Mode::SILENCE)
						print_lose_screen();
					if (mode == Mode::SAVE)
						this->_game_board->record_to_file(level);
					delete this->_game_board;
					finish_game = timer;
					status = Game_status::LOSE;
					//pacman_burn_vector.push_back(timer);
					if (mode == Mode::SAVE)
					{
						result_doc(level, mode, status, timer, pacman_burn_vector);
					}
					if(mode == Mode::SILENCE || mode == Mode::LOAD)
					{
						if(checkSilenceMode(level, status, timer, pacman_burn_vector))
						{
							if(mode == Mode::SILENCE)
								load_silent = Load_Silent_status::SILENT_SUCCED;
							if (mode == Mode::LOAD)
								load_silent = Load_Silent_status::LOAD_SUCCEED;
							
						}
						else
						{
							if (mode == Mode::SILENCE)
								load_silent = Load_Silent_status::SILENT_FAILED;
							if (mode == Mode::LOAD)
								load_silent = Load_Silent_status::LOAD_FAILED;
						}
					}
					return Game_status::LOSE;
				}
				this->_game_board->print_score_and_lives(this->_score, this->_lives);
				
			}
			
			if (flag_ghost_move) {
				if (!this->_game_board->is_fruit_on_screen())
				{
					if (mode == Mode::LOAD || mode == Mode::SILENCE || fruit_not_on_screen_timer <= 0) {
						if (mode == Mode::LOAD || mode == Mode::SILENCE)
						{
							Direction fruit_dir;
							int sign_tmp;
							Point p_tmp;
							int tos_tmp;
							if (game_doc.get_fruit_move(timer, fruit_dir, sign_tmp, p_tmp, tos_tmp)) 
							{
								this->_game_board->create_fruit(timer, sign_tmp, tos_tmp,p_tmp );
								fruit_timer = 0;
							}
						}
						else
						{
							this->_game_board->create_fruit(timer);
							fruit_not_on_screen_timer = rand() % 30 + 90;/*fruit not on screen, between 90 to 120 positions*/
							fruit_timer = 0;
						}					
					}
					else
					{
						fruit_not_on_screen_timer--;
					}
				}
				if (this->_game_board->is_fruit_on_screen())
				{
					if (fruit_timer % 4 == 0)/*Fruit speed*/
					{
						if(mode == Mode::LOAD || mode == Mode::SILENCE)
						{
							Direction fruit_dir;
							int sign_tmp;
							Point p_tmp;
							int tos_tmp;
							if(game_doc.get_fruit_move(timer, fruit_dir, sign_tmp, p_tmp, tos_tmp))
								this->_game_board->move_fruit(timer, fruit_dir);	
						}
						else
							this->_game_board->move_fruit(timer);
					}
					fruit_timer++;
				}
			
				
				if (timer % 2 == 0  || mode == Mode::LOAD || mode == Mode::SILENCE) // if it's a ghost step  
				{
					for(int g = 0 ; g < this->_game_board->get_Ghost_Size(); g++)
					{
						if (mode == Mode::LOAD || mode == Mode::SILENCE)
						{
							Direction g_dir = Direction::STAY;
							if (game_doc.get_ghost_move(timer, g_dir, g))
							{
								this->_game_board->set_ghost_dir(g, g_dir);
								g_dir = Direction::STAY;
							}
						}
						if (timer % 2 == 0)
							this->_game_board->move_ghost(g, timer);
						
					}
					if (timer % 2 == 0 && this->_game_board->is_lose())
					{
						this->_game_board->reset_and_print_board();
						pacman_burn_vector.push_back(timer);
						flag_ghost_move = false;
						key_dir = Direction::STAY;
						ch = ' ';
						this->_lives--; // update Pacman lives
						if (this->_lives <= 0)
						{
							if(mode != Mode::LOAD && mode != Mode::SILENCE)
								print_lose_screen();
							this->_game_board->record_to_file(level);
							delete this->_game_board;
							finish_game = timer;
							status = Game_status::LOSE;
							
							if(mode == Mode::SAVE)
								result_doc(level, mode, status, timer, pacman_burn_vector);
							if (mode == Mode::SILENCE || mode == Mode::LOAD)
							{
								if (checkSilenceMode(level, status, timer, pacman_burn_vector))
								{
									if (mode == Mode::SILENCE)
										load_silent = Load_Silent_status::SILENT_SUCCED;
									if (mode == Mode::LOAD)
										load_silent = Load_Silent_status::LOAD_SUCCEED;
								}
								else
								{
									if (mode == Mode::SILENCE)
										load_silent = Load_Silent_status::SILENT_FAILED;
									if (mode == Mode::LOAD)
										load_silent = Load_Silent_status::LOAD_FAILED;
								}
							}
							return Game_status::LOSE;
						}
						this->_game_board->print_score_and_lives(this->_score, this->_lives);
						//pacman_burn_vector.push_back(timer);
					}
				}
			}
			
			if (key_dir == Direction::STAY)
			{
				ch = ' ';
			}
			const int add_point = this->_game_board->get_and_reset_add_point();
			if (add_point > 0)
			{
				this->_score += add_point;
				this->_game_board->print_score_and_lives(this->_score, this->_lives);
			}
			timer++;
			if(mode == Mode::SAVE || mode == Mode::SIMPLE)
				Sleep(PACMAN_SPEED);
			if (mode == Mode::LOAD)
				Sleep(60);
		}
		gotoxy(0, 26);
		std::cout << "~ Game Paused, Press ESC again to continue... ~" << std::endl;
		while (_kbhit() == 0 && _getch() != ESC_ASCII_NUM);
		gotoxy(0, 26);
		std::cout << "                                                 " << std::endl;
	}
	delete this->_game_board;
}

void Game:: result_doc(const std::string level, Mode mode, Game_status status, int timer, std::vector<int>& pacmam_burn_vec)
{
	if (mode != Mode::SAVE)
		return;
	std::string out_fileName = level;
	std::size_t start_pos = out_fileName.find("pacman_");
	std::size_t end_pos = out_fileName.find(".screen");
	out_fileName = out_fileName.substr(start_pos, end_pos - start_pos);
	out_fileName += ".results";
	std::ofstream outFile(out_fileName);
	for(int i : pacmam_burn_vec)
	{
		outFile << "Pacman-Ghost smash in " << i << " moves." << std::endl;
	}
	outFile << "Finish Level: " << timer << " moves." << std::endl;
	outFile << "Pacman is ";
	if(status == Game_status::WIN)
	{
		outFile << "Win!";
	}
	else if (status == Game_status::LOSE)
	{
		outFile << "LOSE!";
	}
	outFile.close();
	pacmam_burn_vec.clear();
}

bool Game::checkSilenceMode(const std::string level, Game_status status, int timer, std::vector<int>& pacman_burn_vec) const
{
	std::string results_file = level;
	std::size_t start_pos = results_file.find("pacman_");
	std::size_t end_pos = results_file.find(".screen");
	results_file = results_file.substr(start_pos, end_pos - start_pos);
	results_file += ".results";
	std::string line;
	std::string str_to_cmp;
	std::ifstream inFile;
	inFile.open(results_file);
	for (int i : pacman_burn_vec)
	{
		getline(inFile, line);
		str_to_cmp = "Pacman-Ghost smash in ";
		str_to_cmp += std::to_string(i);
		str_to_cmp += " moves.";
		if(str_to_cmp != line)
		{
			pacman_burn_vec.clear();
			return false;
		}
	}
	str_to_cmp.erase();
	getline(inFile, line);
	str_to_cmp = "Finish Level: ";
	str_to_cmp += std::to_string(timer);
	str_to_cmp += " moves.";
	if (str_to_cmp != line)
	{
		pacman_burn_vec.clear();
		return false;
	}
	str_to_cmp.erase();
	getline(inFile, line);
	str_to_cmp = "Pacman is ";
	if(status == Game_status::WIN)
	{
		str_to_cmp += "Win!";
		if (str_to_cmp != line)
		{
			pacman_burn_vec.clear();
			return false;
		}
	}
	if (status == Game_status::LOSE)
	{
		str_to_cmp += "LOSE!";
		if (str_to_cmp != line)
		{
			pacman_burn_vec.clear();
			return false;
		}
	}
	pacman_burn_vec.clear();
	return true;
}

void Game::print_winning_screen()
{
		system("CLS");
		set_text_color(Color::GREEN);
		std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl
		<< std::endl << std::endl << "\t\t\t\t\t\t YOU WIN!!!" << std::endl << std::endl;
		set_text_color(Color::WHITE);
		std::cout << std::endl << std::endl << std::endl << std::endl << std::endl
			<< " Please enter any key to continue..." << std::endl;
		(void)_getch();
}

void Game::print_lose_screen()
{
	system("CLS");
	set_text_color(Color::RED);
	std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl
	<< std::endl << std::endl << "\t\t\t\t\t\t YOU ARE A LOSER!!!" << std::endl << std::endl;
	set_text_color(Color::WHITE);
	std::cout << std::endl << std::endl << std::endl << std::endl << std::endl <<
		" Please enter any key return to Main Menu..." << std::endl;
	(void)_getch();
}


