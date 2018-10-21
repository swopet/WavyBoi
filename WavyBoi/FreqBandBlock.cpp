#include "pch.h"
#include "FreqBandBlock.h"


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
		return_val.float_val = audio_handler->getBass();
		break;
	case 1:
		return_val.float_val = audio_handler->getSubBass();
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
	return position + sf::Vector2f(40 + 3 * gui.outline_thickness / 2, 10 + gui.outline_thickness);
}

void FreqBandBlock::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	sf::RectangleShape main_box(sf::Vector2f(40 + gui.outline_thickness * 2, 20 + gui.outline_thickness * 2));
	main_box.setOutlineColor(gui.obj_outline_color);
	main_box.setOutlineThickness(-gui.outline_thickness);
	main_box.setFillColor(gui.obj_fill_color);
	main_box.setPosition(position);
	target.draw(main_box);
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
	sf::RectangleShape main_box(sf::Vector2f(40 + gui.outline_thickness * 2, 20 + gui.outline_thickness * 2));
	main_box.setPosition(position);
	if (checkIntersection(main_box, sf::Vector2f(mouse_pos))) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
		return response;
	}
	return response;
}
