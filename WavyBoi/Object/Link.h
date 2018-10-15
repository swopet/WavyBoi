#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <Object/Object.h>
#include <Object/Parameter.h>
#include <Object/Link.h>

class Link : public Object {
private:
	Object * in;
	Object * out; //out will only ever be NULL if the link is going into the display
	int out_ind = 0;
	Parameter * parameter = NULL;
	sf::Vector2f in_pos;
	sf::Vector2f out_pos;
public:
	Link(Object *, Object *, Parameter *);
	Link();
	~Link();
	void update();
	Parameter * getParameterFromLink();
	bool checkOverlap(sf::RectangleShape);
	void draw(sf::RenderTarget&, sf::RenderStates);
	void setOutInd(int);
	int getOutInd();
	ClickResponse processLeftClick(sf::Vector2i);
	Object * getInObj();
	Object * getOutObj();
	void setOutPos(sf::Vector2f);
	bool setOutObject(Object *);
};