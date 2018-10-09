#pragma once
#include "Object/Object.h"
#define VIDEO_OUTLINE_THICKNESS 2

class Video : public Object {
private:
	sf::Vector2f size;
	sf::RectangleShape main_box;
	void init();
public:
	Video();
	void draw(sf::RenderTarget&, sf::RenderStates);
	ClickResponse processLeftClick(sf::Vector2i);
};