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
	bool moving = false;
	bool selecting = false;
	bool selected = false;
	bool linking = false;
	Link * new_link = NULL;
	sf::Vector2i select_start_pos;
	sf::Vector2i select_end_pos;
	sf::Vector2i last_mouse_pos;
	std::vector<Object *> selected_objects;
	std::vector<Link *> selected_links;
	sf::Clock clock;
	sf::Time last_fps_draw;
	float curr_fps;
};

class ControlWindow {
private:
	ControlWindowState state;
	sf::RenderWindow * window;
	sf::Font * font = NULL;
	std::vector<Menu *> menu_tabs;
public:
	ControlWindow();
	ControlWindow(AnimationManager *);
	bool update(AnimationManager *);
	void deleteSelected(AnimationManager *);
	void drawTopMenu(AnimationManager *);
	void drawObjects(AnimationManager *);
	void drawChannels(AnimationManager *);
	void drawSelectBox();
	void drawFPS(AnimationManager * animation_manager);
	void drawLinks(AnimationManager * animation_manager);
	void drawNewLink();
	void close();
	bool loadFont(std::string);
	void processLeftClick(sf::Vector2i,AnimationManager *);
	void processLeftClickHeld(AnimationManager *);
	void processLeftClickRelease(sf::Vector2i,AnimationManager *);
};