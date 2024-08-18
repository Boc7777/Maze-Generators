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
	void Prim_CreateMaze();


private: 
	//Prim algorythym
	vector<Direction> Check_Count_Neighbours(int x, int y);
	void Refresh_Neighbour_Cells();
	//End Prim 

	vector<vector<Cell>> cell_tab;
	vector<Cell*> neighbours_cells;

	RenderWindow* window;
	float cell_size;
	int board_width;
	int board_height;
};

