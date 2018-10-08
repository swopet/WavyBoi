#include "Menu.h"

Menu::Menu(){
	
}

void Menu::initialize(MENU_TYPE new_type,sf::Font * new_font,sf::Vector2i new_pos,unsigned int new_height){
	type = new_type;
	switch(type){
		case MENU_TYPE::FILE:
			name = "File";
			menu_options.push_back((MenuOption){std::string("New"),&AnimationManager::clickFileNew});
			menu_options.push_back((MenuOption){std::string("Open"),&AnimationManager::clickFileOpen});
			menu_options.push_back((MenuOption){std::string("Save"),&AnimationManager::clickFileSave});
			menu_options.push_back((MenuOption){std::string("Save As"),&AnimationManager::clickFileSaveAs});
		break;
		case MENU_TYPE::EDIT:
			name = "Edit";
			menu_options.push_back((MenuOption){std::string("Cut"),&AnimationManager::clickEditCut});
			menu_options.push_back((MenuOption){std::string("Copy"),&AnimationManager::clickEditCopy});
			menu_options.push_back((MenuOption){std::string("Paste"),&AnimationManager::clickEditPaste});
			menu_options.push_back((MenuOption){std::string("Delete"),&AnimationManager::clickEditDelete});
		break;
	}
	font = new_font;
	height = new_height;
	//get thiccest menu option
	menu_options_width = 0;
	for (std::vector<MenuOption>::iterator it = menu_options.begin(); it != menu_options.end(); ++it){
		text = sf::Text((*it).name,*font,height);
		unsigned int width = text.findCharacterPos((*it).name.length()).x;
		if (width > menu_options_width) menu_options_width = width;
	}
	menu_options_width += MENU_BORDER_THICKNESS*4;
	pos = new_pos;
	text = sf::Text(name,*font,new_height);
	text.setPosition(new_pos.x+MENU_BORDER_THICKNESS*2,new_pos.y+MENU_BORDER_THICKNESS*2);
	text.setColor(sf::Color::White);
	rect = sf::RectangleShape(sf::Vector2f(text.findCharacterPos(name.length()).x-pos.x+MENU_BORDER_THICKNESS*2,height+MENU_BORDER_THICKNESS*4));
	rect.setPosition(new_pos.x,new_pos.y);
	rect.setFillColor(sf::Color(127,127,127));
	rect.setOutlineThickness(-MENU_BORDER_THICKNESS);
	rect.setOutlineColor(sf::Color(63,63,63));
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states){
	target.draw(rect);
	target.draw(text);
	sf::Vector2i temp_pos = sf::Vector2i(pos.x,pos.y+height+MENU_BORDER_THICKNESS*4);
	if (is_open){
		for (std::vector<MenuOption>::iterator it = menu_options.begin(); it != menu_options.end(); ++it){
			sf::Text temp_text((*it).name,*font,height);
			temp_text.setPosition(temp_pos.x+MENU_BORDER_THICKNESS*2,temp_pos.y+MENU_BORDER_THICKNESS*2);
			temp_text.setColor(sf::Color::White);
			sf::RectangleShape temp_rect(sf::Vector2f(menu_options_width,height+MENU_BORDER_THICKNESS*4));
			temp_rect.setPosition(temp_pos.x,temp_pos.y);
			temp_rect.setFillColor(sf::Color(127,127,127));
			temp_rect.setOutlineThickness(-MENU_BORDER_THICKNESS);
			temp_rect.setOutlineColor(sf::Color(63,63,63));
			target.draw(temp_rect);
			target.draw(temp_text);
			temp_pos = sf::Vector2i(temp_pos.x,temp_pos.y+height+MENU_BORDER_THICKNESS*4);
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
			&& mouse_pos.y >= pos.y + (height+MENU_BORDER_THICKNESS*4)
			&& mouse_pos.x < pos.x + menu_options_width
			&& mouse_pos.y < pos.y + (menu_options.size()+1) * (height+MENU_BORDER_THICKNESS*4))) return;
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
				if (mouse_pos.x >= pos.x
				    && mouse_pos.y >= pos.y + ctr * (height+MENU_BORDER_THICKNESS*4)
					&& mouse_pos.x < pos.x + menu_options_width
					&& mouse_pos.y < pos.y + (ctr+1) * (height+MENU_BORDER_THICKNESS*4)){
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