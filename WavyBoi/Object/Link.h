#pragma once
#include <Object/Object.h>
#include <Object/Parameter.h>
#include <Object/Link.h>

class Link : public Object {
private:
	Object * in;
	Object * out; //out will only ever be NULL if the link is going into the display
	Parameter * parameter;
public:
	Link(Object *, Object *, Parameter *);
	Link();
	~Link();
	void update();
	Parameter * getParameterFromLink();
	void draw(sf::RenderTarget&, sf::RenderStates);
	ClickResponse processLeftClick(sf::Vector2i);
};