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
	sf::RectangleShape main_box(sf::Vector2f(32 + gui.outline_thickness * 2, 32 + gui.outline_thickness * 2));
	main_box.setPosition(position);
	main_box.setOutlineColor(gui.obj_outline_color);
	main_box.setOutlineThickness(-gui.outline_thickness);
	main_box.setFillColor(gui.obj_fill_color);
	sf::RectangleShape tex_box(sf::Vector2f(32, 32));
	tex_box.setPosition(position + sf::Vector2f(gui.outline_thickness, gui.outline_thickness));
	target.draw(main_box);
	switch (func) {
	case COMPARATOR::LT:
		tex_box.setTexture((out_val.int_val) ? &gui.lt_true_32x32_tex : &gui.lt_false_32x32_tex);
		break;
	case COMPARATOR::LTE:
		tex_box.setTexture((out_val.int_val) ? &gui.lte_true_32x32_tex : &gui.lte_false_32x32_tex);
		break;
	case COMPARATOR::GT:
		tex_box.setTexture((out_val.int_val) ? &gui.gt_true_32x32_tex : &gui.gt_false_32x32_tex);
		break;
	case COMPARATOR::GTE:
		tex_box.setTexture((out_val.int_val) ? &gui.gte_true_32x32_tex : &gui.gte_false_32x32_tex);
		break;
	case COMPARATOR::EQ:
		tex_box.setTexture((out_val.int_val) ? &gui.eq_true_32x32_tex : &gui.eq_false_32x32_tex);
		break;
	case COMPARATOR::NEQ:
		tex_box.setTexture((out_val.int_val) ? &gui.neq_true_32x32_tex : &gui.neq_false_32x32_tex);
		break;
	}
	target.draw(tex_box);
	sf::CircleShape circle(gui.outline_thickness + gui.obj_circle_radius);
	circle.setOutlineColor(gui.obj_outline_color);
	circle.setOutlineThickness(-gui.outline_thickness);
	circle.setFillColor(gui.obj_fill_color);
	circle.setPosition(getLeftPos(0) - sf::Vector2f(circle.getRadius(),circle.getRadius()));
	target.draw(circle);
	circle.setPosition(getLeftPos(1) - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	target.draw(circle);
	circle.setPosition(getRightPos() - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	target.draw(circle);
}

sf::Vector2f Comparator::getLeftPos(int ind)
{
	if (ind == 0) {
		return sf::Vector2f(position + sf::Vector2f(gui.outline_thickness / 2, gui.outline_thickness + 8));
	}
	if (ind == 1) {
		return sf::Vector2f(position + sf::Vector2f(gui.outline_thickness / 2, gui.outline_thickness + 24));
	}
}

sf::Vector2f Comparator::getRightPos()
{
	return position + sf::Vector2f(3*gui.outline_thickness/2 + 32, gui.outline_thickness + 16);
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
	if (length(sf::Vector2f(mouse_pos) - getRightPos()) <= gui.obj_circle_radius + gui.outline_thickness) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
		return response;
	}
	sf::RectangleShape switch_box(sf::Vector2f(12,12));
	sf::RectangleShape move_box(sf::Vector2f(32 + gui.outline_thickness * 2, 32 + gui.outline_thickness * 2));
	move_box.setPosition(position);
	switch_box.setPosition(position + move_box.getSize()/2.0f - switch_box.getSize()/2.0f);
	if (checkIntersection(switch_box, sf::Vector2f(mouse_pos))) {
		func = (COMPARATOR)((int)func + 1);
		if (func == COMPARATOR::RESET) func = (COMPARATOR)0;
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
		return response;
	}
	if (checkIntersection(move_box, sf::Vector2f(mouse_pos))) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
		return response;
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
		if (length(sf::Vector2f(mouse_pos) - pos) <= gui.outline_thickness + gui.obj_circle_radius) {
			response.clicked = true;
			response.ind = i;
			response.type = CLICK_RESPONSE::GOT_LEFT;
			return response;
		}
	}
	return response;
}
