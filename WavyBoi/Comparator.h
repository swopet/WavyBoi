#pragma once
#include "Object/Object.h"

enum class COMPARATOR {
	LT,
	LTE,
	EQ,
	NEQ,
	GT,
	GTE
};

class Comparator :
	public Object
{
private:
	sf::Font font;
	PARAM_TYPE left_type = PARAM_TYPE::NONE;
	PARAM_TYPE right_type = PARAM_TYPE::NONE;
	COMPARATOR func = COMPARATOR::LT;
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
	Comparator();
	~Comparator();
	ClickResponse processLeftClick(sf::Vector2i);
	ClickResponse processLeftClickRelease(sf::Vector2i);
};

