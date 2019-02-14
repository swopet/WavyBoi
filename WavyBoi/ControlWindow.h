#pragma once
/* ControlWindow has all the buttons and menus and shit. */

#include <SFML/Graphics.hpp>
#include <string>
#include <AnimationManager.h>
#include <AudioHandler.h>
#include <Menu.h>
#include <vector>
#define TIME_PER_FPS_UPDATE_MS 500

struct ControlWindowState {
	sf::Vector2u window_size;
	bool left_mouse_held = false;
	bool right_mouse_held = false;
	bool moving = false;
	bool selecting = false;
	bool selected = false;
	bool linking = false;
	bool dragging_window = false;
	//Entering Text
	sf::RectangleShape text_box;
	sf::Text text_text;
	bool entering_text = false;
	bool entering_cmd = false;
	std::string text_entered;
	std::string text_field;
	std::string command_entered;
	Object * new_text_obj;
	//Linking
	Link * new_link = NULL;
	//Selecting
	sf::Vector2i select_start_pos;
	sf::Vector2i select_end_pos;
	std::vector<Object *> selected_objects;
	std::vector<Link *> selected_links;
	//Moving
	sf::Vector2i last_mouse_pos;
	sf::Vector2i workspace_pos;
	//Double Click
	sf::Clock clock;
	sf::Time last_left_click;
	//FPS
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
	void InitializeMenuTabs();
	bool update(AnimationManager *);
	void deleteSelected(AnimationManager *);
	
	void drawTopMenu(AnimationManager *);
	void drawObjects(AnimationManager *);
	void drawChannels(AnimationManager *);
	void drawSelectBox();
	void drawCommandBox();
	void drawTextEntered();
	void drawFPS(AnimationManager * animation_manager);
	void drawLinks(AnimationManager * animation_manager);
	void drawNewLink();
	void close();
	void processCommand(std::string, AnimationManager * animation_manager);
	void processRightClick(sf::Vector2i mouse_pos, AnimationManager * animation_manager);
	void processLeftClick(sf::Vector2i,AnimationManager *);
	void processRightClickHeld(AnimationManager * animation_manager);
	void processLeftClickHeld(AnimationManager *);
	void processRightClickRelease(sf::Vector2i mouse_pos, AnimationManager * animation_manager);
	void processLeftClickRelease(sf::Vector2i,AnimationManager *);
	void processDoubleLeftClick(sf::Vector2i mouse_pos, AnimationManager * animation_manager);
	void processMouseWheel(sf::Vector2i mouse_pos, int delta, AnimationManager * animation_manager);
};