#pragma once
#include "Object/Object.h"
#include "Object/Parameter.h"

class Link : public Object {
private:
	Object * in;
	Object * out;
	Parameter * param;
public:
	Link();
	~Link();
	void draw(sf::RenderTarget&, sf::RenderStates);
	ClickResponse processLeftClick(sf::Vector2i);
};