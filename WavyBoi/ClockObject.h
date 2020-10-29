#pragma once
#include "Object/Object.h"
#include <SFML/OpenGL.hpp>

class ClockObject :
	public Object
{
private:
	sf::Clock clock;
	sf::Time curr_time;
	sf::Time last_time;
	bool seconds = true;
	bool playing = false;
public:
	void draw(sf::RenderTarget&, sf::RenderStates);
	void initializeElements();
	void updateGUIElements();
	void update();
	bool checkOverlap(sf::RectangleShape select_box);
    void setPlaying(bool new_playing);
	ClickResponse processLeftClick(sf::Vector2i vec);
	Parameter getParameter();
	ClockObject();
	~ClockObject();
};

