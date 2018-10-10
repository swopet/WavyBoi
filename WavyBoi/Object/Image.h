#pragma once
#include <Object/Object.h>

class Image : public Object{
private:
public:
	Image();
	~Image();
	void draw(sf::RenderTarget&, sf::RenderStates);
	ClickResponse processLeftClick(sf::Vector2i);
};