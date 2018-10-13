#include "pch.h"
#include "Multiplexer.h"

extern sf::Texture add_20x20_tex;
extern sf::Texture delete_20x20_tex;

Multiplexer::Multiplexer()
{
	selections = 2;
}


Multiplexer::~Multiplexer()
{

}

param Multiplexer::getVal()
{
	return curr_value;
}

void Multiplexer::setParameter(Parameter * parameter, int ind)
{
	if (ind == 1) {
		out_type = parameter->getType();
	}
	if (ind == curr_selection){
		curr_value = parameter->getValue();
	}
}

bool Multiplexer::getMultipleInputsAllowed(int ind)
{
	return (ind == 0);
}

sf::Vector2f Multiplexer::getLeftPos(int ind)
{
	if (ind == 0) {
		return position + sf::Vector2f(SELECTOR_OUTLINE_THICKNESS + SELECTOR_BOX_SIZE / 2, SELECTOR_OUTLINE_THICKNESS / 2);
	}
	else {
		if (ind <= selections) {
			return position + sf::Vector2f(SELECTOR_OUTLINE_THICKNESS / 2, SELECTOR_BOX_SIZE / 2 + SELECTOR_OUTLINE_THICKNESS + (SELECTOR_BOX_SIZE + SELECTOR_OUTLINE_THICKNESS * 2)*(ind - 1));
		}
	}
	return sf::Vector2f();
}

sf::Vector2f Multiplexer::getRightPos()
{
	return position + sf::Vector2f(SELECTOR_OUTLINE_THICKNESS * 3 / 2 + SELECTOR_BOX_SIZE, SELECTOR_BOX_SIZE / 2 + SELECTOR_OUTLINE_THICKNESS + (SELECTOR_BOX_SIZE + SELECTOR_OUTLINE_THICKNESS * 2)*(curr_selection - 1));
}

void Multiplexer::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	for (int i = selections; i >= 0; i--) { //draw in reverse so the top circle gets drawn on top lol fight me future trevor
		if (i > 0) {
			sf::RectangleShape box(sf::Vector2f(SELECTOR_BOX_SIZE + SELECTOR_OUTLINE_THICKNESS * 2, SELECTOR_BOX_SIZE + SELECTOR_OUTLINE_THICKNESS * 2));
			box.setOutlineThickness(-SELECTOR_OUTLINE_THICKNESS);
			(i == 1) ? box.setFillColor(sf::Color(128, 128, 128)) : box.setTexture(&delete_20x20_tex);
			box.setOutlineColor(sf::Color(192, 192, 192));
			box.setPosition(position + sf::Vector2f(0, (SELECTOR_BOX_SIZE + SELECTOR_OUTLINE_THICKNESS * 2)*(i-1)));
			target.draw(box);
		}
		sf::CircleShape circle(SELECTOR_OUTLINE_THICKNESS + SELECTOR_CIRCLE_RADIUS);
		circle.setOutlineThickness(-SELECTOR_OUTLINE_THICKNESS);
		circle.setFillColor(sf::Color(128, 128, 128));
		circle.setOutlineColor(sf::Color(192, 192, 192));
		circle.setPosition(getLeftPos(i) - sf::Vector2f(circle.getRadius(), circle.getRadius()));
		target.draw(circle);
		if (i == curr_selection) {
			circle.setPosition(getRightPos() - sf::Vector2f(circle.getRadius(), circle.getRadius()));
			target.draw(circle);
		}
	}
	sf::RectangleShape box(sf::Vector2f(SELECTOR_BOX_SIZE + SELECTOR_OUTLINE_THICKNESS * 2, SELECTOR_BOX_SIZE + SELECTOR_OUTLINE_THICKNESS * 2));
	box.setOutlineThickness(-SELECTOR_OUTLINE_THICKNESS);
	box.setTexture(&add_20x20_tex);
	box.setOutlineColor(sf::Color(192, 192, 192));
	box.setPosition(position + sf::Vector2f(0, (SELECTOR_BOX_SIZE + SELECTOR_OUTLINE_THICKNESS * 2)*selections));
	target.draw(box);

}

ClickResponse Multiplexer::processLeftClick(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	for (int i = 0; i <= selections; i++) {
		sf::Vector2f pos = getLeftPos(i);
		if (length(sf::Vector2f(mouse_pos) - pos) <= SELECTOR_OUTLINE_THICKNESS + SELECTOR_CIRCLE_RADIUS) {
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
		if (length(sf::Vector2f(mouse_pos) - pos) <= SELECTOR_OUTLINE_THICKNESS + SELECTOR_CIRCLE_RADIUS) {
			response.clicked = true;
			response.ind = curr_selection;
			response.type = CLICK_RESPONSE::GOT_RIGHT;
			return response;
		}
	}
	sf::RectangleShape box(sf::Vector2f(SELECTOR_BOX_SIZE + SELECTOR_OUTLINE_THICKNESS * 2, SELECTOR_BOX_SIZE + SELECTOR_OUTLINE_THICKNESS * 2));
	sf::RectangleShape mouse_box(sf::Vector2f(0, 0));
	mouse_box.setPosition(sf::Vector2f(mouse_pos));
	for (int i = 0; i <= selections; i++) {
		box.setPosition(position + sf::Vector2f(0, SELECTOR_BOX_SIZE + SELECTOR_OUTLINE_THICKNESS * 2) * (float)i);
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
		if (length(sf::Vector2f(mouse_pos) - pos) <= SELECTOR_OUTLINE_THICKNESS + SELECTOR_CIRCLE_RADIUS) {
			response.clicked = true;
			response.ind = i;
			response.type = CLICK_RESPONSE::GOT_LEFT;
			return response;
		}
	}
	return response;
}
