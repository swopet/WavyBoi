#include "VideoLoop.h"

VideoLoop::VideoLoop(){
	type = OBJECT_TYPE::VIDEO_LOOP;
}

VideoLoop::~VideoLoop(){
	
}

void VideoLoop::draw(sf::RenderTarget& target, sf::RenderStates states){
	
}

ClickResponse VideoLoop::processLeftClick(sf::Vector2i mouse_pos){
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	
	return response;
}