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
	target.draw(rect);
	target.draw(text);
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
		else is_open = false;
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
		return true;
	}
	else {
		if (is_open){
			int ctr = 1;
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
						(animation_manager->*clickFunc)();
						is_open = false;
						return true;
				}
				ctr++;	
			}
			//check if the click is in the menu dropdown
			return false;
		}
		return false;
	}
}

unsigned int Menu::get_width(){
	return (unsigned int)(rect.getSize().x);
}