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

sf::Vector2f Object::getLeftPos()
{
	return center;
}

sf::Vector2f Object::getRightPos()
{
	return center;
}

void Object::move(sf::Vector2f offset){
	center = center + offset;
}


