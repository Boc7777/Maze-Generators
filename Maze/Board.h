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
	Bottom,
	Nothing
};

class Board
{
public: 
	Board(float cell_size, int board_width, int board_height, RenderWindow* window);
	void DrawBoard();
	bool getGeneratingStatus();


	void Prim_CreateMaze(int cells_in_iteration);
	void Hunt_and_Kill_CreateMaze();
	void Recursive_Backtracking_CreateMaze();
	void Wilson_CreateMaze();
	

private:

	//universal functions
	vector<Direction> Check_Drawed_Around(int x, int y);
	vector<Direction> Check_Space_around(int x, int y);
	void Build_Bridge(int x, int y, Direction dir);
	void Update_Head(Cell* cell);
	bool Check_End_Generating();


	//Prim algorythym
	void Refresh_Neighbour_Cells();

	//Hunt and Kill algorythym 
	void Find_Head();

	//Recursive_Backtracking


	//Wilson
	void Random_Path_Generate();
	vector<Direction> Check_Random_Directions(int x, int y);
	void ClearNeighbour();

	

	vector<vector<Cell>> cell_tab;
	vector<Cell*> neighbours_cells;

	RenderWindow* window;
	bool generating = true;
	float cell_size;
	int board_width;
	int board_height;

	//Hunt and Kill 
	Cell* head_hak;

	//Wilson
	vector<vector<Direction>> Directions_Map;
	pair<int, int> head_wil = make_pair(-1,-1);
	pair<int,int> tail_wil = make_pair(-1,-1);
	bool HeadFoundMaze = false;
	
};

