#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <Object/Object.h>
#include <Object/Parameter.h>
#include <Object/Link.h>
constexpr auto LINK_LINE_THICKNESS = 2;

class Link : public Object {
private:
	Object * in;
	Object * out; //out will only ever be NULL if the link is going into the display
	Parameter * parameter;
	sf::Vector2f in_pos;
	sf::Vector2f out_pos;
public:
	Link(Object *, Object *, Parameter *);
	Link();
	~Link();
	void update();
	Parameter * getParameterFromLink();
	void draw(sf::RenderTarget&, sf::RenderStates);
	ClickResponse processLeftClick(sf::Vector2i);
	void setOutPos(sf::Vector2f);
	bool setOutObject(Object *);
};