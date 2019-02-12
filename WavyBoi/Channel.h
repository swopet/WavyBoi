#pragma once
#include "Object/Object.h"
#include "Object/Parameter.h"
#include "WBScene.h"

class Channel :
	public Object
{
private:
	WBScene * test_scene; //delete once i'm done testing this shiiiit
	sf::Vector2f size;
	sf::RectangleShape main_box;
	sf::RectangleShape video_box;
	sf::CircleShape left_circle;
	sf::Vector2f left_pos;
	const sf::Texture * render_texture = NULL;
	int id;
public:
	void setParamsToDefault();
	void setParameter(Parameter *, int);
	void clearTexture();
	void update();
	bool checkOverlap(sf::RectangleShape select_box);
	void draw(sf::RenderTarget&, sf::RenderStates);
	bool getMultipleInputsAllowed(int);
	PARAM_TYPE getParamTypeForInput(int);
	sf::Vector2f getLeftPos(int);
	const sf::Texture * getTexture();
	Channel(int);
	ClickResponse processLeftClickHeld(sf::Vector2i mouse_pos);
	ClickResponse processLeftClickRelease(sf::Vector2i mouse_pos);
	Channel();
	~Channel();
};

