#include "pch.h"
#include "FreqBandBlock.h"

const std::string band_strings[] = { "20-60Hz","60-250Hz","250-500Hz","500-2kHz","2-4kHz","4-6kHz",">6kHz" };

FreqBandBlock::FreqBandBlock()
{
}


FreqBandBlock::~FreqBandBlock()
{
}

param FreqBandBlock::getVal()
{
	param return_val;
	switch (freq_band) {
	case 0:
		return_val.float_val = audio_handler->getSubBass();
		break;
	case 1:
		return_val.float_val = audio_handler->getBass();
		break;
	case 2:
		return_val.float_val = audio_handler->getLowerMid();
		break;
	case 3:
		return_val.float_val = audio_handler->getMid();
		break;
	case 4:
		return_val.float_val = audio_handler->getUpperMid();
		break;
	case 5:
		return_val.float_val = audio_handler->getPresence();
		break;
	case 6:
		return_val.float_val = audio_handler->getBrilliance();
		break;
	default:
		return_val.float_val = 0.0;
		break;
	}
	return return_val;
}

sf::Vector2f FreqBandBlock::getRightPos()
{
	return position + sf::Vector2f(96 + 3 * gui.outline_thickness / 2, 12 + gui.outline_thickness);
}

void FreqBandBlock::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	sf::RectangleShape main_box(sf::Vector2f(96 + gui.outline_thickness * 2, 24 + gui.outline_thickness * 2));
	main_box.setOutlineColor(gui.obj_outline_color);
	main_box.setOutlineThickness(-gui.outline_thickness);
	main_box.setFillColor(gui.obj_fill_color);
	main_box.setPosition(position);
	target.draw(main_box);
	std::string text_string = band_strings[freq_band];
	sf::Text text(band_strings[freq_band], gui.font, gui.input_text_height);
	text.setFillColor(sf::Color::White);
	text.setPosition(position);
	target.draw(text);
	sf::CircleShape right_circle(gui.obj_circle_radius + gui.outline_thickness);
	right_circle.setOutlineColor(gui.obj_outline_color);
	right_circle.setOutlineThickness(-gui.outline_thickness);
	right_circle.setFillColor(gui.obj_fill_color);
	right_circle.setPosition(getRightPos() - sf::Vector2f(right_circle.getRadius(),right_circle.getRadius()));
	target.draw(right_circle);
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
	sf::RectangleShape main_box(sf::Vector2f(96 + gui.outline_thickness * 2, 24 + gui.outline_thickness * 2));
	main_box.setPosition(position);
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
	sf::RectangleShape main_box(sf::Vector2f(96 + gui.outline_thickness * 2, 24 + gui.outline_thickness * 2));
	main_box.setPosition(position);
	if (checkIntersection(main_box, sf::Vector2f(mouse_pos))) {
		freq_band += delta;
		if (freq_band >= 7) freq_band = 0;
		if (freq_band < 0) freq_band = 6;
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
		return response;
	}
}
