// BattleshipGame.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "BoardClass.h"
#include "IBattleshipGameAlgo.h"
#include "BattleshipPlayerFromFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>

bool is_valid_dir_path(char *pathname)
{
	struct stat info;

	if ((stat(pathname, &info) != 0) || (!info.st_mode & S_IFDIR))
		return false;
	return true;
}
/*Main is for running games etc...*/
int main(int argc, char *argv[])
{
	bool find_board = false, find_a = false, find_b = false;
	string file_board, file_a, file_b;
	const char **segmentationFault = nullptr; //TODO: need to make a board. this one is not good obviously.
	Board brd(10, 10);
	/* reading file names in the path or current directory to file_names.txt */
	if (argc == 2)
	{
		if (!is_valid_dir_path(argv[1]))
		{
			cout << "Wrong path: " << argv[1] << endl;
			return -1;
		}
		
		string const command = string ("dir ") + argv[1] + string(" /b /a-d > file_names.txt 2>&1");
		system(command.c_str());
	}
	else if (argc == 1)
	{
		system("dir /b /a-d > file_names.txt 2>&1");
	}
	else
	{
		cout << "Wrong number of arguments" << endl;
	}
	
	/* reading file_name.txt and validate file types */
	std::ifstream infile("file_names.txt");
	std::string line;
	while (std::getline(infile, line))
	{
		if (line.substr(line.find_last_of(".") + 1) == "sboard")
		{
			find_board = true;
			file_board = line;
		}
		if (line.substr(line.find_last_of(".") + 1) == "attack-a")
		{
			find_a = true;
			file_a = line;
		}
		if (line.substr(line.find_last_of(".") + 1) == "attack-b")
		{
			find_b = true;
			file_b = line;
		}
	}
	
	string path = (argc == 1) ? "Working Directory" : argv[1];
	if(!find_board)
		cout << "Missing board file (*.sboard) looking in path: " << path << endl;
	if (!find_a)
		cout << "Missing attack file for player A (*.attack-a) looking in path: " << path << endl;
	if (!find_b)
		cout << "Missing attack file for player B (*.attack-b) looking in path: " << path << endl;

	if (find_board && find_a && find_b)
	{
		// set board using file
		if(argc == 1)
			brd.SetBoardFromFile((file_board).c_str());
		else
			brd.SetBoardFromFile((string(argv[1]) + "\\" + file_board).c_str());
		// print board //TODO: delete it
		cout << brd << endl;

		// TODO: check board validity
	}
	else
		return -1;


	BattleshipPlayerFromFile playerA = BattleshipPlayerFromFile(0, string("TestFilesV3\\moves\\dirty_ilegal_movesA.attack-a")); //clean_movesA.attack-a"));
	playerA.setBoard(segmentationFault, 10, 10);
	return 0;
}

