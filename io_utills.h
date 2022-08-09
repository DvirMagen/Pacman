#pragma once
#ifndef _IO_UTILS_H_
#define _IO_UTILS_H_
 // you must have a config file, both for windows and for compiling on Linux (MAMA)
 // BUT, you need to change the content of the config!

#include <iostream>
#include "Color.h"
#include "Config.h"
#include "Point.h"
#include <vector>
#include <set>

extern bool is_color_on;

void set_terminal();
void gotoxy(const int x, const  int y);
void gotoxy_print(const int x, const int y, const char ch, const Color color = Color::WHITE);
void gotoxy_print(const Point& p, const char ch, const Color color = Color::WHITE);
void gotoxy_print(const int x, const int y, const std::string text, const Color color = Color::WHITE);
void gotoxy_print(const Point& p, const std::string text, const Color color = Color::WHITE);
void set_text_color(Color color = Color::WHITE);
void hideCursor();
void clear_screen();

#ifndef WINDOWS	
int _getch(void);
int _kbhit(void);
void Sleep(unsigned long);
#else
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#endif

#endif
