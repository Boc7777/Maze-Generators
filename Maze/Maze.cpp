#include <iostream>
#include "SFML/Graphics.hpp"
#include "Board.h"


using namespace std; 
using namespace sf;


const float cell_size = 24.f;
const int width_in_cells = 39;
const int height_in_cells = 39;


unsigned display_width = cell_size * width_in_cells;
unsigned display_height = cell_size * height_in_cells;

int main()
{
    RenderWindow window(VideoMode(display_width, display_height), "Maze");
    //window.setFramerateLimit(2);
    
    Event event;
    Board board(cell_size, width_in_cells, height_in_cells, &window);

    Clock maze_clock;

    while (window.isOpen()) {
        /*window.clear(Color::Black);*/
        Time time = maze_clock.getElapsedTime();


        if (board.getGeneratingStatus()) {
            
            //ALGORITHMS
            // 
            //board.Prim_CreateMaze(1);
            //board.Hunt_and_Kill_CreateMaze();
            //board.Recursive_Backtracking_CreateMaze();
            //board.Wilson_CreateMaze();
            board.Kruskal_CreateMaze();

            //TIMER
            cout << time.asMilliseconds() / 1000.0f << endl;
        }
        else {

        }
        board.DrawBoard();
        //Kruskal Sets
        board.DrawSets();
       

        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                window.close();
            }
        }
        window.display();
    }
}


