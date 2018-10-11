#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Object/Object.h>
#include <sfeMovie/Movie.hpp>
#include <Object\Parameter.h>
constexpr auto VIDEO_OUTLINE_THICKNESS = 2;
constexpr auto VIDEO_CIRCLE_RADIUS = 4;

class Video : public Object {
private:
	sf::Vector2f size;
	sf::RectangleShape main_box;
	sf::RectangleShape video_box;
	sf::CircleShape left_circle;
	sf::CircleShape right_circle;
	void init();
	sfe::Movie * movie;
public:
	Video();
	param getVal();
	Parameter * getNewParameter();
	void update();
	void draw(sf::RenderTarget&, sf::RenderStates);
	void loadFromFile(std::string);
	ClickResponse processLeftClick(sf::Vector2i);
};