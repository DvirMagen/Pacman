
#include <iostream>
#include <string>
#include "Game.h"
#include "io_utills.h"
#include <vector>
#include <filesystem>

namespace  fs = std::filesystem;


void printMenu();
void printLevelsMenu();
void print_instruction_and_keys();
void upadte_Vector(std::vector<std::string>, std::string path);
void load_mode_game_print();
bool run_levels(Game game,std::vector<std::string> fileNames, Game_status& game_status,  Load_Silent_status& load_silent,
	Mode mode = Mode::SIMPLE, std::string file_name = "");

int main(int argc, char* argv[])
{
	set_terminal();
	std::string userChoose;
	std::string levelPath = "Pacman_Boards\\";
	std::string const levelType = ".screen";
	std::string file_name;
	std::vector<std::string> fileNames(0);
	for(const auto& entry: fs::directory_iterator(levelPath))
	{
		if(entry.path().extension() == levelType)
		{
			fileNames.push_back(entry.path().string());
		}
	}
	Mode game_mode = Mode::SIMPLE;
	if (argc > 1)
	{
		const char* mode = argv[1];
		if (strcmp(mode, "-load") == 0)
		{
			if (argc > 2 && strcmp(argv[2], "-silent") == 0)
				game_mode = Mode::SILENCE;
			else
			{
				game_mode = Mode::LOAD;
				load_mode_game_print();
			}
				
		}
		else if (strcmp(mode, "-save") == 0)
		{
			game_mode = Mode::SAVE;
		}
		else
		{
			game_mode = Mode::SIMPLE;
		}
	}
	while (true)
	{
		Game game = Game();
		Game_status game_status = Game_status::WIN;
		Load_Silent_status load_silent = Load_Silent_status::LOAD_SUCCEED;

		if (game_mode != Mode::LOAD && game_mode != Mode::SILENCE)
		{
			printMenu();
			std::cin >> userChoose;
			while ((userChoose != "0") && (userChoose != "1") && (userChoose != "2") &&
				(userChoose != "8") && (userChoose != "9"))
			{
				set_text_color(Color::RED);
				std::cout << " Error: ";
				set_text_color();
				std::cout << " Invalid value" << std::endl << " Please enter your choose: ";
				std::cin >> userChoose;
			}
		}
		else
			userChoose = "1";
		switch (stoi(userChoose))
		{
			case 0:
			{
				is_color_on = !is_color_on;
				break;
			}
			case 1:
			{
				run_levels(game, fileNames, game_status,load_silent, game_mode);
				if (game_mode == Mode::LOAD)
				{
					system("CLS");
					hideCursor();
					if (load_silent == Load_Silent_status::LOAD_SUCCEED)
					{
						std::cout << "SUCCEED - Load Mode Finish..." << std::endl << std::endl;
					}
					if (load_silent == Load_Silent_status::LOAD_FAILED)
					{
						std::cout << "FAILD - Load Mode Finish..." << std::endl << std::endl;
					}

					return 0;
				}
				if (game_mode == Mode::SILENCE)
				{
					hideCursor();
					if (load_silent == Load_Silent_status::SILENT_SUCCED)
					{
						std::cout << "SUCCEED - Silent Mode Finish..." << std::endl << std::endl;
					}
					if (load_silent == Load_Silent_status::SILENT_FAILED)
					{
						std::cout << "FAILD - Silent Mode Finish..." << std::endl << std::endl;
					}
					return 0;
				}
				break;
			}
			case 2:
			{
				system("CLS");
				std::cout << "Please Enter File Name: " << std::endl;
				std::cin >> file_name;
				hideCursor();
				levelPath.append(file_name);
				levelPath.append(levelType);
				const bool file_exist = run_levels(game, fileNames, game_status,load_silent, game_mode, levelPath);
				if (!file_exist)
				{
					system("CLS");
					std::cout << std::endl << std::endl << std::endl << "\t\t\t\t\t\t" << "Your File is Not Exist..." <<
						std::endl << std::endl << "\t\t\t\t\t" << "  Enter any key to get back to menu..." << std::endl;
					(void)_getch();
				}
				levelPath = "Pacman_Boards\\";
				break;
			}
			case 8:
			{
				print_instruction_and_keys();
				break;
			}
			case 9:
			{
				system("CLS");
				hideCursor();
				std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl
					<< "\t\t\t\t\t\tThank you!" << std::endl << std::endl << std::endl << std::endl << "\t\t\t\t\t\t Goodbye!" << std::endl <<
					std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
				return 0;
			}
		}
	}

}

void printMenu()
{
	system("CLS");
	set_text_color(Color::GREEN);
	std::cout << " Welcome to my new Pacman game " << std::endl;
	std::cout << " Be prepared for an unforgettable experience " << std::endl;
	std::cout << " Watch out for ghosts haunting you " << std::endl << std::endl;
	set_text_color(Color::LIGHTBLUE);
	std::cout << " Menu: " << std::endl;
	if (is_color_on)
		std::cout << " 0. Press 0 to turn off color display." << std::endl;
	else
		std::cout << " 0. Press 0 to turn on color display." << std::endl;
	std::cout << " 1. Start Game!" << std::endl;
	std::cout << " 2. Enter Level By Yourself..." << std::endl;
	std::cout << " 8. Present instruction and keys" << std::endl;
	std::cout << " 9. Exit " << std::endl;
	set_text_color();
	std::cout << std::endl << " Please enter your choose: ";
}

void printLevelsMenu()
{
	system("CLS");
	std::cout << " Levels Menu:  " << std::endl<< std::endl;
	std::cout << "  (a) BEST" << std::endl;
	std::cout << "  (b) GOOD" << std::endl;
	std::cout << "  (c) NOVICE" << std::endl;
	std::cout << "  Please Choose Level:  ";
}

void print_instruction_and_keys()
{
	system("CLS");
	hideCursor();
	std::cout << " The game has 3 characters: Pacman and 2 GHOSTS." << std::endl;
	std::cout << " You have 3 l ives, each time one of the GHOSTS manages to eat Pacman, you'll lose a life." << std::endl;
	std::cout << " There are 'breadcrumbs' - (*) -scattered on the board. Each breadcrumb Pacman manages to eat," << std::endl << " credits you with one point." << std::endl;
	std::cout << " Once all the breadcrumbs are eaten, the game ends." << std::endl;
	std::cout << " Besides breadcrumbs, there are fruits on the screen, represented by the numbers 5-9." << std::endl;
	std::cout << " If Pacman manages eat the fruit, it will credit you with the number that represented the fruit in points" << std::endl;
	std::cout << " The fruit will be able to move, and will disappear after some time." << std::endl << std::endl;
	std::cout << " You will control the direction Pacman will move." << std::endl << " After each direction you choose, " << std::endl;
	std::cout << " Pacman will continue to move in that direction, unless you'll press any other direction key." << std::endl;
	std::cout << " For LEFT press 'a', for RIGHT press 'd', for UP press 'w', for DOWN press 'x'." << std::endl;
	std::cout << " In case you want Pacman STAY IN PLACE, press 's'." << std::endl;
	std::cout << " If you want to pause the game, please press 'ESC'. If you wan to release the pause, press 'ESC' again." << std::endl;
	std::cout << " In case you hit the wall, Pacman will stay in place till you'll direct him to a legal direction." << std::endl << std::endl;
	std::cout << " You have 3 possible levels: (a)BEST, (b)GOOD and (c) NOVICE." << std::endl;
	std::cout << " If you choose BEST - the GHOSTS will chase Pacman." << std::endl;
	std::cout << " If you choose GOOD - the GHOSTS will chase Pacman, " << std::endl << " but occasionally they'll move in a single direction for a while." << std::endl;
	std::cout << " If you choose NOVICE - the GHOSTS will move in a single direction," << std::endl << " and will change it from time to time." << std::endl << std::endl << std::endl;
	std::cout << " To the main menu please press 'ENTER'";
	while (_kbhit() == 0 && _getch() != ENTER_ASCII_NUM);
}

void load_mode_game_print()
{
	system("CLS");
	hideCursor();
	gotoxy(35, 5);
	std::cout << " Game is Loading..." << std::endl;
	gotoxy(37, 7);
	std::cout << " Please Wait..." << std::endl;
	for(int i = 10; i >= 1; i--)
	{
		gotoxy(42, 9);
		std::cout <<   i << " Sec ";
		Sleep(700);
	}
}

bool run_levels(Game game, std::vector<std::string> fileNames, Game_status& game_status, Load_Silent_status& load_silent ,Mode mode,  std::string file_name)
{
	
	std::string difficulty;
	bool is_level_found = false;
	hideCursor();

	Ghost_Intelligence ghost_intelligence = Ghost_Intelligence::NOVICE;
	if(mode != Mode::LOAD && mode != Mode::SILENCE)
	{
		printLevelsMenu();
		std::cin >> difficulty;
		/*Check level input validity*/
		while ((difficulty != "a") && (difficulty != "b") && (difficulty != "c"))
		{
			set_text_color(Color::RED);
			std::cout << " Error: ";
			set_text_color();
			std::cout << " Invalid value" << std::endl << " Please Choose a Level: ";
			std::cin >> difficulty;
		}
		
		switch (difficulty[0])
		{
		case 'a':
			ghost_intelligence = Ghost_Intelligence::BEST;
			break;
		case 'b':
			ghost_intelligence = Ghost_Intelligence::GOOD;
			break;
		case 'c':
			ghost_intelligence = Ghost_Intelligence::NOVICE;
			break;
		}
	}
	/*Run the game*/
	for (std::string s_file: fileNames)
	{
		if(!file_name.empty() && file_name != s_file && !is_level_found) continue; /*find start level in vector*/
			is_level_found = true;
		game_status = game.start_game(s_file, ghost_intelligence, mode, load_silent);/*run the game if file is found, start game return user status*/
		if (game_status != Game_status::WIN ) /*If the file not valid or user loose in game*/
			break;
	}
	/*Appropriate message if the file is not valid*/
	if (is_level_found && game_status == Game_status::FILE_INVALID && mode != Mode::LOAD && mode != Mode::SILENCE)
	{
		system("CLS");
			std::cout << std::endl << std::endl << "\t\t\t\t\t" << "  Enter any key to get back to menu..." << std::endl;
		(void)_getch();
	}
	return is_level_found;
}



