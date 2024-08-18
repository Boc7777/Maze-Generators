#include <iostream>
#include "SFML/Graphics.hpp"
#include "Board.h"


using namespace std; 
using namespace sf;


const float cell_size = 5.f;
const int width_in_cells = 50;
const int height_in_cells = 50;


unsigned display_width = cell_size * width_in_cells;
unsigned display_height = cell_size * height_in_cells;

int main()
{
    RenderWindow window(VideoMode(display_width, display_height), "Maze");
    /*window.setFramerateLimit(10);*/
    
    Event event;
    Board board(cell_size, width_in_cells, height_in_cells, &window);

    Clock maze_clock;

    while (window.isOpen()) {
        window.clear(Color::Black);
        Time time = maze_clock.getElapsedTime();


        if (board.getGeneratingStatus()) {
            
            //ALGORITHMS
            /*board.Prim_CreateMaze(5);*/
            board.Hunt_and_Kill_CreateMaze();

            //TIMER
            cout << time.asMilliseconds() / 1000.0f << endl;
        }
        else {

        }
        board.DrawBoard();
       
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                window.close();
            }
        }
        window.display();
    }
}


