#pragma once
#include "Object/Object.h"
#include <AudioHandler.h>
extern AudioHandler * audio_handler;

class FreqBandBlock :
	public Object
{
private:
	int freq_band = 0;
	param out_val;
	sf::RectangleShape main_box;
	sf::Text text;
	const std::string freq_band_strings[7] = { "Sub Bass", "Bass", "Low Mids", "Mids", "Upper Mids", "Presence", "Brilliance" };
public:
	FreqBandBlock();
	~FreqBandBlock();
	Parameter getParameter();
	void update();
	sf::Vector2f getRightPos();
	Parameter * getNewParameter() {
		param new_param;
		new_param.float_val = 0;
		return new Parameter(PARAM_TYPE::FLOAT, new_param, name);
	}
	void draw(sf::RenderTarget&, sf::RenderStates);
	ClickResponse processLeftClick(sf::Vector2i);
	ClickResponse processMouseWheel(sf::Vector2i mouse_pos, int delta);
};

