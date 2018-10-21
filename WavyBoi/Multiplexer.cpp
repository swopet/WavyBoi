#include "pch.h"
#include "Multiplexer.h"

extern GUISettings gui;

Multiplexer::Multiplexer()
{
	selections = 2;
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

sf::Vector2f Multiplexer::getLeftPos(int ind)
{
	if (ind == 0) {
		return position + sf::Vector2f(gui.outline_thickness + SELECTOR_BOX_SIZE / 2, gui.outline_thickness / 2);
	}
	else {
		if (ind <= selections) {
			return position + sf::Vector2f(gui.outline_thickness / 2, SELECTOR_BOX_SIZE / 2 + gui.outline_thickness + (SELECTOR_BOX_SIZE + gui.outline_thickness * 2)*(ind - 1));
		}
	}
	return sf::Vector2f();
}

sf::Vector2f Multiplexer::getRightPos()
{
	return position + sf::Vector2f(gui.outline_thickness * 3 / 2 + SELECTOR_BOX_SIZE, SELECTOR_BOX_SIZE / 2 + gui.outline_thickness + (SELECTOR_BOX_SIZE + gui.outline_thickness * 2)*(curr_selection - 1));
}

void Multiplexer::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	for (int i = selections; i >= 0; i--) { //draw in reverse so the top circle gets drawn on top lol fight me future trevor
		if (i > 0) {
			sf::RectangleShape box(sf::Vector2f(SELECTOR_BOX_SIZE + gui.outline_thickness * 2, SELECTOR_BOX_SIZE + gui.outline_thickness * 2));
			box.setOutlineThickness(-gui.outline_thickness);
			(i == 1) ? box.setFillColor(gui.obj_fill_color) : box.setTexture(&gui.delete_20x20_tex);
			box.setOutlineColor(gui.obj_outline_color);
			box.setPosition(position + sf::Vector2f(0, (SELECTOR_BOX_SIZE + gui.outline_thickness * 2)*(i-1)));
			target.draw(box);
		}
		sf::CircleShape circle(gui.outline_thickness + gui.obj_circle_radius);
		circle.setOutlineThickness(-gui.outline_thickness);
		circle.setFillColor(gui.obj_fill_color);
		circle.setOutlineColor(gui.obj_outline_color);
		circle.setPosition(getLeftPos(i) - sf::Vector2f(circle.getRadius(), circle.getRadius()));
		target.draw(circle);
		if (i == curr_selection) {
			circle.setPosition(getRightPos() - sf::Vector2f(circle.getRadius(), circle.getRadius()));
			target.draw(circle);
		}
	}
	sf::RectangleShape box(sf::Vector2f(SELECTOR_BOX_SIZE + gui.outline_thickness * 2, SELECTOR_BOX_SIZE + gui.outline_thickness * 2));
	box.setOutlineThickness(-gui.outline_thickness);
	box.setTexture(&gui.add_20x20_tex);
	box.setOutlineColor(gui.obj_outline_color);
	box.setPosition(position + sf::Vector2f(0, (SELECTOR_BOX_SIZE + gui.outline_thickness * 2)*selections));
	target.draw(box);

}

ClickResponse Multiplexer::processLeftClick(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	for (int i = 0; i <= selections; i++) {
		sf::Vector2f pos = getLeftPos(i);
		if (length(sf::Vector2f(mouse_pos) - pos) <= gui.outline_thickness + gui.obj_circle_radius) {
			response.clicked = true;
			response.ind = i;
			if (i == 0) {
				response.type = CLICK_RESPONSE::SELECTED;
			}
			else {
				curr_selection = i;
				response.type = CLICK_RESPONSE::PROCESSED;
			}
			return response;
		}
	}
	if (curr_selection != 0 && curr_selection <= selections) {
		sf::Vector2f pos = getRightPos();
		if (length(sf::Vector2f(mouse_pos) - pos) <= gui.outline_thickness + gui.obj_circle_radius) {
			response.clicked = true;
			response.ind = curr_selection;
			response.type = CLICK_RESPONSE::GOT_RIGHT;
			return response;
		}
	}
	sf::RectangleShape box(sf::Vector2f(SELECTOR_BOX_SIZE + gui.outline_thickness * 2, SELECTOR_BOX_SIZE + gui.outline_thickness * 2));
	sf::RectangleShape mouse_box(sf::Vector2f(0, 0));
	mouse_box.setPosition(sf::Vector2f(mouse_pos));
	for (int i = 0; i <= selections; i++) {
		box.setPosition(position + sf::Vector2f(0, SELECTOR_BOX_SIZE + gui.outline_thickness * 2) * (float)i);
		if (checkIntersection(box, mouse_box)) {
			response.clicked = true;
			response.ind = i + 1;
			if (i==0) response.type = CLICK_RESPONSE::SELECTED;
			else if (i == selections) {
				selections++;
				response.type = CLICK_RESPONSE::PROCESSED;
			}
			else {
				selections--;
				if (curr_selection > selections) curr_selection--;
				response.type = CLICK_RESPONSE::DELETED_MUX_INPUT;
			}
			return response;
		}
	}
	return response;
}

ClickResponse Multiplexer::processLeftClickHeld(sf::Vector2i)
{
	return ClickResponse();
}

ClickResponse Multiplexer::processLeftClickRelease(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	for (int i = 0; i <= selections; i++) {
		sf::Vector2f pos = getLeftPos(i);
		if (length(sf::Vector2f(mouse_pos) - pos) <= gui.outline_thickness + gui.obj_circle_radius) {
			response.clicked = true;
			response.ind = i;
			response.type = CLICK_RESPONSE::GOT_LEFT;
			return response;
		}
	}
	return response;
}
