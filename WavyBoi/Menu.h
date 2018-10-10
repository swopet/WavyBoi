#pragma once
#include <pch.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include <AnimationManager.h>
constexpr auto MENU_BORDER_THICKNESS = 2;

struct MenuOption {
	MenuOption(std::string new_name, void (AnimationManager::*newFunc)(), bool new_enabled) : name(new_name), clickFunc(newFunc), enabled(new_enabled) {}
	std::string name;
	void (AnimationManager::*clickFunc)();
	bool enabled;
};

enum class MENU_TYPE {
	FILE,
	EDIT,
	NEW,
	DISPLAY
};

class Menu{
private:
	MENU_TYPE type;
	bool is_open = false;
	std::string name;
	sf::Vector2i pos;
	sf::Text text;
	sf::RectangleShape rect;
	unsigned int height;
	unsigned int menu_options_width;
	sf::Font * font;
	std::vector<MenuOption> menu_options;
public:
	Menu();
	MENU_TYPE getType();
	void toggleOption(int);
	bool getOptionEnabled(int);
	void initialize(MENU_TYPE,sf::Font *,sf::Vector2i,unsigned int);
	void draw(sf::RenderTarget&, sf::RenderStates);
	void update(sf::Vector2i);
	unsigned int get_width();
	bool processLeftClick(sf::Vector2i,AnimationManager *);
};

