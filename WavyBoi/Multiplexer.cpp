#include "pch.h"
#include "Multiplexer.h"

extern GUISettings gui;

Multiplexer::Multiplexer()
{
	selections = 2;
	initializeElements();
}

void Multiplexer::initializeElements()
{
	GUIElement end_rect(GUIElement::RECTANGLE, 24, 24);
	end_rect.setTexture(&gui.add_24x24_tex);
	elements.push_back(end_rect);
	GUIElement input_circle(GUIElement::CIRCLE, gui.obj_circle_radius);
	input_circle.setPosition(sf::Vector2f(12, -gui.outline_thickness / 2.0f) +
		sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
	inputs.push_back(input_circle);
	GUIElement output_circle(GUIElement::CIRCLE, gui.obj_circle_radius);
	outputs.push_back(output_circle);
	updateGUIElements();
}

void Multiplexer::updateGUIElements()
{
	while (elements.size() <= selections) {
		GUIElement rect(GUIElement::RECTANGLE, 24, 24);
		elements.push_back(rect);
	}
	while (elements.size() > selections + 1) {
		elements.pop_back();
	}
	while (inputs.size() <= selections) {
		GUIElement input_circle(GUIElement::CIRCLE, gui.obj_circle_radius);
		inputs.push_back(input_circle);
	}
	while (inputs.size() > selections + 1) {
		inputs.pop_back();
	}
	for (int i = 1; i <= selections; i++) {
		inputs.at(i).setPosition(sf::Vector2f(0, 12.0f + 24.0f * (float)(i - 1)) +
			sf::Vector2f(-gui.outline_thickness/2.0, 0.0) +
			sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius)
		);
		elements.at(i).setTexture(i == 1 ? &gui.blank_24x24_tex : &gui.delete_24x24_tex);
		elements.at(i).setPosition(sf::Vector2f(0.0f, 24.0f * (float)(i - 1)));
	}
	elements.at(0).setPosition(sf::Vector2f(0, 24 * selections));
	outputs.at(0).setPosition(sf::Vector2f(24, 12 + 24 * (curr_selection - 1)) +
		sf::Vector2f(gui.outline_thickness / 2.0, 0.0) +
		sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius)
	);
	Object::updateGUIElements();
}

Multiplexer::~Multiplexer()
{

}

Parameter Multiplexer::getParameter()
{
	return Parameter(out_type,curr_value,name);
}

void Multiplexer::setParameter(Parameter * parameter, int ind)
{
	if (ind == 0) {
		if (parameter->getType() == PARAM_TYPE::INT) {
			curr_selection = parameter->getValue().int_val + 1;
			if (curr_selection > selections) curr_selection = selections;
			if (curr_selection < 1) curr_selection = 1;
		}
	}
	if (ind == 1) {
		out_type = parameter->getType();
	}
	if (ind == curr_selection) {
		if (parameter->getType() == out_type) {
			curr_value = parameter->getValue();
		}
		else {
			
		}
	}
}

bool Multiplexer::getMultipleInputsAllowed(int ind)
{
	return (false);
}

ClickResponse Multiplexer::processLeftClick(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	for (int i = 0; i <= selections; i++) {
		if (checkIntersection(elements.at(i).getGlobalBounds(), mouse_pos)) {
			if (i == 0) {
				selections += 1;
				response.type = CLICK_RESPONSE::PROCESSED;
				response.clicked = true;
				return response;
			}
			else if (i == 1) {
				response.type = CLICK_RESPONSE::SELECTED;
				response.clicked = true;
				return response;
			}
			else {
				selections -= 1;
				if (i == curr_selection)curr_selection--;
				response.type = CLICK_RESPONSE::DELETED_MUX_INPUT;
				response.ind = i;
				response.clicked = true;
				return response;
			}
		}
	}
	return Object::processLeftClick(mouse_pos);
}
