#include "pch.h"
#include "MathFunction.h"

MathFunction::MathFunction(int new_func) {
	func = new_func;
	in_val.float_val = 0;
}

MathFunction::MathFunction()
{
	in_val.float_val = 0;
}


MathFunction::~MathFunction()
{
}

sf::Vector2f MathFunction::getLeftPos(int ind) {
	return position + sf::Vector2f(gui.outline_thickness / 2.0f, main_box.getSize().y / 2.0f);
}

sf::Vector2f MathFunction::getRightPos() {
	return position + sf::Vector2f(main_box.getSize().x - gui.outline_thickness / 2.0f, main_box.getSize().y / 2.0f);
}

void MathFunction::setParameter(Parameter * parameter, int ind)
{
	if (parameter->getType() == PARAM_TYPE::INT) {
		in_val.float_val = (float)parameter->getValue().int_val;
	}
	else if (parameter->getType() == PARAM_TYPE::FLOAT) {
		in_val.float_val = parameter->getValue().float_val;
	}
}

Parameter MathFunction::getParameter()
{
	return Parameter(PARAM_TYPE::FLOAT,out_val,name);
}

void MathFunction::update()
{
	main_box.setSize(sf::Vector2f(48 + gui.outline_thickness * 2, gui.input_text_height + gui.outline_thickness * 4));
	main_box.setOutlineColor(gui.obj_outline_color);
	main_box.setOutlineThickness(-gui.outline_thickness);
	main_box.setFillColor(gui.obj_fill_color);
	main_box.setPosition(position);
	text.setString(func_strings[func]);
	text.setCharacterSize(gui.input_text_height);
	text.setFont(gui.font);
	text.setFillColor(sf::Color::White);
	text.setPosition(position + sf::Vector2f(gui.outline_thickness + gui.obj_circle_radius, gui.outline_thickness));
	switch (func) {
	case 0: //COS
		out_val.float_val = cos(in_val.float_val);
		break;
	case 1: //SIN
		out_val.float_val = sin(in_val.float_val);
		break;
	case 2: //TAN
		out_val.float_val = tan(in_val.float_val);
		break;
	case 3: //LOG
		out_val.float_val = log(in_val.float_val);
		break;
	}
}

void MathFunction::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	target.draw(main_box, states);
	target.draw(text, states);
	sf::CircleShape circle(gui.obj_circle_radius + gui.outline_thickness);
	circle.setOutlineColor(gui.obj_outline_color);
	circle.setOutlineThickness(-gui.outline_thickness);
	circle.setFillColor(gui.obj_fill_color);
	circle.setPosition(getRightPos() - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	target.draw(circle, states);
	circle.setPosition(getLeftPos(0) - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	target.draw(circle, states);
}

ClickResponse MathFunction::processLeftClick(sf::Vector2i mouse_pos)
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

ClickResponse MathFunction::processLeftClickRelease(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - getLeftPos(0)) <= gui.outline_thickness + gui.obj_circle_radius) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	return response;
}

ClickResponse MathFunction::processMouseWheel(sf::Vector2i mouse_pos, int delta)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	sf::RectangleShape switch_box(sf::Vector2f(12, 12));
	sf::RectangleShape move_box(sf::Vector2f(32 + gui.outline_thickness * 2, 32 + gui.outline_thickness * 2));
	move_box.setPosition(position);
	switch_box.setPosition(position + move_box.getSize() / 2.0f - switch_box.getSize() / 2.0f);
	if (checkIntersection(main_box, sf::Vector2f(mouse_pos))) {
		func += delta;
		while (func >= 4) func = func - 4;
		while (func < 0) func = 3 + func;
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
		return response;
	}
	return response;
}
