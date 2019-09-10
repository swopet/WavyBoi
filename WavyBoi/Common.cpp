#include <pch.h>
#include <Common.h>

bool checkIntersection(sf::FloatRect rect1, sf::FloatRect rect2) {
	return ((rect1.left < rect2.left) && 
		(rect1.left + rect1.width > rect2.left) &&
		(rect1.top < rect2.top+rect2.height)) && 
		(rect1.top + rect1.height > rect2.top);
}

bool checkIntersection(sf::FloatRect rect, sf::Vector2f point1, sf::Vector2f point2) {
	sf::Vector2f p1(rect.left,rect.top);
	sf::Vector2f p3(rect.left+rect.width,rect.top+rect.height);
	sf::Vector2f p2 = sf::Vector2f(p3.x, p1.y);
	sf::Vector2f p4 = sf::Vector2f(p1.x, p3.y);
	return (checkIntersection(point1, point2, p1, p2) ||
		checkIntersection(point1, point2, p2, p3) ||
		checkIntersection(point1, point2, p3, p4) ||
		checkIntersection(point1, point2, p4, p1)
	);
}

bool checkIntersection(sf::Vector2f line1_point1, sf::Vector2f line1_point2, sf::Vector2f line2_point1, sf::Vector2f line2_point2) {
	float x1, x2, x3, x4, y1, y2, y3, y4;
	x1 = line1_point1.x;
	x2 = line1_point2.x;
	x3 = line2_point1.x;
	x4 = line2_point2.x;
	y1 = line1_point1.y;
	y2 = line1_point2.y;
	y3 = line2_point1.y;
	y4 = line2_point2.y;
	if (((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1)) == 0) return false;
	float uA = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
	float uB = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
	return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1);
}

bool checkIntersection(sf::FloatRect rect, sf::Vector2f pos) {
	return (pos.x >= rect.left
		&& pos.y >= rect.top
		&& pos.x < rect.left + rect.width
		&& pos.y < rect.top + rect.height);
}

bool checkIntersection(sf::FloatRect rect, sf::Vector2i pos) {
	return checkIntersection(rect, sf::Vector2f(pos));
}

float length(sf::Vector2f vec) {
	return sqrtf(vec.x*vec.x + vec.y*vec.y);
}

float length(sf::Vector3f vec) {
	return sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}