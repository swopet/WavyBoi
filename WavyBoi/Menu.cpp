#include <pch.h>
#include <Menu.h>

Menu::Menu(){
	
}

MENU_TYPE Menu::getType(){
	return type;
}

void Menu::toggleOption(int pos){
	menu_options.at(pos).enabled = !menu_options.at(pos).enabled;
}

bool Menu::getOptionEnabled(int pos){
	return menu_options.at(pos).enabled;
}

void Menu::initialize(MENU_TYPE new_type,sf::Vector2i new_pos){
	type = new_type;
	switch(type){
		case MENU_TYPE::FILE:
			name = "File";
			menu_options.push_back(MenuOption(std::string("New"),&AnimationManager::clickFileNew,true));
			menu_options.push_back(MenuOption(std::string("Open"),&AnimationManager::clickFileOpen,true));
			menu_options.push_back(MenuOption(std::string("Save"),&AnimationManager::clickFileSave,true));
			menu_options.push_back(MenuOption(std::string("Save As"),&AnimationManager::clickFileSaveAs,true));
		break;
		case MENU_TYPE::EDIT:
			name = "Edit";
			menu_options.push_back(MenuOption(std::string("Cut"),&AnimationManager::clickEditCut,true));
			menu_options.push_back(MenuOption(std::string("Copy"),&AnimationManager::clickEditCopy,true));
			menu_options.push_back(MenuOption(std::string("Paste"),&AnimationManager::clickEditPaste,true));
			menu_options.push_back(MenuOption(std::string("Delete"),&AnimationManager::clickEditDelete,true));
		break;
		case MENU_TYPE::DISPLAY:
			name = "Display";
			menu_options.push_back(MenuOption(std::string("Open"),&AnimationManager::clickDisplayOpen,true));
			menu_options.push_back(MenuOption(std::string("Close"),&AnimationManager::clickDisplayClose,false));
		break;
		case MENU_TYPE::NEW:
			name = "New";
			menu_options.push_back(MenuOption(std::string("Comparator"), NULL, true, true));
			menu_options.push_back(MenuOption(std::string("Operator"), NULL, true, true));
			menu_options.push_back(MenuOption(std::string("Number"), NULL, true, true));
			menu_options.push_back(MenuOption(std::string("Function"), NULL, true, true));
			menu_options.push_back(MenuOption(std::string("Clock"), &AnimationManager::clickNewClock, true, false));
		break;
		case MENU_TYPE::COMPARATOR:
			name = "";
			menu_options.push_back(MenuOption(std::string("<"), &AnimationManager::clickNewLT, true, false));
			menu_options.push_back(MenuOption(std::string("<="), &AnimationManager::clickNewLTE, true, false));
			menu_options.push_back(MenuOption(std::string(">"), &AnimationManager::clickNewGT, true, false));
			menu_options.push_back(MenuOption(std::string(">="), &AnimationManager::clickNewGTE, true, false));
			menu_options.push_back(MenuOption(std::string("=="), &AnimationManager::clickNewEQ, true, false));
			menu_options.push_back(MenuOption(std::string("!="), &AnimationManager::clickNewNEQ, true, false));
		break;
		case MENU_TYPE::OPERATOR:
			name = "";
			menu_options.push_back(MenuOption(std::string("+"), &AnimationManager::clickNewPlus, true, false));
			menu_options.push_back(MenuOption(std::string("-"), &AnimationManager::clickNewMinus, true, false));
			menu_options.push_back(MenuOption(std::string("/"), &AnimationManager::clickNewDividedby, true, false));
			menu_options.push_back(MenuOption(std::string("*"), &AnimationManager::clickNewTimes, true, false));
			menu_options.push_back(MenuOption(std::string("^"), &AnimationManager::clickNewExponent, true, false));
			menu_options.push_back(MenuOption(std::string("%"), &AnimationManager::clickNewModulo, true, false));
		break;
		case MENU_TYPE::NUMBER:
			name = "";
			menu_options.push_back(MenuOption(std::string("Integer"), &AnimationManager::clickNewInt, true, false));
			menu_options.push_back(MenuOption(std::string("Float"), &AnimationManager::clickNewFloat, true, false));
			menu_options.push_back(MenuOption(std::string("PI"), &AnimationManager::clickNewPI, true, false));
			menu_options.push_back(MenuOption(std::string("e"), &AnimationManager::clickNewE, true, false));
			break;
		case MENU_TYPE::FUNCTION:
			name = "";
			menu_options.push_back(MenuOption(std::string("Cosine"), &AnimationManager::clickNewCos, true, false));
			menu_options.push_back(MenuOption(std::string("Sine"), &AnimationManager::clickNewSin, true, false));
			menu_options.push_back(MenuOption(std::string("Log"), &AnimationManager::clickNewLog, true, false));
			menu_options.push_back(MenuOption(std::string("Tangent"), &AnimationManager::clickNewTan, true, false));
			break;
	}
	height = gui.menu_text_height;
	//get thiccest menu option
	menu_options_width = 0;
	for (std::vector<MenuOption>::iterator it = menu_options.begin(); it != menu_options.end(); ++it){
		text = sf::Text((*it).name,gui.font,height);
		unsigned int width = (unsigned int)text.findCharacterPos((*it).name.length()).x;
		if (width > menu_options_width) menu_options_width = width;
	}
	menu_options_width += gui.outline_thickness*4;
	pos = new_pos;
	int submenu_index = 0;
	sf::Vector2i new_menu_pos = pos + sf::Vector2i(menu_options_width, 0);
	for (std::vector<MenuOption>::iterator it = menu_options.begin(); it != menu_options.end(); ++it) {
		if ((*it).has_submenu) {
			switch (type) {
			case MENU_TYPE::NEW:

				Menu * new_menu;
				switch (submenu_index) {
				case 0:
					new_menu = new Menu();
					new_menu->initialize(MENU_TYPE::COMPARATOR, new_menu_pos);
					submenus.push_back(new_menu);
					break;
				case 1:
					new_menu = new Menu();
					new_menu->initialize(MENU_TYPE::OPERATOR, new_menu_pos);
					submenus.push_back(new_menu);
					break;
				case 2:
					new_menu = new Menu();
					new_menu->initialize(MENU_TYPE::NUMBER, new_menu_pos);
					submenus.push_back(new_menu);
					break;
				case 3:
					new_menu = new Menu();
					new_menu->initialize(MENU_TYPE::FUNCTION, new_menu_pos);
					submenus.push_back(new_menu);
					break;
				default:
					break;
				}
				break;
			}
			submenu_index++;
			new_menu_pos = new_menu_pos + sf::Vector2i(0, height + gui.outline_thickness * 4);
		}
	}
	text = sf::Text(name,gui.font,gui.menu_text_height);
	text.setPosition(new_pos.x+gui.outline_thickness*2,new_pos.y+gui.outline_thickness*2);
	text.setFillColor(sf::Color::White);
	rect = sf::RectangleShape(sf::Vector2f(text.findCharacterPos(name.length()).x-pos.x+gui.outline_thickness*2,height+gui.outline_thickness*4));
	rect.setPosition(new_pos.x,new_pos.y);
	rect.setFillColor(sf::Color(127,127,127));
	rect.setOutlineThickness(-gui.outline_thickness);
	rect.setOutlineColor(sf::Color(63,63,63));
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states){
	if (name.length() > 0) {
		target.draw(rect);
		target.draw(text);
	}
	sf::Vector2i temp_pos = sf::Vector2i(pos.x,pos.y+height+gui.outline_thickness*4);
	if (is_open){
		for (std::vector<MenuOption>::iterator it = menu_options.begin(); it != menu_options.end(); ++it){
			sf::Text temp_text((*it).name,gui.font,height);
			temp_text.setPosition(temp_pos.x+gui.outline_thickness*2,temp_pos.y+gui.outline_thickness*2);
			temp_text.setFillColor(sf::Color::White);
			sf::RectangleShape temp_rect(sf::Vector2f(menu_options_width,height+gui.outline_thickness*4));
			temp_rect.setPosition(temp_pos.x,temp_pos.y);
			(*it).enabled ? temp_rect.setFillColor(sf::Color(127,127,127)) : temp_rect.setFillColor(sf::Color(95,95,95));
			temp_rect.setOutlineThickness(-gui.outline_thickness);
			temp_rect.setOutlineColor(sf::Color(63,63,63));
			target.draw(temp_rect);
			target.draw(temp_text);
			temp_pos = sf::Vector2i(temp_pos.x,temp_pos.y+height+gui.outline_thickness*4);
		}
	}
	for (std::vector<Menu *>::iterator it = submenus.begin(); it != submenus.end(); ++it) {
		if ((*it)->is_open)
			(*it)->draw(target, states);
	}
}

void Menu::update(sf::Vector2i mouse_pos){
	if (is_open){
		if ((mouse_pos.x >= pos.x
			&& mouse_pos.y >= pos.y
			&& mouse_pos.x < pos.x + rect.getSize().x
			&& mouse_pos.y < pos.y + rect.getSize().y)
			||
			(mouse_pos.x >= pos.x
			&& mouse_pos.y >= pos.y + (height+gui.outline_thickness*4)
			&& mouse_pos.x < pos.x + menu_options_width
			&& mouse_pos.y < pos.y + (menu_options.size()+1) * (height+gui.outline_thickness*4))) return;
		else {
			is_open = false;
			for (std::vector<Menu *>::iterator it = submenus.begin(); it != submenus.end(); ++it) {
				(*it)->update(mouse_pos);
				if ((*it)->is_open) {
					is_open = true;
				}
			}
		}
	}
	
}

bool Menu::processLeftClick(sf::Vector2i mouse_pos, AnimationManager * animation_manager){
	if (mouse_pos.x >= pos.x
		&& mouse_pos.y >= pos.y
		&& mouse_pos.x < pos.x + rect.getSize().x
		&& mouse_pos.y < pos.y + rect.getSize().y){
		//process the click
		std::cout << "clicked on " << name << std::endl;
		is_open = !is_open;
		if (!is_open){
			for (std::vector<Menu *>::iterator it = submenus.begin(); it != submenus.end(); ++it) {
				(*it)->is_open = false;
			}
		}
		return true;
	}
	else {
		if (is_open){
			int ctr = 1;
			std::vector<Menu *>::iterator submenu_it = submenus.begin();
			for (std::vector<MenuOption>::iterator it = menu_options.begin(); it != menu_options.end(); ++it){
				if (!(*it).enabled){
					ctr++;
					continue;
				}
				if (mouse_pos.x >= pos.x
				    && mouse_pos.y >= pos.y + ctr * (height+gui.outline_thickness*4)
					&& mouse_pos.x < pos.x + menu_options_width
					&& mouse_pos.y < pos.y + (ctr+1) * (height+gui.outline_thickness*4)){
						void (AnimationManager::*clickFunc)() = (*it).clickFunc;
						if (clickFunc != NULL) {
							(animation_manager->*clickFunc)();
							is_open = false;
							return true;
						}
						else if ((*it).has_submenu){
							(*submenu_it)->is_open = !(*submenu_it)->is_open;
							for (std::vector<Menu *>::iterator submenu_it_other = submenus.begin(); submenu_it_other != submenus.end(); ++submenu_it_other) {
								if (*submenu_it != *submenu_it_other) (*submenu_it_other)->is_open = false;
							}
							return true;
						}
						else {
							//here if I haven't made a callback function for this bad boi yet
							is_open = false;
							return true;
						}
				}
				if ((*it).has_submenu) ++submenu_it;
				ctr++;
			}
			//check if the click is in the menu dropdown
			//check if the click is in an open submenu dropdown
			for (std::vector<Menu *>::iterator submenu_it = submenus.begin(); submenu_it != submenus.end(); ++submenu_it) {
				if ((*submenu_it)->processLeftClick(mouse_pos, animation_manager)) {
					return true;
				}
			}
			return false;
		}
		return false;
	}
}

unsigned int Menu::get_width(){
	return (unsigned int)(rect.getSize().x);
}