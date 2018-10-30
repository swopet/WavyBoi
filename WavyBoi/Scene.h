#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

class Scene
{
private:
	sf::ContextSettings context_settings;
	sf::RenderTexture * render_target;
public:
	void update();
	void draw();
	const sf::Texture * getTexture() {
		if (render_target != NULL) {
			return &render_target->getTexture();
		}
		else return NULL;
	}
	Scene();
	~Scene();
};

