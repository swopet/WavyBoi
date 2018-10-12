#pragma once
#include <pch.h>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
enum class CLICK_RESPONSE {
	NONE,
	PROCESSED,
	SELECTED,
	GOT_RIGHT,
	GOT_LEFT
};

struct ClickResponse {
	bool clicked;
	CLICK_RESPONSE type;
};


bool checkIntersection(sf::RectangleShape rec1, sf::RectangleShape rec2);

bool checkIntersection(sf::RectangleShape rect, sf::Vector2f point1, sf::Vector2f point2);

bool checkIntersection(sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f);

float length(sf::Vector2f);

float length(sf::Vector3f);