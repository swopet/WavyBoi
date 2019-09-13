#pragma once
#include <GUISettings.h>
#include <Common.h>

extern GUISettings gui;

class GUIElement
{

private:
	
	const sf::Texture * texture = NULL;
	union {
		sf::RectangleShape * rect;
		sf::CircleShape * circle;
	} shape;
	sf::Vector2f position;
	sf::Vector2f origin;
	sf::Vector2f size;
	void initializeRect();
	void initializeCircle();
public:
	enum TYPE {
		RECTANGLE,
		CIRCLE
	};
	void setSize(double radius);
	void setSize(double width, double height);
	void setOrigin(sf::Vector2f);
	void setPosition(sf::Vector2f);
	void setTexture(const sf::Texture *);
	void updateShape();
	void draw(sf::RenderTarget&, sf::RenderStates);
	sf::FloatRect getGlobalBounds();
	GUIElement();
	GUIElement(GUIElement::TYPE, double width, double height);
	GUIElement(GUIElement::TYPE, double radius);
	~GUIElement();
private:
	TYPE type;
};

