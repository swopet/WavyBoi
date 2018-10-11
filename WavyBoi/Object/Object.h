#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <Common.h>
#include <Object/Parameter.h>

enum class OBJECT_TYPE {
	VIDEO,
	LINK,
	VIDEO_LOOP,
	IMAGE,
	NONE
};

class Object {
private:
protected:
	bool visible;
	std::string name;
	OBJECT_TYPE type;
	sf::Vector2f center;
public:
	std::string getName();
	void setVisible(bool);
	virtual void move(sf::Vector2f);
	virtual Parameter * getNewParameter() {
		param new_param;
		new_param.int_val = 0;
		return new Parameter(PARAM_TYPE::INT, new_param, name);
	}
	virtual param getVal() {
		param return_param;
		return_param.int_val = 0;
		return return_param;
	}
	virtual void update() {}
	virtual ~Object() {}
	virtual void draw(sf::RenderTarget&, sf::RenderStates) {}
	virtual ClickResponse processLeftClick(sf::Vector2i vec) {
		ClickResponse response;
		response.clicked = false;
		response.type = CLICK_RESPONSE::NONE;
		return response; 
	}
};