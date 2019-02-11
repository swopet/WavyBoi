#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <Object/Parameter.h>
#include <iostream>
#include <map>
#include <string>

class WBScene
{
protected:
	sf::ContextSettings context_settings;
	sf::RenderTexture * render_target;
	std::map<std::string, Parameter> params;
public:
	virtual void update();
	virtual void draw();
	virtual void setParamsToDefault();
	void setParameter(Parameter *);
	const sf::Texture * getTexture() {
		if (render_target != NULL) {
			return &render_target->getTexture();
		}
		else return NULL;
	}
	WBScene();
	virtual ~WBScene();
};

