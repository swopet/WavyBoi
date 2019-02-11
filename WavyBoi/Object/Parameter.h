#pragma once
#include <string>
#include <SFML/Graphics.hpp>

enum class PARAM_TYPE {
	TEXTURE,
	TEXT,
	FLOAT,
	INT,
	NONE
};

typedef union {
	const sf::Texture * texture;
	std::string * text;
	float float_val;
	int int_val;
} param;

class Parameter{
private:
	PARAM_TYPE type = PARAM_TYPE::NONE;
	param value;
	std::string name;
public:
	PARAM_TYPE getType();
	param getValue();
	std::string getName();
	void setValue(param);
	void setType(PARAM_TYPE new_type);
	void setName(std::string new_name);
	Parameter(PARAM_TYPE, param, std::string);
	Parameter();
};