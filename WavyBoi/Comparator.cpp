#include "pch.h"
#include "Comparator.h"


void Operator::update()
{
	float l_val = (left_type == PARAM_TYPE::FLOAT) ? left_val.float_val : (float)left_val.int_val;
	float r_val = (right_type == PARAM_TYPE::FLOAT) ? right_val.float_val : (float)right_val.int_val;
	float o_val = 0.0;
	switch (op_type) {
	case OPERATOR::ARITHMETIC:
		switch (func.arithmetic) {
		case ARITHMETIC::PLUS:
			o_val = l_val + r_val;
			if (left_type == PARAM_TYPE::INT) out_val.int_val = (int)o_val;
			else out_val.float_val = o_val;
			break;
		case ARITHMETIC::MINUS:
			o_val = l_val - r_val;
			if (left_type == PARAM_TYPE::INT) out_val.int_val = (int)o_val;
			else out_val.float_val = o_val;
			break;
		case ARITHMETIC::TIMES:
			o_val = l_val * r_val;
			if (left_type == PARAM_TYPE::INT) out_val.int_val = (int)o_val;
			else out_val.float_val = o_val;
			break;
		case ARITHMETIC::DIVIDEDBY:
			if (r_val != 0)
				o_val = l_val / r_val;
			if (left_type == PARAM_TYPE::INT) out_val.int_val = (int)o_val;
			else out_val.float_val = o_val;
			break;
		case ARITHMETIC::MODULO:
			o_val = fmodf(l_val,r_val);
			if (left_type == PARAM_TYPE::INT) out_val.int_val = (int)o_val;
			else out_val.float_val = o_val;
			break;
		case ARITHMETIC::EXPONENT:
			o_val = powf(l_val,r_val);
			if (left_type == PARAM_TYPE::INT) out_val.int_val = (int)o_val;
			else out_val.float_val = o_val;
			break;
		}
		break;
	case OPERATOR::BOOLEAN:
		switch (func.boolean) {
		case BOOLEAN_OP::AND:
			out_val.int_val = (l_val != 0 && r_val != 0);
			break;
		case BOOLEAN_OP::OR:
			out_val.int_val = (l_val != 0 || r_val != 0);
			break;
		}
		break;
	case OPERATOR::COMPARATOR:
		switch (func.comparator) {
		case COMPARATOR::EQ:
			out_val.int_val = (l_val == r_val);
			break;
		case COMPARATOR::NEQ:
			out_val.int_val = (l_val != r_val);
			break;
		case COMPARATOR::LT:
			out_val.int_val = (l_val < r_val);
			break;
		case COMPARATOR::LTE:
			out_val.int_val = (l_val <= r_val);
			break;
		case COMPARATOR::GT:
			out_val.int_val = (l_val > r_val);
			break;
		case COMPARATOR::GTE:
			out_val.int_val = (l_val >= r_val);
			break;
		}
		break;
	}
}

Parameter Operator::getParameter() {
	return Parameter(left_type, out_val, name);
}

bool Operator::checkOverlap(sf::RectangleShape select_box)
{
	return (checkIntersection(select_box, main_box));
}

void Operator::setParameter(Parameter * parameter, int ind)
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

void Operator::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	
	main_box.setPosition(position);
	main_box.setOutlineColor(gui.obj_outline_color);
	main_box.setOutlineThickness(-gui.outline_thickness);
	main_box.setFillColor(gui.obj_fill_color);
	sf::RectangleShape tex_box(sf::Vector2f(32, 32));
	tex_box.setPosition(position + sf::Vector2f(gui.outline_thickness, gui.outline_thickness));
	target.draw(main_box, states);
	switch (op_type) {
	case OPERATOR::COMPARATOR:
		switch (func.comparator) {
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
		break; //OPERATOR::COMPARATOR
	case OPERATOR::ARITHMETIC:
		switch (func.arithmetic) {
		case ARITHMETIC::PLUS:
			tex_box.setTexture(&gui.plus_32x32_tex);
			break;
		case ARITHMETIC::MINUS:
			tex_box.setTexture(&gui.minus_32x32_tex);
			break;
		case ARITHMETIC::TIMES:
			tex_box.setTexture(&gui.times_32x32_tex);
			break;
		case ARITHMETIC::DIVIDEDBY:
			tex_box.setTexture(&gui.dividedby_32x32_tex);
			break;
		case ARITHMETIC::MODULO:
			tex_box.setTexture(&gui.modulo_32x32_tex);
			break;
		case ARITHMETIC::EXPONENT:
			tex_box.setTexture(&gui.exponent_32x32_tex);
			break;
		}
		break; //OPERATOR::ARITHMETIC
	}
	target.draw(tex_box, states);
	sf::CircleShape circle(gui.outline_thickness + gui.obj_circle_radius);
	circle.setOutlineColor(gui.obj_outline_color);
	circle.setOutlineThickness(-gui.outline_thickness);
	circle.setFillColor(gui.obj_fill_color);
	circle.setPosition(getLeftPos(0) - sf::Vector2f(circle.getRadius(),circle.getRadius()));
	target.draw(circle, states);
	circle.setPosition(getLeftPos(1) - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	target.draw(circle, states);
	circle.setPosition(getRightPos() - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	target.draw(circle, states);
}

Parameter * Operator::getNewParameter()
{
	param new_param;
	if (left_type == PARAM_TYPE::INT)
		new_param.int_val = 0;
	else if (left_type == PARAM_TYPE::FLOAT)
		new_param.float_val = 0;
	else if (left_type == PARAM_TYPE::NONE) {
		if (right_type == PARAM_TYPE::INT)
			new_param.int_val = 0;
		else if (right_type == PARAM_TYPE::FLOAT)
			new_param.float_val = 0;
		else
			return NULL;
	}
	return new Parameter(left_type, new_param, name);
}

sf::Vector2f Operator::getLeftPos(int ind)
{
	if (ind == 0) {
		return sf::Vector2f(position + sf::Vector2f(gui.outline_thickness / 2, gui.outline_thickness + 8));
	}
	if (ind == 1) {
		return sf::Vector2f(position + sf::Vector2f(gui.outline_thickness / 2, gui.outline_thickness + 24));
	}
	return position;
}

sf::Vector2f Operator::getRightPos()
{
	return position + sf::Vector2f(3*gui.outline_thickness/2 + 32, gui.outline_thickness + 16);
}

Operator::Operator(COMPARATOR new_type) {
	op_type = OPERATOR::COMPARATOR;
	func.comparator = new_type;
	main_box = sf::RectangleShape(sf::Vector2f(32 + gui.outline_thickness * 2, 32 + gui.outline_thickness * 2));
}

Operator::Operator(ARITHMETIC new_type)
{
	op_type = OPERATOR::ARITHMETIC;
	func.arithmetic = new_type;
	main_box = sf::RectangleShape(sf::Vector2f(32 + gui.outline_thickness * 2, 32 + gui.outline_thickness * 2));
}

Operator::Operator()
{
	main_box = sf::RectangleShape(sf::Vector2f(32 + gui.outline_thickness * 2, 32 + gui.outline_thickness * 2));
}


Operator::~Operator()
{
}

ClickResponse Operator::processLeftClick(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - getRightPos()) <= gui.obj_circle_radius + gui.outline_thickness) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
		return response;
	}
	sf::RectangleShape move_box(sf::Vector2f(32 + gui.outline_thickness * 2, 32 + gui.outline_thickness * 2));
	move_box.setPosition(position);
	if (checkIntersection(move_box, sf::Vector2f(mouse_pos))) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
		return response;
	}

	return response;
}

ClickResponse Operator::processLeftClickRelease(sf::Vector2i mouse_pos)
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

ClickResponse Operator::processMouseWheel(sf::Vector2i mouse_pos, int delta)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	sf::RectangleShape switch_box(sf::Vector2f(12, 12));
	sf::RectangleShape move_box(sf::Vector2f(32 + gui.outline_thickness * 2, 32 + gui.outline_thickness * 2));
	move_box.setPosition(position);
	switch_box.setPosition(position + move_box.getSize() / 2.0f - switch_box.getSize() / 2.0f);
	if (checkIntersection(switch_box, sf::Vector2f(mouse_pos))) {
		int int_func;
		switch (op_type) {
		case OPERATOR::COMPARATOR:
			int_func = ((int)func.comparator + delta);
			if (int_func >= (int)COMPARATOR::RESET_RIGHT) int_func = ((int)COMPARATOR::RESET_LEFT + 1);
			if (int_func <= (int)COMPARATOR::RESET_LEFT) int_func = ((int)COMPARATOR::RESET_RIGHT - 1);
			func.comparator = (COMPARATOR)int_func;
			break;
		case OPERATOR::ARITHMETIC:
			int_func = ((int)func.arithmetic + delta);
			if (int_func >= (int)ARITHMETIC::RESET_RIGHT) int_func = ((int)ARITHMETIC::RESET_LEFT + 1);
			if (int_func <= (int)ARITHMETIC::RESET_LEFT) int_func = ((int)ARITHMETIC::RESET_RIGHT - 1);
			func.arithmetic = (ARITHMETIC)int_func;
			break;
		case OPERATOR::BOOLEAN:
			int_func = ((int)func.boolean + delta);
			if (int_func >= (int)BOOLEAN_OP::RESET_RIGHT) int_func = ((int)BOOLEAN_OP::RESET_LEFT + 1);
			if (int_func <= (int)BOOLEAN_OP::RESET_LEFT) int_func = ((int)BOOLEAN_OP::RESET_RIGHT - 1);
			func.boolean = (BOOLEAN_OP)int_func;
			break;
		}
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
		return response;
	}
	return response;
}
