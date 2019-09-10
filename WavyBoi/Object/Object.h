#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <Common.h>
#include <Object/Parameter.h>
#include <GUISettings.h>
#include <GUIElement.h>


extern GUISettings gui;

enum class OBJECT_TYPE {
	VIDEO,
	LINK,
	VIDEO_LOOP,
	IMAGE,
	CHANNEL,
	FREQBANDBLOCK,
	SCENE,
	IMAGE_SEQUENCE,
	NONE
};


class Object {
private:
protected:
	bool visible;
	std::string name = "";
	OBJECT_TYPE type = OBJECT_TYPE::NONE;
	sf::Vector2f position;
	std::vector<GUIElement> inputs;
	std::vector<GUIElement> outputs;
	std::vector<GUIElement> elements;
public:
	std::string getName();
	void setVisible(bool);
	void setPosition(sf::Vector2f new_position) {
		position = new_position;
	}
	virtual void processNewString(std::string field, std::string input) {}
	OBJECT_TYPE getObjectType() { return type; }
	virtual void setParamsToDefault() {};
	virtual bool getMultipleInputsAllowed(int) { return false; }
	virtual PARAM_TYPE getParamTypeForInput(int ind = 0) { return PARAM_TYPE::NONE; }
	virtual void setParameter(Parameter * parameter, int ind){}
	void setParameter(Parameter * parameter) {
		setParameter(parameter, 0);
	}
	virtual bool checkOverlap(sf::RectangleShape);
	virtual sf::Vector2f getLeftPos(int pos) {
		if (pos >= inputs.size() || pos < 0) {
			return position;
		}
		else {
			sf::FloatRect rect = inputs.at(pos).getGlobalBounds();
			return sf::Vector2f(rect.left + rect.width / 2.0, rect.top + rect.height / 2.0);
		}
	}
	sf::Vector2f getLeftPos() { return getLeftPos(0); }
	virtual sf::Vector2f getRightPos(int pos) {
		if (pos >= outputs.size() || pos < 0) {
			return position;
		}
		else {
			sf::FloatRect rect = outputs.at(pos).getGlobalBounds();
			return sf::Vector2f(rect.left + rect.width / 2.0, rect.top + rect.height / 2.0);
		}
	}
	sf::Vector2f getRightPos() { return getRightPos(0);	}
	virtual void move(sf::Vector2f offset) {
		position = position + offset;
	}
	virtual Parameter * getNewParameter() {
		param new_param;
		new_param.int_val = 0;
		return new Parameter(PARAM_TYPE::NONE, new_param, name);
	}
	virtual Parameter getParameter() {
		Parameter return_parameter;
		param return_param;
		return_param.int_val = 0;
		return_parameter.setType(PARAM_TYPE::NONE);
		return_parameter.setValue(return_param);
		return return_parameter;
	}
	virtual void updateGUIElements() {
		std::vector<GUIElement> element_vectors[3] = { elements,inputs,outputs };
		for (int i = 0; i < 3; i++) {
			for (std::vector<GUIElement>::iterator it = element_vectors[i].begin(); it != element_vectors[i].end(); ++it) {
				(*it).setOrigin(position);
				(*it).updateShape();
			}
		}
	}
	virtual void initializeElements() {}
	virtual void update() {}
	virtual ~Object() {}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) {
		updateGUIElements();
		std::vector<GUIElement> element_vectors[3] = {elements,inputs,outputs};
		std::cout << elements.size() << std::endl;
		for (int i = 0; i < 3; i++) {
			for (std::vector<GUIElement>::iterator it = element_vectors[i].begin(); it != element_vectors[i].end(); ++it) {
				(*it).draw(target, states);
			}
		}
	}
	virtual ClickResponse processLeftClick(sf::Vector2i vec) {

		ClickResponse response;
		response.clicked = false;
		response.type = CLICK_RESPONSE::NONE;
		for (std::vector<GUIElement>::iterator it = outputs.begin(); it != outputs.end(); ++it) {
			if (checkIntersection((*it).getGlobalBounds(), vec)) {
				response.clicked = true;
				response.type = CLICK_RESPONSE::GOT_RIGHT;
				return response;
			}
		}
		for (std::vector<GUIElement>::iterator it = elements.begin(); it != elements.end(); ++it) {
			if (checkIntersection((*it).getGlobalBounds(), vec)) {
				response.clicked = true;
				response.type = CLICK_RESPONSE::SELECTED;
				return response;
			}
		}
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
		for (std::vector<GUIElement>::iterator it = inputs.begin(); it != inputs.end(); ++it) {
			if (checkIntersection((*it).getGlobalBounds(),vec)) {
				response.clicked = true;
				response.ind = it - inputs.begin();
				response.type = CLICK_RESPONSE::GOT_LEFT;
				return response;
			}
		}
		return response;
	}
	virtual ClickResponse processDoubleLeftClick(sf::Vector2i vec) {
		ClickResponse response;
		response.clicked = false;
		response.type = CLICK_RESPONSE::NONE;
		return response;
	}
	virtual ClickResponse processMouseWheel(sf::Vector2i, int) {
		ClickResponse response;
		response.clicked = false;
		response.type = CLICK_RESPONSE::NONE;
		return response;
	}
};