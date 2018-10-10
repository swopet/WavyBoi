#include <pch.h>
#include <Object\Link.h>

Link::Link(){
	type = OBJECT_TYPE::LINK;
}

Link::~Link(){
	
}

void Link::draw(sf::RenderTarget& target, sf::RenderStates states){
	
}

ClickResponse Link::processLeftClick(sf::Vector2i mouse_pos){
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	
	return response;
}