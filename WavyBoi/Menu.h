#pragma once
#include <pch.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include <AnimationManager.h>
#include <GUISettings.h>

extern GUISettings gui;

struct MenuOption {
	MenuOption(std::string new_name, void (AnimationManager::*newFunc)(), bool new_enabled, bool new_has_submenu = false) : name(new_name), clickFunc(newFunc), enabled(new_enabled), has_submenu(new_has_submenu) {}
	std::string name;
	void (AnimationManager::*clickFunc)();
	bool enabled;
	bool has_submenu;
};

enum class MENU_TYPE {
	FILE,
	EDIT,
	NEW,
	DISPLAY,
	COMPARATOR,
	OPERATOR
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
	std::vector<MenuOption> menu_options;
	std::vector<Menu *> submenus;
public:
	Menu();
	MENU_TYPE getType();
	void toggleOption(int);
	bool getOptionEnabled(int);
	void initialize(MENU_TYPE,sf::Vector2i);
	void draw(sf::RenderTarget&, sf::RenderStates);
	void update(sf::Vector2i);
	unsigned int get_width();
	bool processLeftClick(sf::Vector2i,AnimationManager *);
};

