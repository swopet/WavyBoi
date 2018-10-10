#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Object/Object.h>
#include <sfeMovie/Movie.hpp>
#define VIDEO_OUTLINE_THICKNESS 2

class Video : public Object {
private:
	sf::Vector2f size;
	sf::RectangleShape main_box;
	sf::RectangleShape video_box;
	void init();
	sfe::Movie * movie;
public:
	Video();
	void draw(sf::RenderTarget&, sf::RenderStates);
	void loadFromFile(std::string);
	ClickResponse processLeftClick(sf::Vector2i);
};