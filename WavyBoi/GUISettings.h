#pragma once
#include <SFML/Graphics.hpp>
constexpr auto TEXTURE_PATH = "C:/Users/Trevor/Stuff/VS/WavyBoi/resources/textures/";

struct GUISettings {
	sf::Font font;
	int menu_text_height;
	int input_text_height;
	int audio_device_text_height;
	int outline_thickness;
	int audio_recorder_width;
	int audio_display_height;
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
	//ARITHMETIC
	sf::Texture plus_32x32_tex;
	sf::Texture minus_32x32_tex;
	sf::Texture times_32x32_tex;
	sf::Texture dividedby_32x32_tex;
	sf::Texture modulo_32x32_tex;
	sf::Texture exponent_32x32_tex;
	//BOOLEAN
	sf::Texture and_32x32_tex;
	sf::Texture or_32x32_tex;
	//VIDEO
	sf::Texture play_24x24_tex;
	sf::Texture pause_24x24_tex;
	sf::Texture stop_24x24_tex;
	sf::Texture loop_false_24x24_tex;
	sf::Texture loop_true_24x24_tex;
	//CLOCK
	sf::Texture clock_72x72_tex;
	sf::Texture seconds_24x24_tex;
	sf::Texture millisecoonds_24x24_tex;
	sf::Texture zero_s_true_24x24_tex;
	sf::Texture zero_s_false_24x24_tex;
};

extern GUISettings gui;

extern void loadGUISettings();