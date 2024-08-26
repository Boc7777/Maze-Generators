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
	//Kruskal && Eller Sets
	void DrawSets();

	bool getGeneratingStatus();

	void Prim_CreateMaze(int cells_in_iteration);
	void Hunt_and_Kill_CreateMaze();
	void Recursive_Backtracking_CreateMaze();
	void Wilson_CreateMaze();
	void Kruskal_CreateMaze();
	void Eller_CreateMaze();
	

private:

	//universal functions
	vector<Direction> Check_Drawed_Around(int x, int y);
	vector<Direction> Check_Space_around(int x, int y);
	void Build_Bridge(int x, int y, Direction dir);
	void Update_Head(Cell* cell);
	bool Check_End_Generating();
	void Draw_Random_Cell();


	//Prim algorythym
	void Refresh_Neighbour_Cells();

	//Hunt and Kill algorythym 
	void Find_Head();

	//Wilson
		void Random_Path_Generate();
		vector<Direction> Check_Random_Directions(int x, int y);
		void ClearNeighbour();

	//Kruskal
		void Build_Bridge_Kruskal(int x,int y);
		bool Should_end_generating_Kruskal();
		void Recursion_Changing_set(int x, int y, string winner);

	//Kruskal && Eller
		void Build_Sets_tab();
		string Get_String_from_sets(int x, int y);

	//Eller
		void Fill_Line(int y);
		void Mix_Line(int y);
		void Drop_Line(int y);
		

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

	//Kruskal
	vector<Cell*> walls_tab;
	int lastset = 0;

	//Kruskal && Eller
	Font font;
	vector<vector<Text>> sets_tab;

	//Eller
	int current_y = 1;
	int lastset_e = 1;
	


	
};

