#pragma once
#include "Object/Object.h"
#include <sstream>
class FloatObject :
	public Object
{
private:
	param out_val;
	param default_val;
	sf::Text text;
	int precision = 2;
public:
	void processNewString(std::string field, std::string input);
	Parameter getParameter();
	bool getMultipleInputsAllowed(int) { return false; }
	void draw(sf::RenderTarget&, sf::RenderStates);
	void initializeElements();
	void updateGUIElements();
	void setParameter(Parameter * parameter, int ind);
	void setParamsToDefault() { out_val = default_val; }
	void setPrecision(int);
	virtual Parameter * getNewParameter();
	ClickResponse processDoubleLeftClick(sf::Vector2i mouse_pos);
	ClickResponse processMouseWheel(sf::Vector2i mouse_pos, int delta);
	FloatObject();
	FloatObject(float);
	~FloatObject();
};

