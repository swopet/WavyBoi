#include "pch.h"
#include "Comparator.h"


void Comparator::update()
{
	if (left_type != right_type) out_val.int_val = 0;
	else {
		switch (left_type) {
		case PARAM_TYPE::FLOAT:
			switch (func) {
			case COMPARATOR::LT:
				out_val.int_val = (left_val.float_val < right_val.float_val) ? 1 : 0;
				break;
			case COMPARATOR::LTE:
				out_val.int_val = (left_val.float_val <= right_val.float_val) ? 1 : 0;
				break;
			case COMPARATOR::EQ:
				out_val.int_val = (left_val.float_val == right_val.float_val) ? 1 : 0;
				break;
			case COMPARATOR::NEQ:
				out_val.int_val = (left_val.float_val != right_val.float_val) ? 1 : 0;
				break;
			case COMPARATOR::GT:
				out_val.int_val = (left_val.float_val > right_val.float_val) ? 1 : 0;
				break;
			case COMPARATOR::GTE:
				out_val.int_val = (left_val.float_val >= right_val.float_val) ? 1 : 0;
				break;
			}
			break;
		case PARAM_TYPE::INT:
			switch (func) {
			case COMPARATOR::LT:
				out_val.int_val = (left_val.int_val < right_val.int_val) ? 1 : 0;
				break;
			case COMPARATOR::LTE:
				out_val.int_val = (left_val.int_val <= right_val.int_val) ? 1 : 0;
				break;
			case COMPARATOR::EQ:
				out_val.int_val = (left_val.int_val == right_val.int_val) ? 1 : 0;
				break;
			case COMPARATOR::NEQ:
				out_val.int_val = (left_val.int_val != right_val.int_val) ? 1 : 0;
				break;
			case COMPARATOR::GT:
				out_val.int_val = (left_val.int_val > right_val.int_val) ? 1 : 0;
				break;
			case COMPARATOR::GTE:
				out_val.int_val = (left_val.int_val >= right_val.int_val) ? 1 : 0;
				break;
			}
			break;
		default:
			out_val.int_val = 0;
			break;
		}
	}
}

param Comparator::getVal() {
	return out_val;
}

void Comparator::setParameter(Parameter * parameter, int ind)
{
	if (ind == 0) {
		left_type = parameter->getType();
		left_val = parameter->getValue();
	}
	if (ind == 1) {
		right_type = parameter->getType();
		right_val = parameter->getValue();
	}
}

void Comparator::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	sf::RectangleShape main_box(sf::Vector2f(20 + COMPARATOR_OUTLINE_THICKNESS * 2, 40 + COMPARATOR_OUTLINE_THICKNESS * 2));
	main_box.setPosition(position);
	main_box.setOutlineColor(sf::Color(192, 192, 192));
	main_box.setOutlineThickness(-COMPARATOR_OUTLINE_THICKNESS);
	main_box.setFillColor(sf::Color(128, 128, 128));
	target.draw(main_box);
	sf::CircleShape circle(COMPARATOR_OUTLINE_THICKNESS + COMPARATOR_CIRCLE_RADIUS);
	circle.setOutlineColor(sf::Color(192, 192, 192));
	circle.setOutlineThickness(-COMPARATOR_OUTLINE_THICKNESS);
	circle.setFillColor(sf::Color(128, 128, 128));
	circle.setPosition(getLeftPos(0) - sf::Vector2f(circle.getRadius(),circle.getRadius()));
	target.draw(circle);
	circle.setPosition(getLeftPos(1) - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	target.draw(circle);
	circle.setPosition(getRightPos() - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	target.draw(circle);
	std::string comp_string = "";
	switch (func) {
	case COMPARATOR::LT:
		comp_string = "<";
		break;
	case COMPARATOR::LTE:
		comp_string = "<=";
		break;
	case COMPARATOR::EQ:
		comp_string = "==";
		break;
	case COMPARATOR::NEQ:
		comp_string = "!=";
		break;
	case COMPARATOR::GT:
		comp_string = ">";
		break;
	case COMPARATOR::GTE:
		comp_string = ">=";
		break;
	}
	sf::Text text(comp_string, font, 20);
	text.setFillColor(sf::Color(255, 255, 255));
	text.setPosition(position + sf::Vector2f(COMPARATOR_OUTLINE_THICKNESS*2, COMPARATOR_OUTLINE_THICKNESS + 10));
	target.draw(text);
}

sf::Vector2f Comparator::getLeftPos(int ind)
{
	if (ind == 0) {
		return sf::Vector2f(position + sf::Vector2f(COMPARATOR_OUTLINE_THICKNESS / 2, COMPARATOR_OUTLINE_THICKNESS /2 + 10));
	}
	if (ind == 1) {
		return sf::Vector2f(position + sf::Vector2f(COMPARATOR_OUTLINE_THICKNESS / 2, COMPARATOR_OUTLINE_THICKNESS / 2 + 30));
	}
}

sf::Vector2f Comparator::getRightPos()
{
	return position + sf::Vector2f(3*COMPARATOR_OUTLINE_THICKNESS/2 + 20, COMPARATOR_OUTLINE_THICKNESS + 20);
}

Comparator::Comparator()
{
	func = COMPARATOR::GT;
	font.loadFromFile("C:/Users/Trevor/Stuff/VS/WavyBoi/resources/fonts/Montserrat-Medium.otf");
	left_type = PARAM_TYPE::FLOAT;
	right_type = PARAM_TYPE::FLOAT;
	left_val.float_val = 1.0;
	right_val.float_val = 0.2;
}


Comparator::~Comparator()
{
}

ClickResponse Comparator::processLeftClick(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - getRightPos()) <= COMPARATOR_CIRCLE_RADIUS + COMPARATOR_OUTLINE_THICKNESS) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
	}
	return response;
}

ClickResponse Comparator::processLeftClickRelease(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	for (int i = 0; i <= 2; i++) {
		sf::Vector2f pos = getLeftPos(i);
		if (length(sf::Vector2f(mouse_pos) - pos) <= COMPARATOR_OUTLINE_THICKNESS + COMPARATOR_CIRCLE_RADIUS) {
			response.clicked = true;
			response.ind = i;
			response.type = CLICK_RESPONSE::GOT_LEFT;
			return response;
		}
	}
	return response;
}
