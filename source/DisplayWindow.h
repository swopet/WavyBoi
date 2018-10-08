#pragma once
/* DisplayWindow draws the final rendering to
the second monitor/projector or whatever. */

#include <SFML/Graphics.hpp>
#include "AnimationManager.h"

class DisplayWindow {
private:
	sf::RenderWindow * window;
public:
	DisplayWindow();
	DisplayWindow(AnimationManager *);
	bool update(AnimationManager *);
	void close();
};