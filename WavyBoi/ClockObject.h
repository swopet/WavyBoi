#pragma once
#include "Object/Object.h"
#include <SFML/OpenGL.hpp>

class ClockObject :
	public Object
{
private:
	sf::RectangleShape clock_rect;
	sf::RectangleShape s_ms_rect;
	sf::RectangleShape play_pause_rect;
	sf::RectangleShape zero_s_rect;
	sf::Clock clock;
	sf::Time curr_time;
	sf::Time last_time;
	bool seconds;
	bool playing;
public:
	sf::Vector2f getRightPos();
	void draw(sf::RenderTarget&, sf::RenderStates);
	void update();
	bool checkOverlap(sf::RectangleShape select_box);
	ClickResponse processLeftClick(sf::Vector2i vec);
	Parameter getParameter();
	ClockObject();
	~ClockObject();
};

