#pragma once
#include <pch.h>
#include <Object/Object.h>
#include <Common.h>

class VideoLoop : public Object {
private:
public:
	VideoLoop();
	~VideoLoop();
	void draw(sf::RenderTarget&, sf::RenderStates);
	ClickResponse processLeftClick(sf::Vector2i);
};