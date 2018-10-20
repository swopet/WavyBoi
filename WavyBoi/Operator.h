#pragma once
#include "Object/Object.h"

enum class OPERATOR {
	COMPARATOR,
	ARITHMETIC,
	BOOLEAN
};

enum class BOOLEAN_OP {
	RESET_LEFT = 0,
	AND,
	OR,
	RESET_RIGHT
};

enum class ARITHMETIC {
	RESET_LEFT = 0,
	PLUS,
	MINUS,
	DIVIDEDBY,
	TIMES,
	EXPONENT,
	MODULO,
	RESET_RIGHT,
};

enum class COMPARATOR {
	RESET_LEFT = 0,
	LT,
	LTE,
	EQ,
	NEQ,
	GT,
	GTE,
	RESET_RIGHT
};

class Operator :
	public Object
{
private:
	PARAM_TYPE left_type = PARAM_TYPE::NONE;
	PARAM_TYPE right_type = PARAM_TYPE::NONE;
	union {
		COMPARATOR comparator;
		ARITHMETIC arithmetic;
		BOOLEAN_OP boolean;
	} func;
	OPERATOR op_type;
	param left_val;
	param right_val;
	param out_val;
public:
	void update();
	param getVal();
	void setParameter(Parameter * parameter, int ind);
	void draw(sf::RenderTarget &, sf::RenderStates);
	sf::Vector2f getLeftPos(int);
	sf::Vector2f getRightPos();
	Operator();
	Operator(COMPARATOR);
	Operator(ARITHMETIC);
	Operator(BOOLEAN_OP);
	~Operator();
	ClickResponse processLeftClick(sf::Vector2i);
	ClickResponse processLeftClickRelease(sf::Vector2i);
	ClickResponse processMouseWheel(sf::Vector2i, int);
};

