#pragma once
#include <SFML/Graphics.hpp>
constexpr auto TEXTURE_PATH = "C:/Users/Trevor/Stuff/VS/WavyBoi/resources/textures/";

struct GUISettings {
	sf::Font font;
	int menu_text_height;
	int outline_thickness;
	sf::Color menu_outline_color;
	sf::Color menu_fill_color;
	sf::Color menu_selected_fill_color;
	sf::Color obj_outline_color;
	sf::Color obj_fill_color;
	int obj_circle_radius;
	//TEXTURES
	//MULTIPLEXER
	sf::Texture add_20x20_tex;
	sf::Texture delete_20x20_tex;
	//COMPARATOR
	sf::Texture gt_true_32x32_tex;
	sf::Texture gt_false_32x32_tex;
	sf::Texture gte_true_32x32_tex;
	sf::Texture gte_false_32x32_tex;
	sf::Texture eq_true_32x32_tex;
	sf::Texture eq_false_32x32_tex;
	sf::Texture neq_true_32x32_tex;
	sf::Texture neq_false_32x32_tex;
	sf::Texture lt_true_32x32_tex;
	sf::Texture lt_false_32x32_tex;
	sf::Texture lte_true_32x32_tex;
	sf::Texture lte_false_32x32_tex;
};

extern GUISettings gui;

extern void loadGUISettings();