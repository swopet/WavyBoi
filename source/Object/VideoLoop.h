#pragma once
#include "Object/Object.h"

class VideoLoop : public Object {
private:
public:
	VideoLoop();
	~VideoLoop();
	void draw(sf::RenderTarget&, sf::RenderStates);
	ClickResponse processLeftClick(sf::Vector2i);
};