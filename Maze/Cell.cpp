#include "Cell.h"

Cell::Cell(float x, float y, float size) {
	this->shape.setPosition(Vector2f(x*size, y*size));
	this->shape.setSize(Vector2f(size, size));
	this->shape.setFillColor(Color::Cyan);
	this->x = x;
	this->y = y;

	setStatus(Other);
}


Status Cell::getStatus() {
	return this->status;
}


void Cell::setStatus(Status status) {
	this->status = status;
	if (status == Drawed) {
		this->shape.setFillColor(Color::White);
	}
	else if (status == Neighbour) {
		this->shape.setFillColor(Color::Red);
	}
	else if(status == Other){
		this->shape.setFillColor(Color::Black);
	}
	else {
		this->shape.setFillColor(Color::Green);
	}

}

pair<int, int> Cell::getCoords() {
	return make_pair(x,y);
}

void Cell::draw(RenderTarget& target, RenderStates state) const {

	target.draw(shape, state);
}