#include <iostream>
#include "SFML/Graphics.hpp"
#include "Board.h"


using namespace std; 
using namespace sf;


const float cell_size = 1.f;
const int width_in_cells = 1000;
const int height_in_cells = 1000;


unsigned display_width = cell_size * width_in_cells;
unsigned display_height = cell_size * height_in_cells;

int main()
{
    RenderWindow window(VideoMode(display_width, display_height), "Maze");
    window.setFramerateLimit(30);
    
    Event event;
    Board board(cell_size, width_in_cells, height_in_cells, &window);

    Clock maze_clock;

    Time time;

    while (window.isOpen()) {
        window.clear(Color::Black);
        /*maze_clock.restart();*/
        

        if (board.getGeneratingStatus()) {
            
            

            //ALGORITHMS
            
            //board.Prim_CreateMaze(10);
            //board.Hunt_and_Kill_CreateMaze();
            board.Depth_First_search_CreateMaze();
            //board.Wilson_CreateMaze();
            //board.Kruskal_CreateMaze();
            //board.Eller_CreateMaze();

            //TIMER
            time = maze_clock.getElapsedTime();
            cout << time.asMilliseconds() / 1000.0f << endl;
           
        }
        

        
       
        


        board.DrawBoard();
        //Kruskal && Eller Sets
        //board.DrawSets();
       
        

        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                window.close();
            }
        }
        window.display();
    }
}


