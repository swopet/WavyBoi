#include <pch.h>
#include <Common.h>

sf::Vector2f getTopLeft(sf::RectangleShape rect) {
	return sf::Vector2f(rect.getSize().x >= 0 ? rect.getPosition().x : (rect.getPosition().x + rect.getSize().x), rect.getSize().y >= 0 ? rect.getPosition().y : (rect.getPosition().y + rect.getSize().y));
}

sf::Vector2f getBottomRight(sf::RectangleShape rect) {
	return sf::Vector2f(rect.getSize().x < 0 ? rect.getPosition().x : (rect.getPosition().x + rect.getSize().x), rect.getSize().y < 0 ? rect.getPosition().y : (rect.getPosition().y + rect.getSize().y));
}

bool checkIntersection(sf::RectangleShape rect1, sf::RectangleShape rect2) {
	sf::Vector2f rec1_topleft = getTopLeft(rect1);
	sf::Vector2f rec1_bottomright = getBottomRight(rect1);
	sf::Vector2f rec2_topleft = getTopLeft(rect2);
	sf::Vector2f rec2_bottomright = getBottomRight(rect2);
	return (rec1_topleft.x < rec2_bottomright.x && rec1_bottomright.x > rec2_topleft.x &&
		rec1_topleft.y < rec2_bottomright.y && rec1_bottomright.y > rec2_topleft.y);
}

float length(sf::Vector2f vec) {
	return sqrtf(vec.x*vec.x + vec.y*vec.y);
}

float length(sf::Vector3f vec) {
	return sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}