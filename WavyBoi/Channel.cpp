#include "pch.h"
#include "Channel.h"

void Channel::setParamsToDefault()
{
	render_texture = NULL;
}

void Channel::setParameter(Parameter * parameter, int ind)
{
	switch (ind) {
	case 0:
		if (parameter->getType() == PARAM_TYPE::TEXTURE) {
			render_texture = parameter->getValue().texture;
		}
		break;
	default:
		break;
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
			sf::Vector2f main_box_size = main_box.getSize() - 2.0f*sf::Vector2f(gui.outline_thickness, gui.outline_thickness);
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
	main_box.setPosition(position - size / 2.0f);
	video_box.setPosition(position - video_box.getSize() / 2.0f);
	left_pos = position - sf::Vector2f((size.x - gui.outline_thickness) / 2.0f, 0);
	left_circle.setPosition(left_pos - sf::Vector2f(left_circle.getRadius(), left_circle.getRadius()));
	target.draw(main_box);
	target.draw(video_box);
	target.draw(left_circle);
}

bool Channel::getMultipleInputsAllowed(int ind)
{
	switch (ind) {
	case 0: //this is the video input, no dupes
		return false;
		break;
	default: //this is the parameter node, dupes allowed
		return true;
		break;
	}
}

PARAM_TYPE Channel::getParamTypeForInput(int ind = 0)
{
	switch (ind) {
	case 0:
		return PARAM_TYPE::TEXTURE;
		break;
	default:
		return PARAM_TYPE::NONE;
	}
}

sf::Vector2f Channel::getLeftPos(int ind = 0) {
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
	size = sf::Vector2f(160 + gui.outline_thickness * 2, 90 + gui.outline_thickness * 2);
	position = sf::Vector2f(1000, 100 * (id+1));
	main_box = sf::RectangleShape(size);
	main_box.setOutlineThickness(-gui.outline_thickness);
	main_box.setOutlineColor(gui.obj_outline_color);
	main_box.setFillColor(sf::Color(0, 0, 0));
	left_circle = sf::CircleShape(gui.obj_circle_radius + gui.outline_thickness);
	left_circle.setOutlineThickness(-gui.outline_thickness);
	left_circle.setOutlineColor(gui.obj_outline_color);
	left_circle.setFillColor(gui.obj_fill_color);
	video_box = sf::RectangleShape(sf::Vector2f(0, 0));
	video_box.setFillColor(gui.obj_fill_color);
}

ClickResponse Channel::processLeftClickHeld(sf::Vector2i mouse_pos) {
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - left_pos) <= left_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	else if (mouse_pos.x >= position.x - size.x / 2.0f &&
		mouse_pos.y >= position.y - size.y / 2.0f &&
		mouse_pos.x < position.x + size.x / 2.0f &&
		mouse_pos.y < position.y + size.y / 2.0f) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
	}
	else {

	}
	return response;
}

ClickResponse Channel::processLeftClickRelease(sf::Vector2i mouse_pos) {
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - left_pos) <= left_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	else if (mouse_pos.x >= position.x - size.x / 2.0f &&
		mouse_pos.y >= position.y - size.y / 2.0f &&
		mouse_pos.x < position.x + size.x / 2.0f &&
		mouse_pos.y < position.y + size.y / 2.0f) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
	}
	else {

	}
	return response;
}

Channel::Channel()
{
}


Channel::~Channel()
{
}
