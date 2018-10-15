#pragma once
#include <SFML/Graphics.hpp>
constexpr auto TEXTURE_PATH = "C:/Users/Trevor/Stuff/VS/WavyBoi/resources/textures/";

struct GUISettings {
	sf::Texture add_20x20_tex;
	sf::Texture delete_20x20_tex;
	sf::Font font;
	int menu_text_height;
	int outline_thickness;
	sf::Color menu_outline_color;
	sf::Color menu_fill_color;
	sf::Color menu_selected_fill_color;
	sf::Color obj_outline_color;
	sf::Color obj_fill_color;
	int obj_circle_radius;
};

extern GUISettings gui;

extern void loadGUISettings();