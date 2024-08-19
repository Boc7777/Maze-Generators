#include "Board.h"
#include <iostream>
#include "Cell.h"

using namespace std;

//enum Direction {
//	Left,
//	Right,
//	Top,
//	Bottom
//};

Board::Board(float cell_size, int board_width, int board_height, RenderWindow* window) {
	this->cell_size = cell_size;
	this->board_width = board_width;
	this->board_height = board_height;
	this->window = window;

	

	for (int y = 0;y < board_height;y++) {
		vector<Cell> row;
		for (int x = 0; x < board_width;x++) {
			row.push_back(Cell(x,y,cell_size));
		}
		cell_tab.push_back(row);
		row.clear();
	}
	srand(time(0));
	int x = (rand() % (board_width-2))+2;
	int y = (rand() % (board_height-2))+2;
	if (x % 2 == 0) {
		x -= 1;
	}
	if (y % 2 == 0) {
		y -= 1;
	}

	cell_tab[y][x].setStatus(Drawed);

	head_hak = &cell_tab[y][x];
}

void Board::DrawBoard() {
	for (auto& cell_row : cell_tab) {
		for (auto& cell : cell_row) {
			window->draw(cell);
		}
	}
}

bool Board::getGeneratingStatus() {
	return generating;
}


//universal functions 
vector<Direction> Board::Check_Drawed_Around(int x, int y) {
	vector<Direction> tab;
	//lewo
	if (x > 1) {
		if (cell_tab[y][x - 2].getStatus() == Drawed) {
			tab.push_back(Left);
		}
	}
	//prawo
	if (x < board_width - 2) {
		if (cell_tab[y][x + 2].getStatus() == Drawed) {
			tab.push_back(Right);
		}
	}
	//gora
	if (y > 1) {
		if (cell_tab[y - 2][x].getStatus() == Drawed) {
			tab.push_back(Top);
		}
	}
	//dol
	if (y < board_height - 2) {
		if (cell_tab[y + 2][x].getStatus() == Drawed) {
			tab.push_back(Bottom);
		}
	}

	return tab;
}

vector<Direction> Board::Check_Space_around(int x, int y) {
	vector<Direction> tab;
	//lewo
	if (x > 1) {
		if (cell_tab[y][x - 2].getStatus() == Other) {
			tab.push_back(Left);
		}
	}
	//prawo
	if (x < board_width - 2) {
		if (cell_tab[y][x + 2].getStatus() == Other) {
			tab.push_back(Right);
		}
	}
	//gora
	if (y > 1) {
		if (cell_tab[y - 2][x].getStatus() == Other) {
			tab.push_back(Top);
		}
	}
	//dol
	if (y < board_height - 2) {
		if (cell_tab[y + 2][x].getStatus() == Other) {
			tab.push_back(Bottom);
		}
	}

	return tab;
}

void Board::Build_Bridge(int x, int y, Direction dir) {
	if (dir == Left) {
		cell_tab[y][x - 1].setStatus(Drawed);
		cell_tab[y][x - 2].setStatus(Drawed);
		Update_Head(&cell_tab[y][x - 2]);
	}
	else if (dir == Right) {
		cell_tab[y][x + 1].setStatus(Drawed);
		cell_tab[y][x + 2].setStatus(Drawed);
		Update_Head(&cell_tab[y][x + 2]);
	}
	else if (dir == Top) {
		cell_tab[y - 1][x].setStatus(Drawed);
		cell_tab[y - 2][x].setStatus(Drawed);
		Update_Head(&cell_tab[y - 2][x]);
	}
	else if (dir == Bottom) {
		cell_tab[y + 1][x].setStatus(Drawed);
		cell_tab[y + 2][x].setStatus(Drawed);
		Update_Head(&cell_tab[y + 2][x]);
	}
}

void Board::Update_Head(Cell* cell) {
	head_hak = cell;
}




//Prim
void Board::Prim_CreateMaze(int cells_in_iteration) {

	Refresh_Neighbour_Cells();
	vector<int> random_tab;
	for (int i = 0;i < cells_in_iteration;i++) {

		bool choosen_exist = true;
		int random;

		if (neighbours_cells.size() - random_tab.size() > 0) {

			while (choosen_exist) {
				random = rand() % neighbours_cells.size();
				bool checker = false;
				for (auto& number : random_tab) {
					if (random == number) {
						checker = true;
					}
				}
				if (!checker) {
					choosen_exist = false;
					random_tab.push_back(random);
				}
			}

			neighbours_cells[random]->setStatus(Drawed);
			int cell_x = neighbours_cells[random]->getCoords().first;
			int cell_y = neighbours_cells[random]->getCoords().second;

			vector<Direction> Dir_tab = Check_Drawed_Around(cell_x, cell_y);
			int randomDir = rand() % Dir_tab.size();
			Direction Choosen_Dir = Dir_tab[randomDir];

			Build_Bridge(cell_x, cell_y, Choosen_Dir);
		}
	}

	if (neighbours_cells.size() == 0) {
		generating = false;
	}
}

void Board::Refresh_Neighbour_Cells() {
	neighbours_cells.clear();
	for (auto& cell_row : cell_tab) {
		for (auto& cell : cell_row) {
			
			int x = cell.getCoords().first;
			int y = cell.getCoords().second;
			if (cell_tab[y][x].getStatus() != Drawed) {
				if (x % 2 == 1 && y % 2 == 1) {
					if (Check_Drawed_Around(x, y).size() > 0 && (cell_tab[y][x].getStatus() == Other || cell_tab[y][x].getStatus() == Neighbour)) {
						cell.setStatus(Neighbour);
						neighbours_cells.push_back(&cell);
					}
				}
			}
		}
	}
}




//Hunt and Kill
void Board::Hunt_and_Kill_CreateMaze() {
	int cell_x = head_hak->getCoords().first;
	int cell_y = head_hak->getCoords().second;
	vector<Direction> Dir_tab = Check_Space_around(cell_x, cell_y);

	if (Dir_tab.size() > 0) {
		int randomDir = rand() % Dir_tab.size();
		Direction Choosen_Dir = Dir_tab[randomDir];

		
		Build_Bridge(cell_x, cell_y, Choosen_Dir);
	}
	else {
		Find_Head();
	}
}

void Board::Find_Head() {
	bool found = false;
	for (auto& cell_row : cell_tab) {
		for (auto& cell : cell_row) {
			if (!found) {
				int cell_x = cell.getCoords().first;
				int cell_y = cell.getCoords().second;
				vector<Direction> Dir_tab = Check_Drawed_Around(cell_x, cell_y);
				if (Dir_tab.size() > 0 && cell_x % 2 == 1 && cell_y % 2 == 1 && cell_tab[cell_y][cell_x].getStatus() == Other) {

					/*cell_tab[cell_y][cell_x].setStatus(Drawed);*/

					Update_Head(&cell);

					//connect to main branch
					int randomDir = rand() % Dir_tab.size();
					Direction Choosen_Dir = Dir_tab[randomDir];

					Build_Bridge(cell_x, cell_y, Choosen_Dir);

					found = true;
				}
			}
		}
	}
	if (!found) {
		generating = false;
	}
}



//Recursive Backtracking
void Board::Recursive_Backtracking_CreateMaze() {
	int cell_x = head_hak->getCoords().first;
	int cell_y = head_hak->getCoords().second;
	vector<Direction> Dir_tab = Check_Space_around(cell_x, cell_y);

	if (Dir_tab.size() > 0) {
		int randomDir = rand() % Dir_tab.size();
		Direction Choosen_Dir = Dir_tab[randomDir];
		

	
		if (Choosen_Dir == Left) {
			cell_tab[cell_y][cell_x - 1].setStatus(Neighbour);
			cell_tab[cell_y][cell_x - 2].setStatus(Neighbour);

			neighbours_cells.push_back(&cell_tab[cell_y][cell_x - 1]);
			neighbours_cells.push_back(&cell_tab[cell_y][cell_x - 2]);

			Update_Head(&cell_tab[cell_y][cell_x - 2]);
		}
		else if (Choosen_Dir == Right) {
			cell_tab[cell_y][cell_x + 1].setStatus(Neighbour);
			cell_tab[cell_y][cell_x + 2].setStatus(Neighbour);

			neighbours_cells.push_back(&cell_tab[cell_y][cell_x + 1]);
			neighbours_cells.push_back(&cell_tab[cell_y][cell_x + 2]);

			Update_Head(&cell_tab[cell_y][cell_x + 2]);
		}
		else if (Choosen_Dir == Top) {
			cell_tab[cell_y - 1][cell_x].setStatus(Neighbour);
			cell_tab[cell_y - 2][cell_x].setStatus(Neighbour);

			neighbours_cells.push_back(&cell_tab[cell_y - 1][cell_x]);
			neighbours_cells.push_back(&cell_tab[cell_y - 2][cell_x]);

			Update_Head(&cell_tab[cell_y - 2][cell_x]);
		}
		else if (Choosen_Dir == Bottom) {
			cell_tab[cell_y + 1][cell_x].setStatus(Neighbour);
			cell_tab[cell_y + 2][cell_x].setStatus(Neighbour);

			neighbours_cells.push_back(&cell_tab[cell_y + 1][cell_x]);
			neighbours_cells.push_back(&cell_tab[cell_y + 2][cell_x]);

			Update_Head(&cell_tab[cell_y + 2][cell_x]);
		}

		
	}

	else {
		if (neighbours_cells.size() > 0) {
			Cell* last_cell = neighbours_cells[neighbours_cells.size() - 1];
			Cell* last_bridge = neighbours_cells[neighbours_cells.size() - 2];

			head_hak->setStatus(Drawed);
			last_cell->setStatus(Drawed);
			last_bridge->setStatus(Drawed);


			if (Check_Space_around(last_cell->getCoords().first, last_cell->getCoords().second).size() > 0) {
				Update_Head(last_cell);
			}
			else {
				neighbours_cells.pop_back();
				neighbours_cells.pop_back();
			}
		}
		else {
			generating = false;
		}

	}
}