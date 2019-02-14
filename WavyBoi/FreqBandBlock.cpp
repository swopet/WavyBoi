#include "pch.h"
#include "FreqBandBlock.h"


FreqBandBlock::FreqBandBlock()
{
	type = OBJECT_TYPE::FREQBANDBLOCK;
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
	main_box.setSize(sf::Vector2f(96 + gui.outline_thickness * 2, gui.input_text_height + gui.outline_thickness * 4));
	main_box.setOutlineColor(gui.obj_outline_color);
	main_box.setOutlineThickness(-gui.outline_thickness);
	main_box.setFillColor(gui.obj_fill_color);
	main_box.setPosition(position);
	out_val.float_val = max;
	std::ostringstream range_str;
	range_str << low_val << " - " << high_val << " Hz";
	text.setString(range_str.str());
	text.setCharacterSize(gui.input_text_height);
	text.setFont(gui.font);
	text.setFillColor(sf::Color::White);
	text.setPosition(position + sf::Vector2f(gui.outline_thickness, gui.outline_thickness));
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

sf::Vector2f FreqBandBlock::getRightPos()
{
	return position + sf::Vector2f(main_box.getSize().x-gui.outline_thickness / 2, main_box.getSize().y/2);
}

void FreqBandBlock::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	target.draw(main_box, states);
	sf::RectangleShape level_box(main_box.getSize() - sf::Vector2f(gui.outline_thickness * 2, gui.outline_thickness * 2));
	
	level_box.setFillColor(sf::Color(0, 255 * max, 0));
	float max_height = level_box.getSize().y;
	level_box.setSize(sf::Vector2f(level_box.getSize().x, max_height * max));
	level_box.setPosition(main_box.getPosition() + sf::Vector2f(gui.outline_thickness, gui.outline_thickness + (1.0 - max) * max_height));
	target.draw(level_box, states);
	target.draw(text, states);
	sf::CircleShape right_circle(gui.obj_circle_radius + gui.outline_thickness);
	right_circle.setOutlineColor(gui.obj_outline_color);
	right_circle.setOutlineThickness(-gui.outline_thickness);
	right_circle.setFillColor(gui.obj_fill_color);
	right_circle.setPosition(getRightPos() - sf::Vector2f(right_circle.getRadius(),right_circle.getRadius()));
	target.draw(right_circle, states);
}

bool FreqBandBlock::checkOverlap(sf::RectangleShape select_box) {
	return checkIntersection(select_box, main_box);
}

ClickResponse FreqBandBlock::processLeftClick(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - getRightPos()) <= gui.obj_circle_radius + gui.outline_thickness) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
		return response;
	}
	if (checkIntersection(main_box, sf::Vector2f(mouse_pos))) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
		return response;
	}
	return response;
}

ClickResponse FreqBandBlock::processMouseWheel(sf::Vector2i mouse_pos, int delta)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (checkIntersection(main_box, sf::Vector2f(mouse_pos))) {
		if (mouse_pos.x <= position.x + main_box.getSize().x/2.0f) {
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
