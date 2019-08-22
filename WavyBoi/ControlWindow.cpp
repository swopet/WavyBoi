#include <pch.h>
#include <ControlWindow.h>

ControlWindow::ControlWindow(AnimationManager * animation_manager){
	Preferences preferences = animation_manager->getPreferences();
	state.window_size = preferences.control_resolution;
	window = new sf::RenderWindow(sf::VideoMode(state.window_size.x,state.window_size.y), "WavyBoi - " + animation_manager->getName());
	window->setPosition(preferences.control_position);
	window->setVerticalSyncEnabled(true);
	font = NULL;
	state.last_fps_draw = state.clock.getElapsedTime();
	state.workspace_pos = sf::Vector2i(0, 0);
	InitializeMenuTabs();
}

void ControlWindow::InitializeMenuTabs() {
	sf::Vector2i pos(0, 0);
	Menu * file_menu = new Menu();
	file_menu->initialize(MENU_TYPE::FILE, pos);
	menu_tabs.push_back(file_menu);
	pos = pos + sf::Vector2i(file_menu->get_width(), 0);
	Menu * edit_menu = new Menu();
	edit_menu->initialize(MENU_TYPE::EDIT, pos);
	menu_tabs.push_back(edit_menu);
	pos = pos + sf::Vector2i(edit_menu->get_width(), 0);
	Menu * new_menu = new Menu();
	new_menu->initialize(MENU_TYPE::NEW, pos);
	menu_tabs.push_back(new_menu);
	pos = pos + sf::Vector2i(new_menu->get_width(), 0);
	Menu * display_menu = new Menu();
	display_menu->initialize(MENU_TYPE::DISPLAY, pos);
	menu_tabs.push_back(display_menu);
}

ControlWindow::ControlWindow(){
	
}

bool ControlWindow::update(AnimationManager * animation_manager){
	sf::Event event;
	processLeftClickHeld(animation_manager);
	processRightClickHeld(animation_manager);
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed){
			return true;
		}
		if (event.type == sf::Event::Resized) {
			std::cout << "Window resized!" << std::endl;
			state.window_size = sf::Vector2u(event.size.width, event.size.height);
			sf::View view;
			view.setSize(event.size.width, event.size.height);
			view.setCenter(sf::Vector2f(event.size.width / 2.0, event.size.height / 2.0));
			view.setViewport(sf::FloatRect(0, 0, 1, 1));
			window->setView(view);
		}
		if (event.type == sf::Event::MouseButtonPressed){
			if (event.mouseButton.button == sf::Mouse::Left){
				processLeftClick(sf::Vector2i(event.mouseButton.x,event.mouseButton.y),animation_manager);
			}
			if (event.mouseButton.button == sf::Mouse::Right){
				processRightClick(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), animation_manager);
			}
			if (event.mouseButton.button == sf::Mouse::Middle){
			}
		}
		if (event.type == sf::Event::MouseWheelScrolled) {
			processMouseWheel(sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y), event.mouseWheelScroll.delta, animation_manager);
		}
		if (event.type == sf::Event::MouseButtonReleased){
			if (event.mouseButton.button == sf::Mouse::Left){
				processLeftClickRelease(sf::Vector2i(event.mouseButton.x,event.mouseButton.y),animation_manager);
			}
			if (event.mouseButton.button == sf::Mouse::Right){
				processRightClickRelease(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), animation_manager);
			}
			if (event.mouseButton.button == sf::Mouse::Middle){
				
			}
		}
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Delete) {
				deleteSelected(animation_manager);
			}
			if (event.key.code == sf::Keyboard::Enter) {
				if (state.entering_text) {
					state.new_text_obj->processNewString(state.text_field, state.text_entered);
					state.text_field = "";
					state.text_entered = "";
					state.new_text_obj = NULL;
					state.entering_text = false;
				}
				else if (state.entering_cmd) {
					processCommand(state.command_entered, animation_manager);
					state.entering_cmd = false;
					state.command_entered = "";
				}
				else {
					state.command_entered = "";
					state.entering_cmd = true;
				}
			}
			if (event.key.code == sf::Keyboard::Up) {
				if (state.entering_cmd) {
					if (state.previous_commands.size() > 0) {
						state.command_entered = state.previous_commands[state.curr_command];
						state.curr_command = (state.curr_command + 1) % state.previous_commands.size();
					}
				}
			}
		}
		if (event.type == sf::Event::TextEntered) {
			if (state.entering_text) {
				if (event.text.unicode == 8) { //BACKSPACE
					if (state.text_entered.length() > 0)
						state.text_entered.pop_back();
				}
				else {
					state.text_entered += static_cast<char>(event.text.unicode);
				}
			}
			else if (state.entering_cmd) {
				state.curr_command = 0;
				if (event.text.unicode == 8) { //BACKSPACE
					if (state.command_entered.length() > 0)
						state.command_entered.pop_back();
				}
				else {
					state.command_entered += static_cast<char>(event.text.unicode);
				}
			}
		}
	}
	if (animation_manager->isDeleteRequested()) {
		deleteSelected(animation_manager);
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
	glViewport(0, 0, state.window_size.x, state.window_size.y);
	drawLinks(animation_manager);
	drawObjects(animation_manager);
	drawChannels(animation_manager);
	if (state.selecting || state.selected) {
		drawSelectBox();
	}
	if (state.linking) {
		drawNewLink();
	}
	drawTopMenu(animation_manager);
	drawFPS(animation_manager);
	if (state.entering_cmd) {
		drawCommandBox();
	}
	AudioHandler * audio_handler = animation_manager->getAudioHandler();
	audio_handler->setPosition(sf::Vector2f(0, state.window_size.y - gui.outline_thickness * 4 - gui.audio_display_height - gui.audio_device_text_height - gui.play_24x24_tex.getSize().y));
	audio_handler->draw(*window, sf::RenderStates());
	if (state.entering_text) {
		drawTextEntered();
	}
    window->display();
	window->setTitle("WavyBoi - " + animation_manager->getName() + (animation_manager->isEdited() ? "*" : ""));
	return false;
}

void ControlWindow::deleteSelected(AnimationManager * animation_manager)
{
	if (state.selected) {
		std::cout << "links to delete: " << state.selected_links.size() << std::endl;
		while (!state.selected_links.empty()) {
			animation_manager->deleteLink(state.selected_links.back());
			state.selected_links.pop_back();
		}
		while (!state.selected_objects.empty()) {
			animation_manager->deleteObject(state.selected_objects.back());
			state.selected_objects.pop_back();
		}
	}
	state.selected = false;
}

void ControlWindow::drawCommandBox()
{
	sf::Text command_text;
	command_text.setString(state.command_entered);
	command_text.setFillColor(sf::Color::White);
	command_text.setCharacterSize(gui.input_text_height);
	command_text.setFont(gui.font);
	float y = state.window_size.y - gui.audio_device_text_height - gui.audio_display_height - gui.outline_thickness * 4 - gui.input_text_height * 2 - 24;
	command_text.setPosition(sf::Vector2f(gui.outline_thickness, y));
	window->draw(command_text);
}

void ControlWindow::drawTopMenu(AnimationManager * animation_manager){
	for (std::vector<Menu *>::iterator it = menu_tabs.begin(); it != menu_tabs.end(); ++it){
		(*it)->draw(*window,sf::RenderStates());
	}
}

void ControlWindow::drawObjects(AnimationManager * animation_manager){
	sf::Transform transform;
	transform.translate(-sf::Vector2f(state.workspace_pos));
	transform.translate(sf::Vector2f(0.0, gui.menu_text_height));
	sf::RenderStates render_states(transform);
	std::vector<Object *> objects = animation_manager->getObjects();
	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it){
		(*it)->draw(*window,render_states);
	}
}

void ControlWindow::drawChannels(AnimationManager * animation_manager)
{
	sf::Transform transform;
	transform.translate(-sf::Vector2f(state.workspace_pos));
	transform.translate(sf::Vector2f(0.0, gui.menu_text_height));
	sf::RenderStates render_states(transform);
	std::vector<Channel *> channels = animation_manager->getChannels();
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it) {
		(*it)->draw(*window, render_states);
	}
}

void ControlWindow::drawSelectBox() {
	sf::Transform transform;
	transform.translate(-sf::Vector2f(state.workspace_pos));
	transform.translate(sf::Vector2f(0.0, gui.menu_text_height));
	sf::RenderStates render_states(transform);
	sf::RectangleShape box(sf::Vector2f(state.select_end_pos - state.select_start_pos));
	box.setPosition(sf::Vector2f(state.select_start_pos));
	box.setOutlineColor(state.selecting ? sf::Color(0, 0, 64, 192) : sf::Color(0, 0, 64, 128) );
	box.setOutlineThickness(2.0f);
	box.setFillColor(state.selecting ? sf::Color(0, 0, 192, 192) : sf::Color(0, 0, 192, 128));
	window->draw(box);
}

void ControlWindow::drawTextEntered() {
	state.text_text.setString(state.text_entered);
	state.text_text.setFont(gui.font);
	state.text_text.setCharacterSize(gui.input_text_height);
	state.text_text.setPosition(0, 0);
	state.text_box.setSize(sf::Vector2f(gui.outline_thickness*2, gui.outline_thickness*2) + sf::Vector2f(state.text_text.findCharacterPos(state.text_entered.length()).x, gui.input_text_height));
	state.text_text.setPosition(sf::Vector2f(state.last_mouse_pos) + sf::Vector2f(gui.outline_thickness, gui.outline_thickness));
	state.text_box.setPosition(sf::Vector2f(state.last_mouse_pos));
	state.text_text.setFillColor(sf::Color::White);
	state.text_box.setFillColor(gui.obj_fill_color);
	state.text_box.setOutlineColor(gui.obj_outline_color);
	state.text_box.setOutlineThickness(gui.outline_thickness);
	window->draw(state.text_box);
	window->draw(state.text_text);
}

void ControlWindow::drawFPS(AnimationManager * animation_manager) {
	if ((state.clock.getElapsedTime() - state.last_fps_draw).asMilliseconds() > TIME_PER_FPS_UPDATE_MS) {
		state.last_fps_draw = state.clock.getElapsedTime();
		state.curr_fps = animation_manager->getFPS();
	}
	sf::Text fps_text(std::to_string((int)state.curr_fps) + " FPS",gui.font,16);
	float width = fps_text.findCharacterPos(fps_text.getString().getSize()).x;
	fps_text.setFillColor(sf::Color::Cyan);
	fps_text.setPosition(sf::Vector2f(window->getSize().x - width, 0));
	window->draw(fps_text);
}

void ControlWindow::drawLinks(AnimationManager * animation_manager) {
	sf::Transform transform;
	transform.translate(-sf::Vector2f(state.workspace_pos));
	transform.translate(sf::Vector2f(0.0, gui.menu_text_height));
	sf::RenderStates render_states(transform);
	std::vector<Link *> links = animation_manager->getLinks();
	for (std::vector<Link *>::iterator it = links.begin(); it != links.end(); ++it) {
		(*it)->draw(*window, render_states);
	}
}

void ControlWindow::drawNewLink() {
	sf::Transform transform;
	transform.translate(-sf::Vector2f(state.workspace_pos));
	transform.translate(sf::Vector2f(0.0, gui.menu_text_height));
	sf::RenderStates render_states(transform);
	state.new_link->draw(*window, render_states);
}

void ControlWindow::close(){
	window->close();
}

void ControlWindow::processCommand(std::string command, AnimationManager * animation_manager)
{
	state.curr_command = 0;
	std::vector<std::string> elts;
	int last_ind = 1;
	for (int i = 0; i <= command.length(); i++) {
		if (i == command.length() || command[i] == ' ') {
			if (i != 0 && command[i - 1] != '\\' || i == command.length()) {
				int size = i - last_ind;
				if (size > 0) elts.push_back(command.substr(last_ind, size));
				last_ind = i + 1;
			}
		}
	}
	if (elts.size() > 0) {
		animation_manager->processCommand(elts);
		state.previous_commands.insert(state.previous_commands.begin(), command);
	}
}

void ControlWindow::processRightClick(sf::Vector2i mouse_pos, AnimationManager * animation_manager) {
	state.last_mouse_pos = mouse_pos;
	state.dragging_window = true;
	state.right_mouse_held = true;
}

void ControlWindow::processLeftClick(sf::Vector2i mouse_pos, AnimationManager * animation_manager){
	if (state.entering_text) {
		if (!checkIntersection(state.text_box, sf::Vector2f(mouse_pos))) {
			state.entering_text = false;
		}
	}
	if ((state.clock.getElapsedTime() - state.last_left_click).asMilliseconds() < 300) {
		processDoubleLeftClick(mouse_pos, animation_manager);
		state.selecting = false;
		return;
	}
	bool processed = false;
	//check if clicked on menu tabs first
	if (!processed){
		for (std::vector<Menu *>::iterator it = menu_tabs.begin(); it != menu_tabs.end(); ++it){
			processed = (*it)->processLeftClick(mouse_pos,animation_manager);
			if (processed) break;
		}
	}
	//then check if clicked on audio handler
	if (!processed) {
		ClickResponse response = animation_manager->getAudioHandler()->processLeftClick(mouse_pos);
		processed = response.clicked;
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
			state.selected_links.clear();
			std::cout << "clicked out of selected box" << std::endl;
		}
	}
	//check if clicked on an object
	if (!processed){
		sf::Vector2i offset = -sf::Vector2i(0, gui.menu_text_height) + sf::Vector2i(state.workspace_pos);
		std::vector<Object *> objects = animation_manager->getObjects();
		for (std::vector<Object *>::reverse_iterator it = objects.rbegin(); it != objects.rend(); ++it){
			ClickResponse response = (*it)->processLeftClick(mouse_pos+offset);
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
						state.linking = true;
						break;
					case CLICK_RESPONSE::DELETED_MUX_INPUT:
						animation_manager->decrementLinkOutIndsGreaterThan(response.ind,*it);
						break;
					default:
						break;
				}
				animation_manager->pushToTop(*it);
				break;
			}
		}
	}
	if (!processed) {
		state.selecting = true;
		state.select_start_pos = mouse_pos;
		state.select_end_pos = mouse_pos;
	}
	state.left_mouse_held = true;
	state.last_mouse_pos = mouse_pos;
	state.last_left_click = state.clock.getElapsedTime();
}

void ControlWindow::processRightClickHeld(AnimationManager * animation_manager) {
	if (!state.right_mouse_held) return;
	else if (state.dragging_window){
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
		sf::Vector2i diff = mouse_pos - state.last_mouse_pos;
		state.last_mouse_pos = mouse_pos;
		state.workspace_pos = state.workspace_pos - diff;
	}
}

void ControlWindow::processLeftClickHeld(AnimationManager * animation_manager){
	if (!state.left_mouse_held) return;
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
	sf::Vector2i diff = mouse_pos - state.last_mouse_pos;
	state.last_mouse_pos = mouse_pos;
	if (state.moving) {
		if (!state.selected_objects.empty()) { //can only move objects, not links
			for (std::vector<Object *>::iterator it = state.selected_objects.begin(); it != state.selected_objects.end(); ++it) {
				(*it)->move(sf::Vector2f(diff));
			}
			state.select_start_pos += diff;
			state.select_end_pos += diff;
		}
	}
	else if (state.linking) {
		sf::Vector2i offset = -sf::Vector2i(0, gui.menu_text_height) + sf::Vector2i(state.workspace_pos);
		state.new_link->setOutPos(sf::Vector2f(mouse_pos + offset));
	}
	else if (state.selecting) {
		sf::Vector2i offset = -sf::Vector2i(0, gui.menu_text_height) + sf::Vector2i(state.workspace_pos);
		std::vector<Object *> objects = animation_manager->getObjects();
		std::vector<Link *> links = animation_manager->getLinks();
		state.selected_objects.clear();
		state.selected_links.clear();
		sf::RectangleShape select_box(sf::Vector2f(state.select_end_pos - state.select_start_pos));
		select_box.setSize(sf::Vector2f(state.select_end_pos - state.select_start_pos));
		select_box.setPosition(sf::Vector2f(state.select_start_pos) + sf::Vector2f(offset));
		for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
			if ((*it)->checkOverlap(select_box)) {
				state.selected_objects.push_back(*it);
			}
		}
		for (std::vector<Link *>::iterator it = links.begin(); it != links.end(); ++it) {
			if ((*it)->checkOverlap(select_box)) state.selected_links.push_back(*it);
		}
		state.select_end_pos = mouse_pos;
	}
}

void ControlWindow::processRightClickRelease(sf::Vector2i mouse_pos, AnimationManager * animation_manager) {
	state.right_mouse_held = false;
	if (state.dragging_window) state.dragging_window = false;
}

void ControlWindow::processLeftClickRelease(sf::Vector2i mouse_pos, AnimationManager * animation_manager) {
	if (!(state.selecting || state.selected)){
		state.selected_objects.clear();
		state.selected_links.clear();
	}
	state.left_mouse_held = false;
	if (state.linking) {
		sf::Vector2i offset = -sf::Vector2i(0, gui.menu_text_height) + sf::Vector2i(state.workspace_pos);
		std::vector<Object *> objects = animation_manager->getObjects();
		for (std::vector<Object *>::reverse_iterator it = objects.rbegin(); it != objects.rend(); ++it) {
			ClickResponse response = (*it)->processLeftClickRelease(mouse_pos+offset);
			if (response.clicked) {
				if (response.type == CLICK_RESPONSE::GOT_LEFT) {
					if (state.new_link->setOutObject((*it))) {
						state.new_link->setOutInd(response.ind);
						//give control of the link to the animation manager
						animation_manager->addLink(state.new_link);
						state.new_link = NULL;
						break;
					}
				}
			}
		}
		if (state.new_link != NULL) {
			std::vector<Channel *> channels = animation_manager->getChannels();
			for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it) {
				ClickResponse response = (*it)->processLeftClickRelease(mouse_pos+offset);
				if (response.clicked){ 
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
		state.selected = (state.selected_objects.size() > 0 || state.selected_links.size() > 0);
	}
}

void ControlWindow::processDoubleLeftClick(sf::Vector2i mouse_pos, AnimationManager * animation_manager) {
	sf::Vector2i offset = -sf::Vector2i(0, gui.menu_text_height) + sf::Vector2i(state.workspace_pos);
	bool processed = false;
	std::cout << "double clicked" << std::endl;
	std::vector<Object *> objects = animation_manager->getObjects();
	std::vector<Link *> links = animation_manager->getLinks();
	for (std::vector<Object *>::reverse_iterator it = objects.rbegin(); it != objects.rend(); ++it) {
		ClickResponse response = (*it)->processDoubleLeftClick(mouse_pos+offset);
		if (response.clicked) {
			if (response.type == CLICK_RESPONSE::GOT_TEXT_FIELD) {
				state.entering_text = true;
				state.new_text_obj = *it;
				state.text_entered = "";
				state.text_field = response.field;
				state.last_mouse_pos = mouse_pos;
			}
			processed = true;
			break;
		}
	}
	if (!processed) {
		for (std::vector<Link *>::reverse_iterator it = links.rbegin(); it != links.rend(); ++it) {
			ClickResponse response = (*it)->processDoubleLeftClick(mouse_pos+offset);
			if (response.clicked) {
				if (response.type == CLICK_RESPONSE::GOT_TEXT_FIELD) {
					state.entering_text = true;
					state.new_text_obj = *it;
					state.text_entered = "";
					state.text_field = response.field;
					state.last_mouse_pos = mouse_pos;
				}
				processed = true;
				break;
			}
		}
	}
	//edit text with a double click
}

void ControlWindow::processMouseWheel(sf::Vector2i mouse_pos, int delta, AnimationManager * animation_manager) {
	sf::Vector2i offset = -sf::Vector2i(0, gui.menu_text_height) + sf::Vector2i(state.workspace_pos);
	bool processed = false;
	if (!processed) {
		ClickResponse response = animation_manager->getAudioHandler()->processMouseWheel(mouse_pos, delta);
		processed = response.clicked;
	}
	if (!processed) {
		std::vector<Object *>objects = animation_manager->getObjects();
		for (std::vector<Object *>::reverse_iterator it = objects.rbegin(); it != objects.rend(); ++it) {
			ClickResponse response = (*it)->processMouseWheel(mouse_pos + offset, delta);
			if (response.clicked) {
				processed = true;
				break;
			}
		}
	}
}