#include "pch.h"
#include "FreqBandBlock.h"


FreqBandBlock::FreqBandBlock()
{
	type = OBJECT_TYPE::FREQBANDBLOCK;
	initializeElements();
}

void FreqBandBlock::initializeElements() {
	GUIElement main_rect(GUIElement::RECTANGLE, 1, 1);
	main_rect.setPosition(sf::Vector2f(0, 0));
	elements.push_back(main_rect);
	GUIElement output_circle(GUIElement::CIRCLE, gui.obj_circle_radius);
	outputs.push_back(output_circle);
	updateGUIElements();
}

void FreqBandBlock::updateGUIElements() {
	std::ostringstream range_str;
	range_str << low_val << " - " << high_val << " Hz";
	text.setString(range_str.str());
	text.setCharacterSize(gui.input_text_height);
	text.setFont(gui.font);
	text.setFillColor(sf::Color::White);
	sf::FloatRect text_rect = text.getLocalBounds();
	elements.at(0).setSize(text_rect.width + text_rect.left + gui.text_buffer*2.0, text_rect.height + text_rect.top + gui.text_buffer*2.0);
	text.setPosition(position + sf::Vector2f(gui.text_buffer, gui.text_buffer) -
		sf::Vector2f(text_rect.left / 2.0, text_rect.top / 2.0));
	outputs.at(0).setPosition(sf::Vector2f(elements.at(0).getGlobalBounds().width, elements.at(0).getGlobalBounds().height/2.0)+
		sf::Vector2f(gui.outline_thickness/2.0, 0) + sf::Vector2f(-gui.obj_circle_radius,-gui.obj_circle_radius));
	Object::updateGUIElements();
}

FreqBandBlock::~FreqBandBlock()
{
}

Parameter FreqBandBlock::getParameter()
{
	return Parameter(PARAM_TYPE::FLOAT, out_val, name);
}

void FreqBandBlock::update()
{
	out_val.float_val = max;
	
	changed = false;
}

void FreqBandBlock::setRange(std::pair<int, int> range)
{
	low_val = range.first;
	high_val = range.second;
}

void FreqBandBlock::updateValsFromHandler(AudioHandler * audio_handler)
{
	max = audio_handler->getMaxAtRange(std::pair<int,int>(low_val, high_val));
	avg = audio_handler->getAvgAtRange(std::pair<int, int>(low_val, high_val));
}

void FreqBandBlock::sendRangeToHandler(AudioHandler * audio_handler)
{
	audio_handler->addRange(std::pair<int, int>(low_val, high_val));
}

void FreqBandBlock::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	Object::draw(target, states);
	sf::RectangleShape level_box(sf::Vector2f(elements.at(0).getGlobalBounds().width, elements.at(0).getGlobalBounds().height)
		+ sf::Vector2f(gui.outline_thickness * 2, gui.outline_thickness * 2));
	level_box.setFillColor(sf::Color(0, 255 * max, 0));
	float max_height = elements.at(0).getGlobalBounds().height + gui.outline_thickness * 2;
	level_box.setSize(sf::Vector2f(level_box.getSize().x, -max_height * max));
	level_box.setPosition(sf::Vector2f(elements.at(0).getGlobalBounds().left, elements.at(0).getGlobalBounds().top + elements.at(0).getGlobalBounds().height) +
		sf::Vector2f(-gui.outline_thickness, gui.outline_thickness));
	target.draw(level_box, states);
	target.draw(text, states);
}

bool FreqBandBlock::checkOverlap(sf::RectangleShape select_box) {
	return checkIntersection(select_box.getGlobalBounds(), elements.at(0).getGlobalBounds());
}

ClickResponse FreqBandBlock::processMouseWheel(sf::Vector2i mouse_pos, int delta)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (checkIntersection(elements.at(0).getGlobalBounds(), sf::Vector2f(mouse_pos))) {
		if (mouse_pos.x <= position.x + elements.at(0).getGlobalBounds().width/2.0f) {
			low_val += delta;
			if (low_val > high_val) low_val = high_val;
			if (low_val < 0) low_val = 0;
		}
		else {
			high_val += delta;
			if (high_val > 24000) high_val = 24000;
			if (high_val < low_val) high_val = low_val;
		}
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
		return response;
	}
	return response;
}
