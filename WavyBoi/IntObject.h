#pragma once
#include "Object/Object.h"
class IntObject :
	public Object
{
private:
	param out_val;
	param default_val;
	sf::Text text;
public:
	void processNewString(std::string field, std::string input);
	Parameter getParameter();
	bool getMultipleInputsAllowed(int) { return false; }
	void draw(sf::RenderTarget&, sf::RenderStates);
	void initializeElements();
	void updateGUIElements();
	void setParameter(Parameter * parameter, int ind);
	bool checkOverlap(sf::RectangleShape select_box);
	void setParamsToDefault() { out_val = default_val; }
	ClickResponse processDoubleLeftClick(sf::Vector2i mouse_pos);
	IntObject();
	IntObject(int);
	~IntObject();
};

