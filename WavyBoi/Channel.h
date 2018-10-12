#pragma once
#include "Object/Object.h"
#include "Object/Parameter.h"

constexpr auto CHANNEL_OUTLINE_THICKNESS = 2;
constexpr auto CHANNEL_CIRCLE_RADIUS = 4;

class Channel :
	public Object
{
private:
	sf::Vector2f size;
	sf::RectangleShape main_box;
	sf::RectangleShape video_box;
	sf::CircleShape left_circle;
	sf::Vector2f left_pos;
	const sf::Texture * render_texture;
	int id;
public:
	void setParameter(Parameter *);
	void clearTexture();
	void update();
	void draw(sf::RenderTarget&, sf::RenderStates);
	sf::Vector2f getLeftPos();
	const sf::Texture * getTexture();
	Channel(int);
	Channel();
	~Channel();
};

