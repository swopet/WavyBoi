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
	sf::RectangleShape main_box;
	sf::Text text;
	bool changed = false;
public:
	FreqBandBlock();
	~FreqBandBlock();
	Parameter getParameter();
	void update();
	void updateValsFromHandler(AudioHandler *);
	void sendRangeToHandler(AudioHandler *);
	sf::Vector2f getRightPos();
	Parameter * getNewParameter() {
		param new_param;
		new_param.float_val = 0;
		return new Parameter(PARAM_TYPE::FLOAT, new_param, name);
	}
	void draw(sf::RenderTarget&, sf::RenderStates);
	bool checkOverlap(sf::RectangleShape select_box);
	ClickResponse processLeftClick(sf::Vector2i);
	ClickResponse processMouseWheel(sf::Vector2i mouse_pos, int delta);
};

