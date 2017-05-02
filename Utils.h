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

using namespace std;

const string FILE_NOT_FOUND = "file not found";
const string ERROR_FINDING_PATH = "error finding path";

class Utils
{
	public:		
		static void gotoxy(int x, int y);
		static void setcolor(WORD color);
		static void setForeGroundColor(int ForeGroundColor);
		static COORD GetConsoleCursorPosition(HANDLE hConsoleOutput);
		static void Utils::updateBoardPrint(COORD hit_coord, char current);
		static string find_path(const string& path_expr_to_find);
		static void set_quiet(bool is_quiet);
		static bool get_quiet();
		static void set_delay(int delay);
		static const int DEFAULT_PRINT_DELAY = 2000;
		static const int DEFAULT_START_ANIMATION_DELAY = 350;
		static const int DEFAULT_END_ANIMATION_DELAY = 500;
		static const int WHITE_COLOR = 7;
		static const int RED_COLOR = 4;
		static const int PRINT_INDENT = 4;
		static const char HIT_SIGN = 'X';

	private:
		Utils(){} //dissallow crating instances of this class
		static bool QUIET;
		static int MILISECONDS_PRINT_DELAY;
};
