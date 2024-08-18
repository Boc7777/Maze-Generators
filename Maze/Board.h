#pragma once
#include <SFML/Graphics.hpp>
#include "vector"
#include "Cell.h"

using namespace sf; 
using namespace std;

enum Direction {
	Left,
	Right,
	Top,
	Bottom
};

class Board
{
public: 
	Board(float cell_size, int board_width, int board_height, RenderWindow* window);
	void DrawBoard();
	bool getGeneratingStatus();


	void Prim_CreateMaze(int cells_in_iteration);
	void Hunt_and_Kill_CreateMaze();

private:
	vector<Direction> Check_Count_Neighbours(int x, int y);
	vector<Direction> Check_Space_around(int x, int y);
	void Build_Bridge(int x, int y, Direction dir);


	//Prim algorythym
	void Refresh_Neighbour_Cells();

	//Hunt and Kill algorythym 
	void Find_Head();


	

	vector<vector<Cell>> cell_tab;
	vector<Cell*> neighbours_cells;

	RenderWindow* window;
	bool generating = true;
	float cell_size;
	int board_width;
	int board_height;

	//Hunt and Kill
	Cell* head_hak;
};

