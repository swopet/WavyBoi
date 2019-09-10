#include "pch.h"
#include "MathFunction.h"

MathFunction::MathFunction(int new_func) {
	func = new_func;
	in_val.float_val = 0;
	initializeElements();
}

MathFunction::MathFunction()
{
	in_val.float_val = 0;
	initializeElements();
}

void MathFunction::initializeElements() {
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

void MathFunction::updateGUIElements()
{
	text = sf::Text(func_strings[func], gui.font, gui.input_text_height);
	text.setFillColor(sf::Color::White);
	sf::FloatRect text_rect = text.getLocalBounds();
	elements.at(0).setSize(text_rect.width + text_rect.left + gui.obj_text_buffer*2.0, text_rect.height + text_rect.top + gui.obj_text_buffer*2.0);
	inputs.at(0).setPosition(
		sf::Vector2f(0, elements.at(0).getGlobalBounds().height / 2.0) +
		sf::Vector2f(-gui.outline_thickness / 2.0, 0.0) +
		sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
	outputs.at(0).setPosition(
		sf::Vector2f(elements.at(0).getGlobalBounds().width, elements.at(0).getGlobalBounds().height / 2.0) +
		sf::Vector2f(gui.outline_thickness / 2.0, 0.0) +
		sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
	Object::updateGUIElements();
	text.setPosition(position + sf::Vector2f(gui.obj_text_buffer, gui.obj_text_buffer) -
		sf::Vector2f(text_rect.left / 2.0, text_rect.top / 2.0));
}

MathFunction::~MathFunction()
{
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
	Object::draw(target, states);
	target.draw(text, states);
}

bool MathFunction::checkOverlap(sf::RectangleShape select_box) {
	return checkIntersection(select_box.getGlobalBounds(), elements.at(0).getGlobalBounds());
}

ClickResponse MathFunction::processMouseWheel(sf::Vector2i mouse_pos, int delta)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (checkIntersection(elements.at(0).getGlobalBounds(), sf::Vector2f(mouse_pos))) {
		func += delta;
		while (func >= 4) func = func - 4;
		while (func < 0) func = 3 + func;
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
		return response;
	}
	return response;
}
