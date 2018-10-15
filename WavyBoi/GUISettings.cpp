#include <pch.h>
#include <GUISettings.h>

GUISettings gui;

void loadGUISettings() {
	gui.add_20x20_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("add_20x20.png"));
	gui.delete_20x20_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("delete_20x20.png"));
	gui.font.loadFromFile("C:/Users/Trevor/Stuff/VS/WavyBoi/resources/fonts/Montserrat-Medium.otf");
	gui.menu_text_height = 24;
	gui.outline_thickness = 2;
	gui.menu_outline_color = sf::Color(192, 192, 192);
	gui.menu_fill_color = sf::Color(128, 128, 128);
	gui.menu_selected_fill_color = sf::Color(64, 64, 64);
	gui.obj_outline_color = sf::Color(192, 192, 192);
	gui.obj_fill_color = sf::Color(128, 128, 128);
	gui.obj_circle_radius = 4;
}