#include "Board.h"
#include <iostream>
#include "Cell.h"
#include <string>

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

	font.loadFromFile("C:\\Users\\48664\\Desktop\\Maze\\Maze\\czcionka.ttf");
	
	for (int y = 0;y < board_height;y++) {
		vector<Cell> row;
		for (int x = 0; x < board_width;x++) {
			row.push_back(Cell(x,y,cell_size));
		}
		cell_tab.push_back(row);
		row.clear();
	}

	Build_Sets_tab();

	srand(time(0));

	Draw_Random_Cell();
	/*for (int y = 0;y < board_height;y++) {
		for (int x = 0; x < board_width;x++) {
			if (x % 2 == 1 && y % 2 == 1) {
				cell_tab[y][x].setStatus(Drawed);
			}
		}
	}*/
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

bool Board::Check_End_Generating() {
	for (int y = 0;y < board_height;y++) {
		for (int x = 0;x < board_width;x++) {
			if (cell_tab[y][x].getStatus() == Other && x%2==1&&y%2==1) {
				return false;
			}
		}
	}
	return true;
}

void Board::Draw_Random_Cell() {
	int x = (rand() % (board_width - 2)) + 2;
	int y = (rand() % (board_height - 2)) + 2;
	if (x % 2 == 0) {
		x -= 1;
	}
	if (y % 2 == 0) {
		y -= 1;
	}

	cell_tab[y][x].setStatus(Drawed);

	head_hak = &cell_tab[y][x];
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




//Wilson
void Board::Wilson_CreateMaze() {
	//is empty
	if (!HeadFoundMaze){
		Random_Path_Generate();

	}
	else{
		

		Direction Dir_tail = Directions_Map[tail_wil.second][tail_wil.first];
		int tail_x = tail_wil.first;
		int tail_y = tail_wil.second;

		if (tail_x == head_wil.first && tail_y == head_wil.second) {
			HeadFoundMaze = false;
			tail_wil = make_pair(-1, -1);
			Directions_Map.clear();
		}


		cell_tab[tail_y][tail_x].setStatus(Drawed);
		Build_Bridge(tail_x, tail_y, Dir_tail);
		if (Dir_tail == Left) {
			tail_wil = make_pair(tail_x - 2, tail_y);
		}
		else if (Dir_tail == Right) {
			tail_wil = make_pair(tail_x + 2, tail_y);
		}
		else if (Dir_tail == Top) {
			tail_wil = make_pair(tail_x, tail_y-2);
		}
		else if (Dir_tail == Bottom) {
			tail_wil = make_pair(tail_x , tail_y+2);
		}
		
	
	
	}
}

void Board::Random_Path_Generate() {
	//tail isn't chosen
	if (tail_wil.first == -1 && tail_wil.second == -1) {
		ClearNeighbour();

		int found = false;
		while (!found) {
			if (Check_End_Generating()) {
				generating = false;
				found = true;
			}
			else {
				int x = rand() % board_width;
				int y = rand() % board_height;
				if (cell_tab[y][x].getStatus() == Other && x % 2 == 1 && y % 2 == 1) {
					found = true;
					tail_wil = make_pair(x, y);
					head_wil = tail_wil;
				}
			}

		}

		for (int y = 0;y < board_height;y++) {
			vector<Direction> row;
			for (int x = 0;x < board_width;x++) {
				row.push_back(Nothing);
			}
			Directions_Map.push_back(row);
		}



	}
	
	//random generating 
	else {

		int x = head_wil.first;
		int y = head_wil.second;

		cell_tab[y][x].setStatus(Head);

		if (cell_tab[y][x].getStatus() != Drawed) {

			vector<Direction> Dir_tab = Check_Random_Directions(x, y);
			int randomDir = rand() % Dir_tab.size();
			Direction Choosen_Dir = Dir_tab[randomDir];

			cell_tab[y][x].setStatus(Neighbour);
			Directions_Map[y][x] = Choosen_Dir;

			int new_x = x;
			int new_y = y;

			if (Choosen_Dir == Left) {
				new_x = x - 2;
				new_y = y;

				cell_tab[y][x - 1].setStatus(Neighbour);
				
			}
			else if (Choosen_Dir == Right) {
				new_x = x + 2;
				new_y = y;

				cell_tab[y][x + 1].setStatus(Neighbour);
				
			}
			else if (Choosen_Dir == Top) {
				new_x = x;
				new_y = y-2;

				cell_tab[y - 1][x].setStatus(Neighbour);
				
			}
			else if (Choosen_Dir == Bottom) {
				new_x = x;
				new_y = y+2;

				cell_tab[y + 1][x].setStatus(Neighbour);

			}


			if (cell_tab[new_y][new_x].getStatus() == Drawed) {
				HeadFoundMaze = true;
			}
			cell_tab[new_y][new_x].setStatus(Head);
			head_wil = make_pair(new_x, new_y);

		}
	}

}

vector<Direction> Board::Check_Random_Directions(int x, int y) {
	vector<Direction> tab;
	//lewo
	if (x > 1) {
			tab.push_back(Left);
	}
	//prawo
	if (x < board_width - 2) {
			tab.push_back(Right);
	}
	//gora
	if (y > 1) {	
			tab.push_back(Top);
	}
	//dol
	if (y < board_height - 2) {
			tab.push_back(Bottom);
	}

	return tab;
}

void Board::ClearNeighbour() {
	for (int y = 0;y < board_height;y++) {
		for (int x = 0;x < board_width;x++) {
			if (cell_tab[y][x].getStatus() == Neighbour) {
				cell_tab[y][x].setStatus(Other);
			}

		}
	}
}




//Kruskal
void Board::Kruskal_CreateMaze() {
	if (walls_tab.size() > 0) {
		int random = rand() % walls_tab.size();

		int x = walls_tab[random]->getCoords().first;
		int y = walls_tab[random]->getCoords().second;

		Build_Bridge_Kruskal(x, y);

		walls_tab.erase(walls_tab.begin() + random);
	}
	Should_end_generating_Kruskal();
	
}

void Board::Build_Bridge_Kruskal(int x, int y) {
	//vertically
	if (y % 2 == 0) {
		if (Get_String_from_sets(x, y - 1) == "" && Get_String_from_sets(x, y + 1) == "") {
			cell_tab[y][x].setStatus(Drawed);
			cell_tab[y-1][x].setStatus(Drawed);
			cell_tab[y+1][x].setStatus(Drawed);

			lastset += 1;
			sets_tab[y][x].setString(to_string(lastset));
			sets_tab[y-1][x].setString(to_string(lastset));
			sets_tab[y+1][x].setString(to_string(lastset));
		}

		else if (Get_String_from_sets(x, y - 1) == "" || Get_String_from_sets(x, y + 1) == "") {
			cell_tab[y][x].setStatus(Drawed);
			cell_tab[y - 1][x].setStatus(Drawed);
			cell_tab[y + 1][x].setStatus(Drawed);

			string str;
			if (Get_String_from_sets(x, y - 1) == "") {
				str = Get_String_from_sets(x, y + 1);
			}
			else {
				str = Get_String_from_sets(x, y - 1);
			}
			
			sets_tab[y][x].setString(str);
			sets_tab[y - 1][x].setString(str);
			sets_tab[y + 1][x].setString(str);
		}

		else if (Get_String_from_sets(x, y - 1) != Get_String_from_sets(x, y + 1)) {
			string winner = (stoi(Get_String_from_sets(x, y - 1)) < stoi(Get_String_from_sets(x, y + 1)) ? Get_String_from_sets(x, y - 1) : Get_String_from_sets(x, y + 1));
			
			sets_tab[y][x].setString("*");
			cell_tab[y][x].setStatus(Drawed);

			Recursion_Changing_set(x, y, winner);
		}

		
	}
	//horizontally
	else {
		if (Get_String_from_sets(x-1, y) == "" && Get_String_from_sets(x+1, y) == "") {
			cell_tab[y][x].setStatus(Drawed);
			cell_tab[y][x+1].setStatus(Drawed);
			cell_tab[y][x-1].setStatus(Drawed);

			lastset += 1;
			sets_tab[y][x].setString(to_string(lastset));
			sets_tab[y][x+1].setString(to_string(lastset));
			sets_tab[y][x-1].setString(to_string(lastset));
		}

		else if (Get_String_from_sets(x-1, y) == "" || Get_String_from_sets(x+1, y) == ""){
			cell_tab[y][x].setStatus(Drawed);
			cell_tab[y][x-1].setStatus(Drawed);
			cell_tab[y][x+1].setStatus(Drawed);

			string str;
			if (Get_String_from_sets(x+1, y) == "") {
				str = Get_String_from_sets(x-1, y);
			}
			else {
				str = Get_String_from_sets(x+1, y);
			}

			sets_tab[y][x].setString(str);
			sets_tab[y][x-1].setString(str);
			sets_tab[y][x+1].setString(str);
		}

		else if (Get_String_from_sets(x-1, y) != Get_String_from_sets(x+1, y)){
			string winner = (stoi(Get_String_from_sets(x-1, y)) < stoi(Get_String_from_sets(x+1, y)) ? Get_String_from_sets(x-1, y) : Get_String_from_sets(x+1, y));

			sets_tab[y][x].setString("*");
			cell_tab[y][x].setStatus(Drawed);

			Recursion_Changing_set(x, y, winner);
		}
		
	}
}

bool Board::Should_end_generating_Kruskal() {
	string set = Get_String_from_sets(1, 1);
	for (int y = 0;y < board_height;y++) {
		for (int x = 0;x < board_width;x++) {
			if (y % 2 == 1 && x % 2 == 1) {
				if (Get_String_from_sets(x, y) == "" || Get_String_from_sets(x,y) != set) {
					return false;
				}
			}
		}
	}
	generating = false;
	return true;
	
}

void Board::Recursion_Changing_set(int x, int y, string winner) {
	if (Get_String_from_sets(x, y) != "" && Get_String_from_sets(x, y) != winner) {
		sets_tab[y][x].setString(winner);

		if (x - 1 > 0) {
			if (Get_String_from_sets(x - 1, y) != "" && Get_String_from_sets(x - 1, y) != winner) {
				Recursion_Changing_set(x - 1, y, winner);
			}
		}
		if (x + 1 < board_width) {
			if (Get_String_from_sets(x + 1, y) != "" && Get_String_from_sets(x + 1, y) != winner) {
				Recursion_Changing_set(x + 1, y, winner);
			}
		}
		if (y - 1 > 0) {
			if (Get_String_from_sets(x, y - 1) != "" && Get_String_from_sets(x, y - 1) != winner) {
				Recursion_Changing_set(x, y - 1, winner);
			}
		}
		if (y + 1 < board_height) {
			if (Get_String_from_sets(x, y + 1) != "" && Get_String_from_sets(x, y + 1) != winner) {
				Recursion_Changing_set(x, y + 1, winner);
			}
		}
	}
}




//Kruskal && Eller Sets
void Board::DrawSets() {
	if (generating) {
		for (auto& set_row : sets_tab) {
			for (auto& set : set_row) {
				set.setOrigin(set.getLocalBounds().width / 2, set.getLocalBounds().height / 2);
				window->draw(set);
			}
		}
	}
}

void Board::Build_Sets_tab() {
	//creating sets_tab  = empty texts
	for (int y = 0;y < board_height;y++) {
		vector<Text> row;
		for (int x = 0;x < board_width;x++) {
			Text text;
			text.setFont(font);
			text.setCharacterSize(cell_size / 1.4f);
			text.setFillColor(Color::Red);
			text.setPosition(Vector2f(x * cell_size + (0.5 * cell_size), y * cell_size + (0.5 * cell_size)));

			text.setString("");
			row.push_back(text);
		}
		sets_tab.push_back(row);
	}

	//create list of all walls
	int a = 0;
	int b = 0;
	if (board_width % 2 == 1) {
		a = 1;
	}
	if (board_height % 2 == 1) {
		b = 1;
	}

	for (int y = 0; y < board_height;y++) {
		for (int x = 0;x < board_width;x++) {
			if (((x % 2 == 0 && y % 2 == 1) || (x % 2 == 1 && y % 2 == 0)) && x > 0 && x < board_width - a && y>0 && y < board_height - b) {
				walls_tab.push_back(&cell_tab[y][x]);
			}
		}
	}

}

string Board::Get_String_from_sets(int x, int y) {
	return sets_tab[y][x].getString().toAnsiString();
}




//Eller 
void Board::Eller_CreateMaze() {
	if(current_y < board_height-2) {
		Fill_Line(current_y);
		Mix_Line(current_y);
		Drop_Line(current_y);
	}
	//end
	else if (current_y == board_height - 1 || current_y == board_height - 2) {
		Fill_Line(current_y);
		for (int x = 0; x < board_width;x++) {
			if (x % 2 == 0 && x > 0 && x < board_width - 2) {
				if (Get_String_from_sets(x - 1, current_y) != Get_String_from_sets(x + 1, current_y)) {
					cell_tab[current_y][x].setStatus(Drawed);
				}
			}
		}

		generating = false;
	}
	current_y += 2;
}

void Board::Fill_Line(int y) {
	for (int x = 0; x < board_width;x++) {
		if (cell_tab[y][x].getStatus() == Other && x % 2 == 1) {

			cell_tab[y][x].setStatus(Drawed);
			sets_tab[y][x].setString(to_string(lastset_e));
			lastset_e++;
		}
	}
}

void Board::Mix_Line(int y){
	for (int x = 0; x < board_width;x++) {
		if (x % 2 == 0 && x>0 && x<board_width-1) {
			if (Get_String_from_sets(x - 1, y) != Get_String_from_sets(x + 1, y)) {
				int good = true;
				if (y > 1) {
					if (Get_String_from_sets(x - 1, y - 1) == Get_String_from_sets(x + 1, y - 1) && Get_String_from_sets(x + 1, y - 1) != "") {
						good = false;
					}
				}

				if (good) {
					int random = rand() % 3;
					if (random == 1) {
						cell_tab[y][x].setStatus(Drawed);
						string winner = Get_String_from_sets(x - 1, y);
						sets_tab[y][x].setString(winner);

						int a = x + 1;

						while (a < board_width && cell_tab[y][a].getStatus() == Drawed) {
							sets_tab[y][a].setString(winner);
							a++;
						}


					}
					else if (random == 2) {
						cell_tab[y][x].setStatus(Drawed);
						string winner = Get_String_from_sets(x + 1, y);
						sets_tab[y][x].setString(winner);

						int a = x - 1;
						while (cell_tab[y][a].getStatus() == Drawed && a > 0) {
							sets_tab[y][a].setString(winner);
							a--;
						}
					}
				}


			}
		}
	}
}

void Board::Drop_Line(int y) {
	int random_amount;
	for (int x = 1; x < board_width; x+=2) {
			int lenght = 1;
			int a = x+2;

			while (a<board_width && Get_String_from_sets(a-1,y)!="") {
				lenght++;
				a += 2;
			}
			if (lenght > 3) {
				lenght--;
			}
			if (lenght > 4) {
				lenght--;
			}
			random_amount = 1 + ((rand() % lenght) + 1)/3;


			for (int b = 0; b < random_amount;b+=1) {
				int random_place = rand() % lenght;
				cell_tab[y+1][x + random_place*2].setStatus(Drawed);
				cell_tab[y+2][x + random_place*2].setStatus(Drawed);
				sets_tab[y+1][x + random_place*2].setString(Get_String_from_sets(x, y));
				sets_tab[y+2][x + random_place*2].setString(Get_String_from_sets(x, y));
			}

			x += (lenght-1)*2;
			
		
	}
}