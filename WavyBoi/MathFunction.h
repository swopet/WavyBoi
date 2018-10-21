#pragma once
#include "Object/Object.h"

class MathFunction :
	public Object
{
private:
	int func = 0;
	param out_val;
	param in_val;
	sf::RectangleShape main_box;
	sf::Text text;
	const std::string func_strings[4] = { "Cos", "Sin", "Tan", "Log" };
public:
	MathFunction(int);
	MathFunction();
	~MathFunction();
	void setParameter(Parameter * parameter, int ind);
	Parameter getParameter();
	void update();
	sf::Vector2f getLeftPos(int ind);
	sf::Vector2f getRightPos();
	Parameter * getNewParameter() {
		param new_param;
		new_param.float_val = 0;
		return new Parameter(PARAM_TYPE::FLOAT, new_param, name);
	}
	void draw(sf::RenderTarget&, sf::RenderStates);
	ClickResponse processLeftClick(sf::Vector2i);
	ClickResponse processLeftClickRelease(sf::Vector2i mouse_pos);
	ClickResponse processMouseWheel(sf::Vector2i mouse_pos, int delta);
};

