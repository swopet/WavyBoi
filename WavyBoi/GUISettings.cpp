#include <pch.h>
#include <GUISettings.h>

GUISettings gui;

void loadGUISettings() {
	std::string cwd = std::experimental::filesystem::current_path().u8string();
	std::replace(cwd.begin(), cwd.end(), '\\', '/'); //WHYYYY does it return it in a format it doesn't like
	std::string TEXTURE_PATH = cwd + std::string(TEXTURE_PATH_EXT);
	gui.font.loadFromFile(cwd + std::string("/resources/fonts/Montserrat-Medium.otf"));
	gui.menu_text_height = 20;
	gui.input_text_height = 16;
	gui.text_buffer = 8;
	gui.audio_device_text_height = 16;
	gui.outline_thickness = -2.0;
	gui.audio_recorder_width = 200;
	gui.audio_display_height = 50;
	gui.menu_outline_color = sf::Color(192, 192, 192);
	gui.menu_fill_color = sf::Color(128, 128, 128);
	gui.menu_selected_fill_color = sf::Color(64, 64, 64);
	gui.obj_outline_color = sf::Color(192, 192, 192);
	gui.obj_fill_color = sf::Color(128, 128, 128);
	gui.obj_circle_radius = 5.0;

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
	//ARITHMETIC
	gui.plus_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("plus_32x32.png"));
	gui.minus_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("minus_32x32.png"));
	gui.times_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("times_32x32.png"));
	gui.dividedby_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("dividedby_32x32.png"));
	gui.modulo_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("modulo_32x32.png"));
	gui.exponent_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("exponent_32x32.png"));
	//BOOLEAN
	gui.and_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("and_32x32.png"));
	gui.or_32x32_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("or_32x32.png"));
	//VIDEO
	gui.play_24x24_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("play_24x24.png"));
	gui.pause_24x24_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("pause_24x24.png"));
	gui.stop_24x24_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("stop_24x24.png"));
	gui.loop_false_24x24_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("loop_false_24x24.png"));
	gui.loop_true_24x24_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("loop_true_24x24.png"));
	//CLOCK
	gui.clock_72x72_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("clock_72x72.png"));
	gui.seconds_24x24_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("seconds_24x24.png"));
	gui.millisecoonds_24x24_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("milliseconds_24x24.png"));
	gui.zero_s_true_24x24_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("zero_s_true_24x24.png"));
	gui.zero_s_false_24x24_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("zero_s_false_24x24.png"));
}