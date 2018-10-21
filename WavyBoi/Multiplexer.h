#pragma once
#include "Object/Object.h"
constexpr auto SELECTOR_BOX_SIZE = 40;

class Multiplexer :
	public Object
{
private:
	int selections;
	int curr_selection = 1;
	Parameter ** params;
	PARAM_TYPE out_type = PARAM_TYPE::NONE;
	param curr_value;
public:
	Multiplexer();
	~Multiplexer();
	Parameter getParameter();
	void setParameter(Parameter *, int);
	Parameter * getNewParameter() {
		return new Parameter(out_type,curr_value,name);
	}
	bool getMultipleInputsAllowed(int);
	sf::Vector2f getLeftPos(int);
	sf::Vector2f getRightPos();
	void draw(sf::RenderTarget&, sf::RenderStates);
	virtual ClickResponse processLeftClick(sf::Vector2i);
	virtual ClickResponse processLeftClickHeld(sf::Vector2i);
	virtual ClickResponse processLeftClickRelease(sf::Vector2i);
};

