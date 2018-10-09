#include "Image.h"

Image::Image(){
	type = OBJECT_TYPE::IMAGE;
}

Image::~Image(){
	
}

void Image::draw(sf::RenderTarget& target, sf::RenderStates states){
	
}

ClickResponse Image::processLeftClick(sf::Vector2i mouse_pos){
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	
	return response;
}