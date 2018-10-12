#include "pch.h"
#include "Channel.h"

void Channel::setParameter(Parameter * parameter)
{
	if (parameter->getType() == PARAM_TYPE::TEXTURE) {
		render_texture = parameter->getValue().texture;
	}
}

void Channel::clearTexture()
{
	render_texture = NULL;
}

void Channel::update()
{
	if (render_texture != NULL) {
		sf::Vector2f tex_size = sf::Vector2f(render_texture->getSize());
		if (tex_size.y != 0) { //don't know why it would but just to be sure
			sf::Vector2f main_box_size = main_box.getSize() - 2.0f*sf::Vector2f(CHANNEL_OUTLINE_THICKNESS, CHANNEL_OUTLINE_THICKNESS);
			//fit the preview video to the inside of the video object
			float movie_ratio = tex_size.x / tex_size.y;
			float main_box_ratio = main_box_size.x / main_box_size.y;
			sf::Vector2f video_box_size;
			if (movie_ratio > main_box_ratio) video_box_size = sf::Vector2f(main_box_size.x, main_box_size.x / movie_ratio);
			else video_box_size = sf::Vector2f(main_box_size.y * movie_ratio, main_box_size.y);
			video_box = sf::RectangleShape(video_box_size);
			video_box.setTexture(render_texture);
		}
	}
	else {
		video_box = sf::RectangleShape(sf::Vector2f(0, 0));
	}
}

void Channel::draw(sf::RenderTarget& target, sf::RenderStates states) {
	main_box.setPosition(center - size / 2.0f);
	video_box.setPosition(center - video_box.getSize() / 2.0f);
	left_pos = center - sf::Vector2f((size.x - CHANNEL_OUTLINE_THICKNESS) / 2.0f, 0);
	left_circle.setPosition(left_pos - sf::Vector2f(left_circle.getRadius(), left_circle.getRadius()));
	target.draw(main_box);
	target.draw(video_box);
	target.draw(left_circle);
}

sf::Vector2f Channel::getLeftPos() {
	return left_pos;
}

const sf::Texture * Channel::getTexture()
{
	return render_texture;
}

Channel::Channel(int new_id) {
	id = new_id;
	name = "Output Channel " + std::to_string(id);
	type = OBJECT_TYPE::CHANNEL;
	size = sf::Vector2f(160 + CHANNEL_OUTLINE_THICKNESS * 2, 90 + CHANNEL_OUTLINE_THICKNESS * 2);
	center = sf::Vector2f(1000, 100 * (id+1));
	main_box = sf::RectangleShape(size);
	main_box.setOutlineThickness(-CHANNEL_OUTLINE_THICKNESS);
	main_box.setOutlineColor(sf::Color(192, 192, 192));
	main_box.setFillColor(sf::Color(0, 0, 0));
	left_circle = sf::CircleShape(CHANNEL_CIRCLE_RADIUS + CHANNEL_OUTLINE_THICKNESS);
	left_circle.setOutlineThickness(-CHANNEL_OUTLINE_THICKNESS);
	left_circle.setOutlineColor(sf::Color(192, 192, 192));
	left_circle.setFillColor(sf::Color(128, 128, 128));
	video_box = sf::RectangleShape(sf::Vector2f(0, 0));
	video_box.setFillColor(sf::Color(128, 128, 128));
}

Channel::Channel()
{
}


Channel::~Channel()
{
}
