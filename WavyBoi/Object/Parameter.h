#pragma once
#include <string>
#include <SFML/Graphics.hpp>

enum class PARAM_TYPE {
	VIDEO_TEXTURE,
    SHADER_TEXTURE,
	TEXT,
	FLOAT,
	INT,
	COLOR_RGBA,
	COLOR_HSVA,
	NONE
};

struct color_rgba {
	double r;
	double g;
	double b;
	double a;
};

struct color_hsva {
	double h;
	double s;
	double v;
	double a;
};

typedef union {
	const sf::Texture * texture;
	std::string * text;
	float float_val;
	int int_val;
	color_rgba rgba_val;
	color_hsva hsva_val;
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