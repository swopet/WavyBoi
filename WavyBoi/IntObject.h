#pragma once
#include "Object/Object.h"
class IntObject :
	public Object
{
private:
	param out_val;
	param default_val;
	sf::Vector2f left_pos;
	sf::Vector2f right_pos;
	sf::RectangleShape main_box;
	sf::Text text;
public:
	void processNewString(std::string field, std::string input);
	bool getMultipleInputsAllowed(int) { return false; }
	void draw(sf::RenderTarget&, sf::RenderStates);
	void update();
	void setParameter(Parameter * parameter, int ind);
	void setParamsToDefault() { out_val = default_val; }
	param getVal() { return out_val; }
	sf::Vector2f getLeftPos(int) { return left_pos; }
	sf::Vector2f getRightPos() { return right_pos; }
	ClickResponse processLeftClick(sf::Vector2i vec);
	ClickResponse processLeftClickRelease(sf::Vector2i vec);
	ClickResponse processDoubleLeftClick(sf::Vector2i mouse_pos);
	IntObject();
	IntObject(int);
	~IntObject();
};

