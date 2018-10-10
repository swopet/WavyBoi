#include <pch.h>
#include <Object\Link.h>

Link::Link(Object * obj_in, Object *obj_out, Parameter *new_parameter)
{
	in = obj_in;
	out = obj_out;
	parameter = new_parameter;
}

Link::Link(){
	type = OBJECT_TYPE::LINK;
}

Link::~Link(){
	
}

void Link::update()
{
	parameter->setValue(in->getVal());
}

Parameter * Link::getParameterFromLink()
{
	return parameter;
}

void Link::draw(sf::RenderTarget& target, sf::RenderStates states){
	
}

ClickResponse Link::processLeftClick(sf::Vector2i mouse_pos){
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	
	return response;
}