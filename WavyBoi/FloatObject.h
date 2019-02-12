#pragma once
#include "Object/Object.h"
#include <sstream>
class FloatObject :
	public Object
{
private:
	param out_val;
	param default_val;
	sf::Vector2f left_pos;
	sf::Vector2f right_pos;
	sf::RectangleShape main_box;
	sf::Text text;
	int precision = 2;
public:
	void processNewString(std::string field, std::string input);
	Parameter getParameter();
	bool getMultipleInputsAllowed(int) { return false; }
	void draw(sf::RenderTarget&, sf::RenderStates);
	void update();
	void setParameter(Parameter * parameter, int ind);
	void setParamsToDefault() { out_val = default_val; }
	void setPrecision(int);
	virtual Parameter * getNewParameter();
	bool checkOverlap(sf::RectangleShape);
	sf::Vector2f getLeftPos(int) { return left_pos; }
	sf::Vector2f getRightPos() { return right_pos; }
	ClickResponse processLeftClick(sf::Vector2i vec);
	ClickResponse processLeftClickRelease(sf::Vector2i vec);
	ClickResponse processDoubleLeftClick(sf::Vector2i mouse_pos);
	ClickResponse processMouseWheel(sf::Vector2i mouse_pos, int delta);
	FloatObject();
	FloatObject(float);
	~FloatObject();
};

