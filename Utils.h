#pragma once


#include <windows.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <sstream>
//#define DEBUG_BUILD

#ifdef DEBUG_BUILD
#define DEBUG(x) do { std::cerr << x << endl; } while (0)
#else
#define DEBUG(x) do {} while (0)
#endif

class Utils
{
	public:		
		static void gotoxy(int x, int y);
		static void setcolor(WORD color);
		static void setForeGroundColor(int ForeGroundColor);
		static COORD GetConsoleCursorPosition(HANDLE hConsoleOutput);
		static void Utils::updateBoardPrintHit(COORD hit_coord);
		static void Utils::updateBoardPrintMiss(COORD hit_coord, char current);
		static void set_quiet(bool is_quiet);
		static bool get_quiet();
		static void set_delay(int delay);
		static const int DEFAULT_PRINT_DELAY = 2000;
	private:
		Utils(){} //dissallow crating instances of this class
		static bool QUIET;
		static int MILISECONDS_PRINT_DELAY;
};
