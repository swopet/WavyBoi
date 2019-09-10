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

}

bool Channel::checkOverlap(sf::RectangleShape select_box) {
	return checkIntersection(select_box.getGlobalBounds(), elements.at(0).getGlobalBounds());
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

const sf::Texture * Channel::getTexture()
{
	return render_texture;
}

void Channel::initializeElements() {
	GUIElement main_box(GUIElement::RECTANGLE, 160 - gui.outline_thickness * 2, 90 - gui.outline_thickness * 2);
	main_box.setPosition(sf::Vector2f(0, 0));
	elements.push_back(main_box);
	GUIElement video_box(GUIElement::RECTANGLE, 160, 90);
	video_box.setPosition(sf::Vector2f(-gui.outline_thickness, -gui.outline_thickness));
	elements.push_back(video_box);
	GUIElement input_circle(GUIElement::CIRCLE, gui.obj_circle_radius);
	input_circle.setPosition(sf::Vector2f(0, 45) +
		sf::Vector2f(-gui.outline_thickness / 2.0, 0.0) +
		sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
	inputs.push_back(input_circle);
	updateGUIElements();
}

void Channel::updateGUIElements() {
	int tex_box = 1;
	if (render_texture != NULL) {
		elements.at(tex_box).setTexture((sf::Texture *)render_texture);
		sf::Vector2f tex_size = sf::Vector2f(render_texture->getSize());
		sf::Vector2f main_box_size(160, 90);
		float movie_ratio = tex_size.x / tex_size.y;
		float main_box_ratio = main_box_size.x / main_box_size.y;
		sf::Vector2f video_box_size;
		if (movie_ratio > main_box_ratio) video_box_size = sf::Vector2f(main_box_size.x, main_box_size.x / movie_ratio);
		else video_box_size = sf::Vector2f(main_box_size.y * movie_ratio, main_box_size.y);
		elements.at(0).setSize(video_box_size.x - gui.outline_thickness * 2, video_box_size.y - gui.outline_thickness * 2);
		elements.at(tex_box).setSize(video_box_size.x, video_box_size.y);
		elements.at(tex_box).setPosition(sf::Vector2f(-gui.outline_thickness, -gui.outline_thickness));
		inputs.at(0).setPosition(sf::Vector2f(0,video_box_size.y/2.0) +
			sf::Vector2f(-gui.outline_thickness / 2.0, -gui.outline_thickness) +
			sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
	}
	else {
		elements.at(tex_box).setSize(0, 0);
	}
	Object::updateGUIElements();
}

Channel::Channel(int new_id) {
	id = new_id;
	name = "Output Channel " + std::to_string(id);
	type = OBJECT_TYPE::CHANNEL;
	position = sf::Vector2f(1000, 100 * (id+1));
	initializeElements();
}

Channel::Channel()
{

}


Channel::~Channel()
{
}
