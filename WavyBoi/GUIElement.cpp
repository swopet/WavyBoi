#include "pch.h"
#include "GUIElement.h"


void GUIElement::initializeRect()
{
	shape.rect = new sf::RectangleShape();
	shape.rect->setSize(size);
	updateShape();
}

void GUIElement::initializeCircle()
{
	shape.circle = new sf::CircleShape();
	shape.circle->setPointCount(24);
	shape.circle->setRadius(size.x);
	updateShape();
}

void GUIElement::setSize(double radius)
{
	size = sf::Vector2f(radius, radius);
}

void GUIElement::setSize(double width, double height)
{
	size = sf::Vector2f(width, height);
}

void GUIElement::setOrigin(sf::Vector2f new_origin)
{
	origin = new_origin;
}

void GUIElement::setPosition(sf::Vector2f new_position)
{
	position = new_position;
}

void GUIElement::setTexture(const sf::Texture * new_texture)
{
	texture = new_texture;
	switch (type) {
	case RECTANGLE:
		shape.rect->setTexture(texture);
		break;
	case CIRCLE:
	default:
		break;
	}
	updateShape();
}

void GUIElement::updateShape()
{
	switch (type) {
	case GUIElement::RECTANGLE:
		if (shape.rect == NULL)
			initializeRect();
		shape.rect->setPosition(origin + position);
		shape.rect->setSize(size);
		if (texture == NULL) {
			shape.rect->setOutlineThickness(gui.outline_thickness);
			shape.rect->setFillColor(gui.obj_fill_color);
			shape.rect->setOutlineColor(gui.obj_outline_color);
		}
		else {
			shape.rect->setOutlineThickness(0);
		}
		break;
	case GUIElement::CIRCLE:
		if (shape.circle == NULL)
			initializeCircle();
		shape.circle->setPosition(origin + position);
		shape.circle->setRadius(size.x);
		if (texture == NULL) {
			shape.circle->setOutlineThickness(gui.outline_thickness);
			shape.circle->setFillColor(gui.obj_fill_color);
			shape.circle->setOutlineColor(gui.obj_outline_color);
		}
		else {
			shape.circle->setOutlineThickness(0);
		}
		break;
	default:
		return;
		break;
	}
}

void GUIElement::draw(sf::RenderTarget& target, sf::RenderStates states)
{
	if (type == RECTANGLE) {
		target.draw(*shape.rect, states);
	}
	if (type == CIRCLE) {
		target.draw(*shape.circle, states);
	}
}

sf::FloatRect GUIElement::getGlobalBounds()
{
	switch (type) {
	case RECTANGLE:
		if (shape.rect != NULL)
			return shape.rect->getGlobalBounds();
		break;
	case CIRCLE:
		if (shape.circle != NULL)
			return shape.circle->getGlobalBounds();
		break;
	}
	return sf::FloatRect(0, 0, 0, 0);
}

GUIElement::GUIElement()
{
}

GUIElement::GUIElement(GUIElement::TYPE new_type, double width, double height)
{
	origin = sf::Vector2f(0, 0);
	position = sf::Vector2f(0, 0);
	type = new_type;
	setSize(width, height);
	updateShape();
}

GUIElement::GUIElement(GUIElement::TYPE new_type, double radius)
{
	origin = sf::Vector2f(0, 0);
	position = sf::Vector2f(0, 0);
	type = new_type;
	setSize(radius);
	updateShape();
}

GUIElement::~GUIElement()
{
}
