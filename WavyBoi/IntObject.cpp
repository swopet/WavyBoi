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
	Object::draw(target, states);
	target.draw(text, states);
}

void IntObject::initializeElements() {
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

void IntObject::updateGUIElements()
{
	text = sf::Text(std::to_string(out_val.int_val), gui.font, gui.input_text_height);
	text.setFillColor(sf::Color::White);
	sf::FloatRect text_rect = text.getLocalBounds();
	elements.at(0).setSize(text_rect.width + text_rect.left + gui.obj_text_buffer*2.0, text_rect.height + text_rect.top + gui.obj_text_buffer*2.0);
	inputs.at(0).setPosition(
		sf::Vector2f(0, elements.at(0).getGlobalBounds().height/2.0) +
		sf::Vector2f(-gui.outline_thickness / 2.0, 0.0) +
		sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
	outputs.at(0).setPosition(
		sf::Vector2f(elements.at(0).getGlobalBounds().width, elements.at(0).getGlobalBounds().height/2.0) +
		sf::Vector2f(gui.outline_thickness / 2.0, 0.0) +
		sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
	Object::updateGUIElements();
	text.setPosition(position +sf::Vector2f(gui.obj_text_buffer , gui.obj_text_buffer) -
		sf::Vector2f(text_rect.left/2.0, text_rect.top/2.0));
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
	return checkIntersection(select_box.getGlobalBounds(), elements.at(0).getGlobalBounds());
}

ClickResponse IntObject::processDoubleLeftClick(sf::Vector2i mouse_pos)
{
	if (checkIntersection(elements.at(0).getGlobalBounds(),mouse_pos)){
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
	initializeElements();
	default_val.int_val = 0;
	out_val.int_val = 0;
}

IntObject::IntObject(int new_val)
{
	initializeElements();
	out_val.int_val = new_val;
	default_val.int_val = new_val;
}


IntObject::~IntObject()
{
}
