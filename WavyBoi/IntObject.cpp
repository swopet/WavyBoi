#include "pch.h"
#include "IntObject.h"


void IntObject::processNewString(std::string field, std::string input)
{
	if (field == "value") {
		try {
			int new_val = std::stoi(input);
			out_val.int_val = new_val;
			default_val = out_val;
		}
		catch (const std::invalid_argument& ia) {
			std::cerr << "Invalid argument: " << ia.what() << '\n';
		}
	}
}

void IntObject::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	target.draw(main_box, states);
	target.draw(text, states);
	sf::CircleShape circle(gui.outline_thickness + gui.obj_circle_radius);
	circle.setFillColor(gui.obj_fill_color);
	circle.setOutlineColor(gui.obj_outline_color);
	circle.setOutlineThickness(-gui.outline_thickness);
	circle.setPosition(left_pos - sf::Vector2f(circle.getRadius(),circle.getRadius()));
	target.draw(circle, states);
	circle.setPosition(right_pos - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	target.draw(circle, states);
}

void IntObject::update()
{
	text = sf::Text(std::to_string(out_val.int_val), gui.font, gui.input_text_height);
	float box_width = text.findCharacterPos(text.getString().getSize()).x;
	main_box = sf::RectangleShape(sf::Vector2f(box_width, gui.input_text_height) + sf::Vector2f(gui.outline_thickness * 4 + gui.obj_circle_radius*2, gui.outline_thickness * 4));
	main_box.setPosition(position);
	main_box.setFillColor(gui.obj_fill_color);
	main_box.setOutlineColor(gui.obj_outline_color);
	main_box.setOutlineThickness(-gui.outline_thickness);

	text.setPosition(position + sf::Vector2f(gui.obj_circle_radius, 0) + sf::Vector2f(gui.outline_thickness * 2, gui.outline_thickness * 2));
	text.setFillColor(sf::Color::White);
	left_pos = position + sf::Vector2f(gui.outline_thickness / 2.0f, main_box.getSize().y/2.0f);
	right_pos = position + sf::Vector2f(main_box.getSize().x - gui.outline_thickness / 2.0f, main_box.getSize().y/2.0f);
}

void IntObject::setParameter(Parameter * parameter, int ind)
{
	if (parameter->getType() == PARAM_TYPE::INT) {
		out_val.int_val = parameter->getValue().int_val;
	}
	else if (parameter->getType() == PARAM_TYPE::FLOAT) {
		out_val.int_val = (int)parameter->getValue().float_val;
	}
}

Parameter IntObject::getParameter()
{
	return Parameter(PARAM_TYPE::INT, out_val, name);
}

bool IntObject::checkOverlap(sf::RectangleShape select_box)
{
	return checkIntersection(select_box, main_box);
}

ClickResponse IntObject::processLeftClick(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - left_pos) <= gui.outline_thickness + gui.obj_circle_radius) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	else if (length(sf::Vector2f(mouse_pos) - right_pos) <= gui.outline_thickness + gui.obj_circle_radius) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
	}
	else if (checkIntersection(main_box, sf::Vector2f(mouse_pos))) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
	}
	return response;
}

ClickResponse IntObject::processLeftClickRelease(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - left_pos) <= gui.outline_thickness + gui.obj_circle_radius) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	return response;
}

ClickResponse IntObject::processDoubleLeftClick(sf::Vector2i mouse_pos)
{
	if (checkIntersection(main_box,sf::Vector2f(mouse_pos))){
		ClickResponse response;
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_TEXT_FIELD;
		response.field = "value";
		return response;
	}
	return ClickResponse();
}

IntObject::IntObject()
{
	default_val.int_val = 0;
	out_val.int_val = 0;
}

IntObject::IntObject(int new_val)
{
	out_val.int_val = new_val;
	default_val.int_val = new_val;
}


IntObject::~IntObject()
{
}
