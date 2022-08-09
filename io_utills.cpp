#include "io_utills.h"

bool is_color_on = true;

#ifndef WINDOWS
void set_terminal() {}
void gotoxy(int x, int y) {}
void gotoxy_print(const int x, const int y, const char ch) {}
int _getch(void) { return 0; }
int _kbhit(void) { return 0; }
void Sleep(unsigned long) {}
void hideCursor() {}
void clear_screen() {}
#else

//Font Change
void set_terminal()
{
	CONSOLE_FONT_INFOEX cfi;
	GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	cfi.cbSize = sizeof cfi;
	cfi.dwFontSize.X = 16;
	cfi.dwFontSize.Y = 16;
	wcscpy_s(cfi.FaceName, L"Terminal");//Font 
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void gotoxy(const int x, const int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	std::cout << std::flush;
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

//This function tells the cursor to go to a specific location and prints a character at that location
void gotoxy_print(const int x, const int y, const char ch, const Color color)
{
	gotoxy(x, y);
	set_text_color(color);
	printf("%c", ch);
	set_text_color(Color::WHITE);
}

void gotoxy_print(const Point& p, const char ch, const Color color)
{
	gotoxy_print(p.get_y(), p.get_x(), ch, color);
}

//This function tells the cursor to go to a specific location and prints a character at that location
void gotoxy_print(const int x, const int y, const std::string text, const Color color)
{
	gotoxy(x, y);
	set_text_color(color);
	std::cout<< text;
	set_text_color(Color::WHITE);
}

void gotoxy_print(const Point& p, const std::string text, const Color color)
{
	gotoxy_print(p.get_y(), p.get_x(), text, color);
}

void set_text_color(Color color)
{
	if (is_color_on)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)color);
}


void hideCursor()
{
	HANDLE myconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CURSOR;
	CURSOR.dwSize = 1;
	CURSOR.bVisible = FALSE;
	SetConsoleCursorInfo(myconsole, &CURSOR);//second argument need pointer
}

void clear_screen()
{
	system("CLS");
}

#endif
