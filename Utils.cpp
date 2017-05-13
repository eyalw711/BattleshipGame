#include "Utils.h"
#include "GameManager.h"

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

void Utils::updateBoardPrint(int player_color, COORD hit_coord, char current, int hit_color)
{
	if (!Utils::QUIET)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(MILISECONDS_PRINT_DELAY));
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD cursor_coord = GetConsoleCursorPosition(h);
		gotoxy(PRINT_INDENT + hit_coord.X * 2 - 2, hit_coord.Y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), player_color); cout << "@";
		gotoxy(PRINT_INDENT + hit_coord.X * 2 - 2, hit_coord.Y);
		std::this_thread::sleep_for(std::chrono::milliseconds(DEFAULT_START_ANIMATION_DELAY));
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), player_color); cout << " ";
		gotoxy(PRINT_INDENT + hit_coord.X * 2 - 2, hit_coord.Y);
		std::this_thread::sleep_for(std::chrono::milliseconds(DEFAULT_END_ANIMATION_DELAY));
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), hit_color); cout << current;
		gotoxy(cursor_coord.X, cursor_coord.Y);
		setcolor(WHITE_COLOR);
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

string Utils::find_path(const string& path_expr_to_find)
{
	WIN32_FIND_DATAA fileData;
	HANDLE hFind;
	string retVal = "";
	hFind = FindFirstFileA(path_expr_to_find.c_str(), &fileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			retVal = FILE_NOT_FOUND;
		//else
	    //	retVal = ERROR_FINDING_PATH;
	}

	else
	{
		retVal = fileData.cFileName;
		FindClose(hFind);
	}
	return retVal;
}

string Utils::find_file(const string& path_expr_to_find, int player_id, bool at_least_two)
{
	WIN32_FIND_DATAA fileData;
	HANDLE hFind;
	string first_file, second_file; // lexiogrphic order
	string retVal = "", tmp_str = "";
	hFind = FindFirstFileA(path_expr_to_find.c_str(), &fileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			retVal = FILE_NOT_FOUND;
		//else
		//	retVal = ERROR_FINDING_PATH;
	}

	else
	{
		first_file = fileData.cFileName;
		second_file = fileData.cFileName;
		do
		{
			tmp_str = fileData.cFileName;
			if (first_file.compare(tmp_str) > 0)
			{
				second_file = first_file;
				first_file = tmp_str;
			}
			else if (second_file.compare(tmp_str) > 0 || first_file.compare(second_file) == 0)
			{
				second_file = tmp_str;
			}

		} while (FindNextFileA(hFind, &fileData));
		if (at_least_two && first_file.compare(second_file) == 0) // make sure there exist at least two different files
			return FILE_NOT_FOUND;
		if (player_id == PLAYER_A)
			retVal = first_file;
		else
			retVal = second_file;
		FindClose(hFind);
	}
	return retVal;
}