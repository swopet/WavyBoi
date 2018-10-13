#pragma once
#include <pch.h>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <string>
constexpr auto TEXTURE_PATH = "C:/Users/Trevor/Stuff/VS/WavyBoi/resources/textures/";
enum class CLICK_RESPONSE {
	NONE,
	PROCESSED,
	DELETED_MUX_INPUT,
	SELECTED,
	GOT_RIGHT,
	GOT_LEFT
};

struct ClickResponse {
	bool clicked = false;
	CLICK_RESPONSE type = CLICK_RESPONSE::NONE;
	int ind = 0;
};

extern sf::Texture add_20x20_tex;
extern sf::Texture delete_20x20_tex;
void loadApplicationTextures();

bool checkIntersection(sf::RectangleShape rec1, sf::RectangleShape rec2);

bool checkIntersection(sf::RectangleShape rect, sf::Vector2f point1, sf::Vector2f point2);

bool checkIntersection(sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f);

float length(sf::Vector2f);

float length(sf::Vector3f);