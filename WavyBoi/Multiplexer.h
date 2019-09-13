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
	void initializeElements();
	void updateGUIElements();
	~Multiplexer();
	Parameter getParameter();
	void setParameter(Parameter *, int);
	Parameter * getNewParameter() {
		return new Parameter(out_type,curr_value,name);
	}
	bool getMultipleInputsAllowed(int);
	virtual ClickResponse processLeftClick(sf::Vector2i);
};

