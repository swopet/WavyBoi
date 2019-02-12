#include "pch.h"
#include "FloatObject.h"


void FloatObject::processNewString(std::string field, std::string input)
{
	if (field == "value") {
		try {
			float new_val = std::stof(input);
			out_val.float_val = new_val;
			default_val = out_val;
		}
		catch (const std::invalid_argument& ia) {
			std::cerr << "Invalid argument: " << ia.what() << '\n';
		}
	}
}

Parameter FloatObject::getParameter()
{
	return Parameter(PARAM_TYPE::FLOAT, out_val, name);
}

void FloatObject::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	target.draw(main_box);
	target.draw(text);
	sf::CircleShape circle(gui.outline_thickness + gui.obj_circle_radius);
	circle.setFillColor(gui.obj_fill_color);
	circle.setOutlineColor(gui.obj_outline_color);
	circle.setOutlineThickness(-gui.outline_thickness);
	circle.setPosition(left_pos - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	target.draw(circle);
	circle.setPosition(right_pos - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	target.draw(circle);
}

void FloatObject::update()
{
	std::ostringstream out;
	out.precision(precision);
	out << std::fixed << out_val.float_val;
	text = sf::Text(out.str(), gui.font, gui.input_text_height);
	float box_width = text.findCharacterPos(text.getString().getSize()).x;
	main_box = sf::RectangleShape(sf::Vector2f(box_width, gui.input_text_height) + sf::Vector2f(gui.outline_thickness * 4 + gui.obj_circle_radius * 2, gui.outline_thickness * 4));
	main_box.setPosition(position);
	main_box.setFillColor(gui.obj_fill_color);
	main_box.setOutlineColor(gui.obj_outline_color);
	main_box.setOutlineThickness(-gui.outline_thickness);

	text.setPosition(position + sf::Vector2f(gui.obj_circle_radius, 0) + sf::Vector2f(gui.outline_thickness * 2, gui.outline_thickness * 2));
	text.setFillColor(sf::Color::White);
	left_pos = position + sf::Vector2f(gui.outline_thickness / 2.0f, main_box.getSize().y / 2.0f);
	right_pos = position + sf::Vector2f(main_box.getSize().x - gui.outline_thickness / 2.0f, main_box.getSize().y / 2.0f);
}

void FloatObject::setParameter(Parameter * parameter, int ind)
{
	if (parameter->getType() == PARAM_TYPE::INT) {
		out_val.float_val = (float)parameter->getValue().int_val;
	}
	else if (parameter->getType() == PARAM_TYPE::FLOAT) {
		out_val.float_val = parameter->getValue().float_val;
	}
}

void FloatObject::setPrecision(int precision_)
{
	precision = precision_;
}

Parameter * FloatObject::getNewParameter()
{
	param new_param;
	new_param.float_val = 0;
	return new Parameter(PARAM_TYPE::FLOAT, new_param, name);
}

ClickResponse FloatObject::processLeftClick(sf::Vector2i mouse_pos)
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

bool FloatObject::checkOverlap(sf::RectangleShape select_box)
{
	return checkIntersection(select_box, main_box);
}

ClickResponse FloatObject::processLeftClickRelease(sf::Vector2i mouse_pos)
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

ClickResponse FloatObject::processDoubleLeftClick(sf::Vector2i mouse_pos)
{
	if (checkIntersection(main_box, sf::Vector2f(mouse_pos))) {
		ClickResponse response;
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_TEXT_FIELD;
		response.field = "value";
		return response;
	}
	return ClickResponse();
}

ClickResponse FloatObject::processMouseWheel(sf::Vector2i mouse_pos, int delta)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (checkIntersection(main_box, sf::Vector2f(mouse_pos))) {
		precision += delta;
		if (precision > 7) precision = 7;
		if (precision < 0) precision = 0;
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
		return response;
	}
	return response;
}

FloatObject::FloatObject()
{
	default_val.float_val = 0;
	out_val.float_val = 0;
}

FloatObject::FloatObject(float new_val)
{
	out_val.float_val = new_val;
	default_val.float_val = new_val;
}


FloatObject::~FloatObject()
{
}
