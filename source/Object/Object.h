#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Common.h"

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
	void move(sf::Vector2f);
	virtual ~Object(){};
	virtual void draw(sf::RenderTarget&, sf::RenderStates){}
	virtual ClickResponse processLeftClick(sf::Vector2i){};
};