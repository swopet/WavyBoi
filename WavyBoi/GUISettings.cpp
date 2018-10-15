#include <pch.h>
#include <GUISettings.h>

GUISettings gui;

void loadGUISettings() {
	
	gui.font.loadFromFile("C:/Users/Trevor/Stuff/VS/WavyBoi/resources/fonts/Montserrat-Medium.otf");
	gui.menu_text_height = 24;
	gui.outline_thickness = 2;
	gui.menu_outline_color = sf::Color(192, 192, 192);
	gui.menu_fill_color = sf::Color(128, 128, 128);
	gui.menu_selected_fill_color = sf::Color(64, 64, 64);
	gui.obj_outline_color = sf::Color(192, 192, 192);
	gui.obj_fill_color = sf::Color(128, 128, 128);
	gui.obj_circle_radius = 4;

	//TEXTURES
	//MULTIPLEXER
	gui.add_20x20_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("add_20x20.png"));
	gui.delete_20x20_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("delete_20x20.png"));
	//COMPARATOR
	gui.gte_false_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("gte_false_32x32.png"));
	gui.gte_true_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("gte_true_32x32.png"));
	gui.gt_false_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("gt_false_32x32.png"));
	gui.gt_true_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("gt_true_32x32.png"));
	gui.lte_false_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("lte_false_32x32.png"));
	gui.lte_true_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("lte_true_32x32.png"));
	gui.lt_false_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("lt_false_32x32.png"));
	gui.lt_true_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("lt_true_32x32.png"));
	gui.eq_false_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("eq_false_32x32.png"));
	gui.eq_true_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("eq_true_32x32.png"));
	gui.neq_false_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("neq_false_32x32.png"));
	gui.neq_true_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("neq_true_32x32.png"));
}