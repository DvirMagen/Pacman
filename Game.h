#pragma once
#include <iostream>
#include "GameBoard.h"
#include "io_utills.h"
#include "vector"
#include "ostream"

enum class Game_status { WIN , LOSE , FILE_INVALID };
enum class Load_Silent_status { LOAD_SUCCEED, LOAD_FAILED, SILENT_SUCCED, SILENT_FAILED };

class Game
{ /* Game - Data Members*/
	GameBoard* _game_board = nullptr;
	int _score = 0;
	int _lives = 3;
	std::vector<int> pacman_burn_vector = {};
	
	static void print_winning_screen();
	static void print_lose_screen();

public:
	Game();
	Game_status start_game(const std::string level, const Ghost_Intelligence ghost_intelligence, Mode mode, Load_Silent_status& load_silnet);
	/*Doc to result file function*/
	static void result_doc(const std::string level, Mode mode, Game_status status, int timer, std::vector<int> &pacmam_burn_vec);
	bool checkSilenceMode(const std::string level , Game_status status, int timer, std::vector<int>& pacman_burn_vec) const;
};