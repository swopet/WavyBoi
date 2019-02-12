#include "pch.h"
#include "FreqBandBlock.h"


FreqBandBlock::FreqBandBlock()
{
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
	switch (freq_band) {
	case 0:
		out_val.float_val = audio_handler->getSubBass();
		break;
	case 1:
		out_val.float_val = audio_handler->getBass();
		break;
	case 2:
		out_val.float_val = audio_handler->getLowerMid();
		break;
	case 3:
		out_val.float_val = audio_handler->getMid();
		break;
	case 4:
		out_val.float_val = audio_handler->getUpperMid();
		break;
	case 5:
		out_val.float_val = audio_handler->getPresence();
		break;
	case 6:
		out_val.float_val = audio_handler->getBrilliance();
		break;
	default:
		out_val.float_val = 0.0;
		break;
	}
	text.setString(freq_band_strings[freq_band]);
	text.setCharacterSize(gui.input_text_height);
	text.setFont(gui.font);
	text.setFillColor(sf::Color::White);
	text.setPosition(position + sf::Vector2f(gui.outline_thickness, gui.outline_thickness));
}

sf::Vector2f FreqBandBlock::getRightPos()
{
	return position + sf::Vector2f(main_box.getSize().x-gui.outline_thickness / 2, main_box.getSize().y/2);
}

void FreqBandBlock::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	target.draw(main_box);
	target.draw(text);
	sf::CircleShape right_circle(gui.obj_circle_radius + gui.outline_thickness);
	right_circle.setOutlineColor(gui.obj_outline_color);
	right_circle.setOutlineThickness(-gui.outline_thickness);
	right_circle.setFillColor(gui.obj_fill_color);
	right_circle.setPosition(getRightPos() - sf::Vector2f(right_circle.getRadius(),right_circle.getRadius()));
	target.draw(right_circle);
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
	sf::RectangleShape switch_box(sf::Vector2f(12, 12));
	sf::RectangleShape move_box(sf::Vector2f(32 + gui.outline_thickness * 2, 32 + gui.outline_thickness * 2));
	move_box.setPosition(position);
	switch_box.setPosition(position + move_box.getSize() / 2.0f - switch_box.getSize() / 2.0f);
	if (checkIntersection(main_box, sf::Vector2f(mouse_pos))) {
		freq_band += delta;
		while (freq_band >= 7) freq_band = freq_band - 7;
		while (freq_band < 0) freq_band = 6 + freq_band;
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
		return response;
	}
	return response;
}
