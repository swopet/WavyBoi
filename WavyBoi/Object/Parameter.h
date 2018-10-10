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
	PARAM_TYPE type;
	param value;
	std::string name;
public:
	PARAM_TYPE getType();
	param getValue();
	std::string getName();
	void setValue(param);
	Parameter(PARAM_TYPE, param, std::string);
	Parameter();
};