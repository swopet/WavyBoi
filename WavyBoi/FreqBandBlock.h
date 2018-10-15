#pragma once
#include "Object/Object.h"
#include <AudioHandler.h>
extern AudioHandler * audio_handler;

class FreqBandBlock :
	public Object
{
private:
	int freq_band = 0;
public:
	FreqBandBlock();
	~FreqBandBlock();
	param getVal();
	sf::Vector2f getRightPos();
	Parameter * getNewParameter() {
		param new_param;
		new_param.float_val = 0;
		return new Parameter(PARAM_TYPE::FLOAT, new_param, name);
	}
	void draw(sf::RenderTarget&, sf::RenderStates);
	ClickResponse processLeftClick(sf::Vector2i);
};

