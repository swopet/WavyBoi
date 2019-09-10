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
	Object::draw(target, states);
	target.draw(text, states);
}

void FloatObject::initializeElements()
{
	GUIElement main_box_element = GUIElement(GUIElement::RECTANGLE, 32 - gui.outline_thickness * 2, 32 - gui.outline_thickness * 2);
	main_box_element.setOrigin(position);
	main_box_element.setPosition(sf::Vector2f(0, 0));
	elements.push_back(main_box_element);
	GUIElement input_circle = GUIElement(GUIElement::CIRCLE, gui.obj_circle_radius);
	inputs.push_back(input_circle);
	GUIElement output_circle = GUIElement(GUIElement::CIRCLE, gui.obj_circle_radius);
	outputs.push_back(output_circle);
	updateGUIElements();
}

void FloatObject::updateGUIElements()
{
	std::ostringstream out;
	out.precision(precision);
	out << std::fixed << out_val.float_val;
	text = sf::Text(out.str(), gui.font, gui.input_text_height);
	text.setFillColor(sf::Color::White);
	sf::FloatRect text_rect = text.getLocalBounds();
	elements.at(0).setSize(text_rect.width + text_rect.left + gui.text_buffer*2.0, text_rect.height + text_rect.top + gui.text_buffer*2.0);
	inputs.at(0).setPosition(
		sf::Vector2f(0, elements.at(0).getGlobalBounds().height / 2.0) +
		sf::Vector2f(-gui.outline_thickness / 2.0, 0.0) +
		sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
	outputs.at(0).setPosition(
		sf::Vector2f(elements.at(0).getGlobalBounds().width, elements.at(0).getGlobalBounds().height / 2.0) +
		sf::Vector2f(gui.outline_thickness / 2.0, 0.0) +
		sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
	Object::updateGUIElements();
	text.setPosition(position + sf::Vector2f(gui.text_buffer, gui.text_buffer) -
		sf::Vector2f(text_rect.left / 2.0, text_rect.top / 2.0));
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

ClickResponse FloatObject::processDoubleLeftClick(sf::Vector2i mouse_pos)
{
	if (checkIntersection(elements.at(0).getGlobalBounds(), sf::Vector2f(mouse_pos))) {
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
	if (checkIntersection(elements.at(0).getGlobalBounds(), sf::Vector2f(mouse_pos))) {
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
	initializeElements();
}

FloatObject::FloatObject(float new_val)
{
	out_val.float_val = new_val;
	default_val.float_val = new_val;
	initializeElements();
}


FloatObject::~FloatObject()
{
}
