#include "Object/Object.h"

std::string Object::getName(){
	return name;
}

void Object::setVisible(bool new_visibility){
	visible = new_visibility;
}

void Object::move(sf::Vector2f offset){
	center = center + offset;
}