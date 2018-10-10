#pragma once
#include <pch.h>
#include <string>
#include <SFML/Graphics.hpp>

enum class PARAM_TYPE {
	TEXTURE,
	TEXT,
	FLOAT,
	INT
};

typedef union {
	const sf::Texture * texture;
	std::string * text;
	float float_val;
	int int_val;
} param;

class Parameter{
private:
	param value;
	std::string name;
public:
	Parameter();
};