#include "stdafx.h"
#include "Utils.h"
#include "GameFromFileManager.h"

using namespace std;

bool Utils::QUIET = false;
int Utils::MILISECONDS_PRINT_DELAY = Utils::DEFAULT_PRINT_DELAY;;

void Utils::gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x; coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return;
}

void Utils::setcolor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	return;
}

void Utils::setForeGroundColor(int ForeGroundColor)
{
	int color = ForeGroundColor;
	setcolor(color);
}

COORD Utils::GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

void Utils::updateBoardPrintHit(COORD hit_coord)
{
	if (!Utils::QUIET)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(MILISECONDS_PRINT_DELAY));
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD cursor_coord = GetConsoleCursorPosition(h);
		gotoxy(4 + hit_coord.X * 2 - 2, hit_coord.Y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); cout << "@";
		gotoxy(4 + hit_coord.X * 2 - 2, hit_coord.Y);
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); cout << " ";
		gotoxy(4 + hit_coord.X * 2 - 2, hit_coord.Y);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); cout << "X";
		gotoxy(cursor_coord.X, cursor_coord.Y);
		setcolor(7);
	}
}

void Utils::updateBoardPrintMiss(COORD hit_coord, char current)
{
	if (!Utils::QUIET)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(MILISECONDS_PRINT_DELAY));
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD cursor_coord = GetConsoleCursorPosition(h);
		gotoxy(4 + hit_coord.X * 2 - 2, hit_coord.Y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); cout << "@";
		gotoxy(4 + hit_coord.X * 2 - 2, hit_coord.Y);
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); cout << " ";
		gotoxy(4 + hit_coord.X * 2 - 2, hit_coord.Y);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); cout << current;
		gotoxy(cursor_coord.X, cursor_coord.Y);
		setcolor(7);
	}
}

void Utils::set_quiet(bool is_quiet)
{
	Utils::QUIET = is_quiet;
}
bool Utils::get_quiet()
{
	return Utils::QUIET;
}
void Utils::set_delay(int delay)
{
	Utils::MILISECONDS_PRINT_DELAY = delay;
}