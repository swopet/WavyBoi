#pragma once
/* ControlWindow has all the buttons and menus and shit. */

#include <SFML/Graphics.hpp>
#include <string>
#include <AnimationManager.h>
#include <Menu.h>
#include <vector>
#define TIME_PER_FPS_UPDATE_MS 500

struct ControlWindowState {
	sf::Vector2u window_size;
	unsigned int menu_height;
	bool left_mouse_held = false;
	sf::Vector2i last_mouse_pos;
	std::vector<Object *> selected;
	sf::Clock clock;
	sf::Time last_fps_draw;
	float curr_fps;
};

class ControlWindow {
private:
	ControlWindowState state;
	sf::RenderWindow * window;
	sf::Font * font;
	std::vector<Menu *> menu_tabs;
public:
	ControlWindow();
	ControlWindow(AnimationManager *);
	bool update(AnimationManager *);
	void drawTopMenu(AnimationManager *);
	void drawObjects(AnimationManager *);
	void drawFPS(AnimationManager * animation_manager);
	void close();
	bool loadFont(std::string);
	void processLeftClick(sf::Vector2i,AnimationManager *);
	void processLeftClickHeld(AnimationManager *);
	void processLeftClickRelease(sf::Vector2i,AnimationManager *);
};