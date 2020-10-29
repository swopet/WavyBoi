#include "pch.h"
#include "Operator.h"


void Operator::initializeElements()
{
	GUIElement main_box_element = GUIElement(GUIElement::RECTANGLE, 32 - gui.outline_thickness * 2, 32 - gui.outline_thickness * 2);
	main_box_element.setOrigin(position);
	main_box_element.setPosition(sf::Vector2f(0, 0));
	elements.push_back(main_box_element);
	GUIElement tex_box_element = GUIElement(GUIElement::RECTANGLE, 32, 32);
	tex_box_element.setOrigin(position);
	tex_box_element.setPosition(sf::Vector2f(-gui.outline_thickness, -gui.outline_thickness));
	elements.push_back(tex_box_element);
	for (int i = 0; i < 2; i++) {
		GUIElement circle = GUIElement(GUIElement::CIRCLE, gui.obj_circle_radius);
		circle.setOrigin(position);
		circle.setPosition(sf::Vector2f(0, 10 + 12 * i) +
			sf::Vector2f(-gui.outline_thickness / 2.0, -gui.outline_thickness / 2.0) +
			sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
		inputs.push_back(circle);
	}
	GUIElement circle = GUIElement(GUIElement::CIRCLE, gui.obj_circle_radius);
	circle.setOrigin(position);
	circle.setPosition(sf::Vector2f(32, 16) +
		sf::Vector2f(-gui.outline_thickness * 1.5, -gui.outline_thickness / 2.0) +
		sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
	outputs.push_back(circle);
}

void Operator::updateGUIElements()
{
	switch (op_type) {
	case OPERATOR::COMPARATOR:
		switch (func.comparator) {
		case COMPARATOR::LT:
			elements.at(1).setTexture((out_val.int_val) ? &gui.lt_true_32x32_tex : &gui.lt_false_32x32_tex);
			break;
		case COMPARATOR::LTE:
			elements.at(1).setTexture((out_val.int_val) ? &gui.lte_true_32x32_tex : &gui.lte_false_32x32_tex);
			break;
		case COMPARATOR::GT:
			elements.at(1).setTexture((out_val.int_val) ? &gui.gt_true_32x32_tex : &gui.gt_false_32x32_tex);
			break;
		case COMPARATOR::GTE:
			elements.at(1).setTexture((out_val.int_val) ? &gui.gte_true_32x32_tex : &gui.gte_false_32x32_tex);
			break;
		case COMPARATOR::EQ:
			elements.at(1).setTexture((out_val.int_val) ? &gui.eq_true_32x32_tex : &gui.eq_false_32x32_tex);
			break;
		case COMPARATOR::NEQ:
			elements.at(1).setTexture((out_val.int_val) ? &gui.neq_true_32x32_tex : &gui.neq_false_32x32_tex);
			break;
		}
		break; //OPERATOR::COMPARATOR
	case OPERATOR::ARITHMETIC:
		switch (func.arithmetic) {
		case ARITHMETIC::PLUS:
			elements.at(1).setTexture(&gui.plus_32x32_tex);
			break;
		case ARITHMETIC::MINUS:
			elements.at(1).setTexture(&gui.minus_32x32_tex);
			break;
		case ARITHMETIC::TIMES:
			elements.at(1).setTexture(&gui.times_32x32_tex);
			break;
		case ARITHMETIC::DIVIDEDBY:
			elements.at(1).setTexture(&gui.dividedby_32x32_tex);
			break;
		case ARITHMETIC::MODULO:
			elements.at(1).setTexture(&gui.modulo_32x32_tex);
			break;
		case ARITHMETIC::EXPONENT:
			elements.at(1).setTexture(&gui.exponent_32x32_tex);
			break;
		}
		break; //OPERATOR::ARITHMETIC
	}
	Object::updateGUIElements();
}

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
			out_val.int_val = (l_val == r_val) ? 1 : 0;
			break;
		case COMPARATOR::NEQ:
			out_val.int_val = (l_val != r_val) ? 1 : 0;
			break;
		case COMPARATOR::LT:
			out_val.int_val = (l_val < r_val) ? 1 : 0;
			break;
		case COMPARATOR::LTE:
			out_val.int_val = (l_val <= r_val) ? 1 : 0;
			break;
		case COMPARATOR::GT:
			out_val.int_val = (l_val > r_val) ? 1 : 0;
			break;
		case COMPARATOR::GTE:
			out_val.int_val = (l_val >= r_val) ? 1 : 0;
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
	return (checkIntersection(select_box.getGlobalBounds(), elements.at(0).getGlobalBounds()));
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

Operator::Operator(COMPARATOR new_type) {
	initializeElements();
	op_type = OPERATOR::COMPARATOR;
	func.comparator = new_type;
	//main_box = sf::RectangleShape(sf::Vector2f(32 + gui.outline_thickness * 2, 32 + gui.outline_thickness * 2));
}

Operator::Operator(ARITHMETIC new_type)
{
	initializeElements();
	op_type = OPERATOR::ARITHMETIC;
	func.arithmetic = new_type;
	//main_box = sf::RectangleShape(sf::Vector2f(32 + gui.outline_thickness * 2, 32 + gui.outline_thickness * 2));
}

Operator::Operator()
{
	initializeElements();
	//main_box = sf::RectangleShape(sf::Vector2f(32 + gui.outline_thickness * 2, 32 + gui.outline_thickness * 2));
}


Operator::~Operator()
{
}

ClickResponse Operator::processMouseWheel(sf::Vector2i mouse_pos, int delta)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (checkIntersection(inputs.at(0).getGlobalBounds(), sf::Vector2f(mouse_pos))) {
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
