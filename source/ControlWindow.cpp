#include "ControlWindow.h"



ControlWindow::ControlWindow(AnimationManager * animation_manager){
	window = new sf::RenderWindow(sf::VideoMode(1600,900), "WavyBoi - " + animation_manager->getName());
	state.window_size = sf::Vector2u(1600,900);
	state.menu_height = 24;
	font = NULL;
	loadFont("resources/fonts/Montserrat-Medium.otf");
	sf::Vector2i pos(0,0);
	Menu * file_menu = new Menu();
	file_menu->initialize(MENU_TYPE::FILE,font,pos,state.menu_height);
	menu_tabs.push_back(file_menu);
	pos = pos + sf::Vector2i(file_menu->get_width(),0);
	Menu * edit_menu = new Menu();
	edit_menu->initialize(MENU_TYPE::EDIT,font,pos,state.menu_height);
	menu_tabs.push_back(edit_menu);
	pos = pos + sf::Vector2i(edit_menu->get_width(),0);
	Menu * display_menu = new Menu();
	display_menu->initialize(MENU_TYPE::DISPLAY,font,pos,state.menu_height);
	menu_tabs.push_back(display_menu);
}

ControlWindow::ControlWindow(){
	
}

bool ControlWindow::update(AnimationManager * animation_manager){
	sf::Event event;
	processLeftClickHeld(animation_manager);
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed){
			return true;
		}
		if (event.type == sf::Event::MouseButtonPressed){
			if (event.mouseButton.button == sf::Mouse::Left){
				processLeftClick(sf::Vector2i(event.mouseButton.x,event.mouseButton.y),animation_manager);
			}
			if (event.mouseButton.button == sf::Mouse::Right){
				
			}
			if (event.mouseButton.button == sf::Mouse::Middle){
				
			}
		}
		if (event.type == sf::Event::MouseButtonReleased){
			if (event.mouseButton.button == sf::Mouse::Left){
				processLeftClickRelease(sf::Vector2i(event.mouseButton.x,event.mouseButton.y),animation_manager);
			}
			if (event.mouseButton.button == sf::Mouse::Right){
				
			}
			if (event.mouseButton.button == sf::Mouse::Middle){
				
			}
		}
	}
	for (std::vector<Menu *>::iterator it = menu_tabs.begin(); it != menu_tabs.end(); ++it){
		if ((*it)->getType() == MENU_TYPE::DISPLAY){
			if (animation_manager->isDisplayOpen()){
				if ((*it)->getOptionEnabled(0)){
					(*it)->toggleOption(0);
					(*it)->toggleOption(1);
				}
			}
			else {
				if ((*it)->getOptionEnabled(1)){
					(*it)->toggleOption(0);
					(*it)->toggleOption(1);
				}
			}
		}
		(*it)->update(sf::Mouse::getPosition(*window));
	}
	window->clear();
	drawTopMenu(animation_manager);
    window->display();
	window->setTitle("WavyBoi - " + animation_manager->getName() + (animation_manager->isEdited() ? "*" : ""));
	return false;
}

void ControlWindow::drawTopMenu(AnimationManager * animation_manager){
	for (std::vector<Menu *>::iterator it = menu_tabs.begin(); it != menu_tabs.end(); ++it){
		(*it)->draw(*window,sf::RenderStates());
	}
}

void ControlWindow::close(){
	window->close();
}

bool ControlWindow::loadFont(std::string font_name){
	if (font != NULL){
		delete font;
	}
	font = new sf::Font();
	if (!font->loadFromFile(font_name)){
		std::cerr << "Could not load font " << font_name << std::endl;
	}
}

void ControlWindow::processLeftClick(sf::Vector2i mouse_pos,AnimationManager * animation_manager){
	bool processed = false;
	std::cout << "Mouse left clicked at " << mouse_pos.x << "," << mouse_pos.y << std::endl;
	if (!processed){
		for (std::vector<Menu *>::iterator it = menu_tabs.begin(); it != menu_tabs.end(); ++it){
			processed = (*it)->processLeftClick(mouse_pos,animation_manager);
			if (processed) break;
		}
	}
	state.left_mouse_held = true;
	state.last_mouse_pos = mouse_pos;
}
void ControlWindow::processLeftClickHeld(AnimationManager * animation_manager){
	if (!state.left_mouse_held) return;
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
	state.last_mouse_pos = mouse_pos;
}
void ControlWindow::processLeftClickRelease(sf::Vector2i mouse_pos,AnimationManager * animation_manager){
	state.left_mouse_held = false;
}