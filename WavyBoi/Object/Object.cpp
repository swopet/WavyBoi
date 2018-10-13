#include <pch.h>
#include <Object\Object.h>

std::string Object::getName(){
	return name;
}

void Object::setVisible(bool new_visibility){
	visible = new_visibility;
}

bool Object::checkOverlap(sf::RectangleShape)
{
	return false;
}


