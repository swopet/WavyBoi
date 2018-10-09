#include "Video.h"

Video::Video(){
	name = "unnamed video";
	type = OBJECT_TYPE::VIDEO;
	size = sf::Vector2f(80,60);
	center = sf::Vector2f(100,100);
	init();
}

void Video::init(){
	main_box = sf::RectangleShape(size);
	main_box.setOutlineThickness(-VIDEO_OUTLINE_THICKNESS);
	main_box.setOutlineColor(sf::Color(191,191,191));
	main_box.setFillColor(sf::Color(0,0,0));
}

void Video::draw(sf::RenderTarget& target, sf::RenderStates states){
	main_box.setPosition(center - size/2.0f);
	target.draw(main_box);
}

ClickResponse Video::processLeftClick(sf::Vector2i mouse_pos){
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (mouse_pos.x >= center.x - size.x/2.0f &&
		mouse_pos.y >= center.y - size.y/2.0f &&
		mouse_pos.x < center.x + size.x/2.0f &&
		mouse_pos.y < center.y + size.y/2.0f){
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
	}
	else {
		
	}
	return response;
}