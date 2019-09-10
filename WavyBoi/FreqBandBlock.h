#pragma once
#include "Object/Object.h"
#include <AudioHandler.h>
#include <sstream>

class FreqBandBlock :
	public Object
{
private:
	int low_val = 0;
	int high_val = 60;
	float max = 0.0;
	float avg = 0.0;
	param out_val;
	sf::Text text;
	bool changed = false;
public:
	FreqBandBlock();
	void initializeElements();
	void updateGUIElements();
	~FreqBandBlock();
	Parameter getParameter();
	void update();
	void setRange(std::pair<int, int>);
	void updateValsFromHandler(AudioHandler *);
	void sendRangeToHandler(AudioHandler *);
	Parameter * getNewParameter() {
		param new_param;
		new_param.float_val = 0;
		return new Parameter(PARAM_TYPE::FLOAT, new_param, name);
	}
	void draw(sf::RenderTarget&, sf::RenderStates);
	bool checkOverlap(sf::RectangleShape select_box);
	ClickResponse processMouseWheel(sf::Vector2i mouse_pos, int delta);
};

