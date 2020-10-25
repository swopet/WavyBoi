#include <pch.h>
#include <Object\Object.h>

std::string Object::getName(){
	return name;
}

void Object::setVisible(bool new_visibility){
	visible = new_visibility;
}

bool Object::checkOverlap(sf::RectangleShape select_box)
{
    for (auto it = elements.begin(); it != elements.end(); ++it) {
      if(checkIntersection(select_box.getGlobalBounds(), it->getGlobalBounds())) return true;
    }
	return false;
}


