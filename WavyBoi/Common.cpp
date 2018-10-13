#include <pch.h>
#include <Common.h>

sf::Vector2f getTopLeft(sf::RectangleShape rect) {
	return sf::Vector2f(rect.getSize().x >= 0 ? rect.getPosition().x : (rect.getPosition().x + rect.getSize().x), rect.getSize().y >= 0 ? rect.getPosition().y : (rect.getPosition().y + rect.getSize().y));
}

sf::Vector2f getBottomRight(sf::RectangleShape rect) {
	return sf::Vector2f(rect.getSize().x < 0 ? rect.getPosition().x : (rect.getPosition().x + rect.getSize().x), rect.getSize().y < 0 ? rect.getPosition().y : (rect.getPosition().y + rect.getSize().y));
}

sf::Texture add_20x20_tex;
sf::Texture delete_20x20_tex;

void loadApplicationTextures()
{
	add_20x20_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("add_20x20.png"));
	delete_20x20_tex.loadFromFile(std::string(TEXTURE_PATH) + std::string("delete_20x20.png"));
}

bool checkIntersection(sf::RectangleShape rect1, sf::RectangleShape rect2) {
	sf::Vector2f rec1_topleft = getTopLeft(rect1);
	sf::Vector2f rec1_bottomright = getBottomRight(rect1);
	sf::Vector2f rec2_topleft = getTopLeft(rect2);
	sf::Vector2f rec2_bottomright = getBottomRight(rect2);
	return (rec1_topleft.x < rec2_bottomright.x && rec1_bottomright.x > rec2_topleft.x &&
		rec1_topleft.y < rec2_bottomright.y && rec1_bottomright.y > rec2_topleft.y);
}

bool checkIntersection(sf::RectangleShape rect, sf::Vector2f point1, sf::Vector2f point2) {
	sf::Vector2f p1 = getTopLeft(rect);
	sf::Vector2f p3 = getBottomRight(rect);
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
	if (((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1)) == 0) return true;
	float uA = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
	float uB = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
	return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1);
}

float length(sf::Vector2f vec) {
	return sqrtf(vec.x*vec.x + vec.y*vec.y);
}

float length(sf::Vector3f vec) {
	return sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}