#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Object/Object.h>
#include <sfeMovie/Movie.hpp>
#include <Object\Parameter.h>
#include <Common.h>

class Video : public Object {
private:
	sf::Vector2f size;
	sf::RectangleShape main_box;
	sf::RectangleShape video_box;
	sf::RectangleShape play_pause_rect;
	sf::RectangleShape stop_rect;
	sf::RectangleShape loop_rect;
	sf::CircleShape left_circle;
	sf::Vector2f left_pos;
	sf::CircleShape right_circle;
	sf::Vector2f right_pos;
	sf::Time last_offset;
	float speed = 1.0;
	void init();
	void togglePlay();
	void stopAndReset();
	sfe::Movie * movie;
	bool playing = false;
	bool loop = false;
public:
	Video();
	Parameter * getNewParameter();
	Parameter getParameter();
	sf::Vector2f getLeftPos(unsigned int);
	sf::Vector2f getRightPos();
	bool checkOverlap(sf::RectangleShape);
	void update();
	void setSpeed(float new_speed);
	void draw(sf::RenderTarget&, sf::RenderStates);
	void loadFromFile(std::string);
	ClickResponse processLeftClick(sf::Vector2i);
	ClickResponse processLeftClickHeld(sf::Vector2i);
	ClickResponse processLeftClickRelease(sf::Vector2i mouse_pos);
};