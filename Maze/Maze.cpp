#include <iostream>
#include "SFML/Graphics.hpp"
#include "Board.h"


using namespace std; 
using namespace sf;


const float cell_size = 5.f;
const int width_in_cells = 101;
const int height_in_cells = 101;


unsigned display_width = cell_size * width_in_cells;
unsigned display_height = cell_size * height_in_cells;

int main()
{
    RenderWindow window(VideoMode(display_width, display_height), "Maze");
    /*window.setFramerateLimit(60);*/
    
    Event event;
    Board board(cell_size, width_in_cells, height_in_cells, &window);
 
    while (window.isOpen()) {
        window.clear(Color::Black);

        board.Prim_CreateMaze();
        board.DrawBoard();
       
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                window.close();
            }
        }
        window.display();
    }
}


