#pragma once
#include "Object/Object.h"
#include "Object/Parameter.h"
#include "WBScene.h"

class Channel :
	public Object
{
private:
	const sf::Texture * render_texture = NULL;
	int id;
public:
	void setParamsToDefault();
	void setParameter(Parameter *, int);
	void clearTexture();
	void update();
	bool checkOverlap(sf::RectangleShape select_box);
	bool getMultipleInputsAllowed(int);
	std::vector<PARAM_TYPE> getParamTypesForInput(int);
	const sf::Texture * getTexture();
	void initializeElements();
	void updateGUIElements();
	Channel(int);
	Channel();
	~Channel();
};

