#pragma once
#include <SFML/Graphics.hpp>
#include <experimental/filesystem>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string.h>
#include <iostream>


class Preferences
{
public:
	sf::Vector2u control_resolution,
		display_resolution;
	sf::Vector2i control_position,
		display_position;
	void loadConfigFromFile();
    void setControlResolution(sf::Vector2u);
    void setControlPosition(sf::Vector2i);
    void setDisplayResolution(sf::Vector2u);
    void setDisplayPosition(sf::Vector2i);
	Preferences();
	~Preferences();
	void writeNewDefaults();
};


sf::Vector2i getIntPairFromString(std::string pair);
sf::Vector2u getUnsignedIntPairFromString(std::string pair);
