#pragma once
#include <SFML/Graphics.hpp>

using namespace sf; 
using namespace std;

enum Status {
	Drawed,
	Neighbour,
	Other,
};

class Cell :public Drawable
{
public:
	Cell(float x, float y , float size);

	Status getStatus();
	void setStatus(Status status);

	pair<int, int> getCoords();

private: 
	Status status;
	RectangleShape shape;
	void draw(RenderTarget& target, RenderStates state) const override;
	
	int x;
	int y;
};

