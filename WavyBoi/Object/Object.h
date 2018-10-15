#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <Common.h>
#include <Object/Parameter.h>
#include <GUISettings.h>

extern GUISettings gui;

enum class OBJECT_TYPE {
	VIDEO,
	LINK,
	VIDEO_LOOP,
	IMAGE,
	CHANNEL,
	FREQBANDBLOCK,
	NONE
};


class Object {
private:
protected:
	bool visible;
	std::string name;
	OBJECT_TYPE type = OBJECT_TYPE::NONE;
	sf::Vector2f position;
public:
	std::string getName();
	void setVisible(bool);
	void setPosition(sf::Vector2f new_position) {
		position = new_position;
	}
	OBJECT_TYPE getObjectType() { return type; }
	virtual void setParamsToDefault() {};
	virtual bool getMultipleInputsAllowed(int) { return true; }
	virtual PARAM_TYPE getParamTypeForInput(int ind = 0) { return PARAM_TYPE::NONE; }
	virtual void setParameter(Parameter * parameter, int ind){}
	void setParameter(Parameter * parameter) {
		setParameter(parameter, 0);
	}
	virtual bool checkOverlap(sf::RectangleShape);
	virtual sf::Vector2f getLeftPos(int) { return position; }
	sf::Vector2f getLeftPos() { return getLeftPos(0); }
	virtual sf::Vector2f getRightPos() { return position;	}
	virtual void move(sf::Vector2f offset) {
		position = position + offset;
	}
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
	virtual ClickResponse processLeftClickHeld(sf::Vector2i vec) {
		ClickResponse response;
		response.clicked = false;
		response.type = CLICK_RESPONSE::NONE;
		return response;
	}
	virtual ClickResponse processLeftClickRelease(sf::Vector2i vec) {
		ClickResponse response;
		response.clicked = false;
		response.type = CLICK_RESPONSE::NONE;
		return response;
	}
};