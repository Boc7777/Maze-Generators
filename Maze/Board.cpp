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

	/*for (int y = 0;y < board_height;y++) {
		for (int x = 0; x < board_width;x++) {

			if (x % 2 == 1 && y % 2 == 1) {
				cell_tab[y][x].setStatus(Drawed);
			}
		}
	}*/

	cell_tab[5][5].setStatus(Drawed);


}


void Board::DrawBoard() {
	for (auto& cell_row : cell_tab) {
		for (auto& cell : cell_row) {
			window->draw(cell);
		}
	}
}




//Prim's Algorithm
void Board::Prim_CreateMaze() {

	Refresh_Neighbour_Cells();
	vector<int> random_tab;
	for (int i = 0;i < 5;i++) {

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

			vector<Direction> Dir_tab = Check_Count_Neighbours(cell_x, cell_y);
			int randomDir = rand() % Dir_tab.size();
			Direction Choosen_Dir = Dir_tab[randomDir];

			if (Choosen_Dir == Left) {
				cell_tab[cell_y][cell_x - 1].setStatus(Drawed);
			}
			else if (Choosen_Dir == Right) {
				cell_tab[cell_y][cell_x + 1].setStatus(Drawed);
			}
			else if (Choosen_Dir == Top) {
				cell_tab[cell_y - 1][cell_x].setStatus(Drawed);
			}
			else if (Choosen_Dir == Bottom) {
				cell_tab[cell_y + 1][cell_x].setStatus(Drawed);
			}
		}
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
					if (Check_Count_Neighbours(x, y).size() > 0 && (cell_tab[y][x].getStatus() == Other || cell_tab[y][x].getStatus() == Neighbour)) {
						cell.setStatus(Neighbour);
						neighbours_cells.push_back(&cell);
					}
				}
			}
		}
	}
}

vector<Direction> Board::Check_Count_Neighbours(int x, int y) {
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

