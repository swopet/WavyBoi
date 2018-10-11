#include <pch.h>
#include <ControlWindow.h>



ControlWindow::ControlWindow(AnimationManager * animation_manager){
	window = new sf::RenderWindow(sf::VideoMode(1600,900), "WavyBoi - " + animation_manager->getName());
	state.window_size = sf::Vector2u(1600,900);
	state.menu_height = 24;
	font = NULL;
	state.last_fps_draw = state.clock.getElapsedTime();
	loadFont("C:/Users/Trevor/Stuff/VS/WavyBoi/resources/fonts/Montserrat-Medium.otf");
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
	animation_manager->update();
	window->setActive(true);
	window->clear();
	drawLinks(animation_manager);
	drawObjects(animation_manager);
	if (state.selecting || state.selected) {
		drawSelectBox();
	}
	if (state.linking) {
		drawNewLink();
	}
	drawTopMenu(animation_manager);
	drawFPS(animation_manager);
    window->display();
	window->setTitle("WavyBoi - " + animation_manager->getName() + (animation_manager->isEdited() ? "*" : ""));
	return false;
}

void ControlWindow::drawTopMenu(AnimationManager * animation_manager){
	for (std::vector<Menu *>::iterator it = menu_tabs.begin(); it != menu_tabs.end(); ++it){
		(*it)->draw(*window,sf::RenderStates());
	}
}

void ControlWindow::drawObjects(AnimationManager * animation_manager){
	std::vector<Object *> objects = animation_manager->getObjects();
	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it){
		(*it)->draw(*window,sf::RenderStates());
	}
}

void ControlWindow::drawSelectBox() {
	sf::RectangleShape box(sf::Vector2f(state.select_end_pos - state.select_start_pos));
	box.setPosition(sf::Vector2f(state.select_start_pos));
	box.setOutlineColor(state.selecting ? sf::Color(0, 0, 64, 192) : sf::Color(0, 0, 64, 128) );
	box.setOutlineThickness(2.0f);
	box.setFillColor(state.selecting ? sf::Color(0, 0, 192, 192) : sf::Color(0, 0, 192, 128));
	window->draw(box);
}

void ControlWindow::drawFPS(AnimationManager * animation_manager) {
	if ((state.clock.getElapsedTime() - state.last_fps_draw).asMilliseconds() > TIME_PER_FPS_UPDATE_MS) {
		state.last_fps_draw = state.clock.getElapsedTime();
		state.curr_fps = animation_manager->getFPS();
	}
	sf::Text fps_text(std::to_string((int)state.curr_fps) + " FPS",*font,16);
	float width = fps_text.findCharacterPos(fps_text.getString().getSize()).x;
	fps_text.setFillColor(sf::Color::Cyan);
	fps_text.setPosition(sf::Vector2f(window->getSize().x - width, 0));
	window->draw(fps_text);
}

void ControlWindow::drawLinks(AnimationManager * animation_manager) {
	std::vector<Link *> links = animation_manager->getLinks();
	for (std::vector<Link *>::iterator it = links.begin(); it != links.end(); ++it) {
		(*it)->draw(*window, sf::RenderStates());
	}
}

void ControlWindow::drawNewLink() {
	state.new_link->draw(*window, sf::RenderStates());
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
		return false;
	}
	return true;
}

void ControlWindow::processLeftClick(sf::Vector2i mouse_pos,AnimationManager * animation_manager){
	bool processed = false;
	//check if clicked on menu tabs first
	if (!processed){
		for (std::vector<Menu *>::iterator it = menu_tabs.begin(); it != menu_tabs.end(); ++it){
			processed = (*it)->processLeftClick(mouse_pos,animation_manager);
			if (processed) break;
		}
	}
	//check if clicked on selected box
	if (!processed && state.selected) {
		sf::RectangleShape select_box(sf::Vector2f(state.select_end_pos - state.select_start_pos));
		select_box.setSize(sf::Vector2f(state.select_end_pos - state.select_start_pos));
		select_box.setPosition(sf::Vector2f(state.select_start_pos));
		sf::RectangleShape mouse_box(sf::Vector2f(0, 0));
		mouse_box.setPosition(sf::Vector2f(mouse_pos));
		//if the click is within the boundaries, start moving, otherwise turn off the selection
		if (checkIntersection(mouse_box, select_box)) {
			state.moving = true;
			std::cout << "clicked on selected box" << std::endl;
			processed = true;
		}
		else {
			state.selected = false;
			state.selected_objects.clear();
			std::cout << "clicked out of selected box" << std::endl;
		}
	}
	//check if clicked on an object
	if (!processed){
		std::vector<Object *> objects = animation_manager->getObjects();
		for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it){
			ClickResponse response = (*it)->processLeftClick(mouse_pos);
			processed = response.clicked;
			if (processed){
				switch(response.type){
					case CLICK_RESPONSE::SELECTED:
						state.selected_objects.push_back(*it);
						state.moving = true;
						//add the object to the selected_objects stack in control_window
						break;
					case CLICK_RESPONSE::GOT_LEFT:
						std::cout << "got left side of " << (*it)->getName() << std::endl;
						break;
					case CLICK_RESPONSE::GOT_RIGHT:
						std::cout << "got right side of " << (*it)->getName() << std::endl;
						state.new_link = new Link((*it), NULL, (*it)->getNewParameter());
						state.linking = true;
						break;
					default:
						break;
				}
				break;
			}
		}
	}
	if (!processed) {
		state.selecting = true;
		state.select_start_pos = mouse_pos;
	}
	state.left_mouse_held = true;
	state.last_mouse_pos = mouse_pos;
}
void ControlWindow::processLeftClickHeld(AnimationManager * animation_manager){
	if (!state.left_mouse_held) return;
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
	sf::Vector2i diff = mouse_pos - state.last_mouse_pos;
	state.last_mouse_pos = mouse_pos;
	if (state.moving) {
		for (std::vector<Object *>::iterator it = state.selected_objects.begin(); it != state.selected_objects.end(); ++it) {
			(*it)->move(sf::Vector2f(diff));
		}
		state.select_start_pos += diff;
		state.select_end_pos += diff;
	}
	else if (state.linking) {
		state.new_link->setOutPos(sf::Vector2f(mouse_pos));
	}
	else if (state.selecting) {
		std::vector<Object *> objects = animation_manager->getObjects();
		state.selected_objects.clear();
		sf::RectangleShape select_box(sf::Vector2f(state.select_end_pos - state.select_start_pos));
		select_box.setSize(sf::Vector2f(state.select_end_pos - state.select_start_pos));
		select_box.setPosition(sf::Vector2f(state.select_start_pos));
		for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
			if ((*it)->checkOverlap(select_box)) state.selected_objects.push_back(*it);
		}
		state.select_end_pos = mouse_pos;
	}
}
void ControlWindow::processLeftClickRelease(sf::Vector2i mouse_pos, AnimationManager * animation_manager) {
	if (!(state.selecting || state.selected)){
		state.selected_objects.clear();
	}
	state.left_mouse_held = false;
	if (state.linking) {
		std::vector<Object *> objects = animation_manager->getObjects();
		for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
			ClickResponse response = (*it)->processLeftClick(mouse_pos);
			if (response.clicked) {
				if (response.type == CLICK_RESPONSE::GOT_LEFT) {
					if (state.new_link->setOutObject((*it))) {
						//give control of the link to the animation manager
						animation_manager->addLink(state.new_link);
						state.new_link = NULL;
						break;
					}
				}
			}
		}
		if (state.new_link != NULL) {
			delete state.new_link;
			state.new_link = NULL;
		}
		state.linking = false;
	}
	if (state.moving) state.moving = false;
	if (state.selecting) {
		state.select_end_pos = mouse_pos;
		state.selecting = false;
		state.selected = state.selected_objects.size() > 0;
	}
}