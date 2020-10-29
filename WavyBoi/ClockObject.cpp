#include "pch.h"
#include "ClockObject.h"

void ClockObject::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	Object::draw(target, states);
	float time = curr_time.asSeconds();
	sf::Vector2f second_hand(16.f*cos(time*PI / 30.f - PI/2.f), 16.f*sin(time*PI / 30.f - PI / 2.f));
	sf::Vertex line[2] = {
		sf::Vertex(position + sf::Vector2f(36,36)),
		sf::Vertex(position + sf::Vector2f(36,36) + second_hand)
	};
	line[0].color = sf::Color(sf::Color::White);
	line[1].color = sf::Color(sf::Color::White);
	target.draw(line, 2, sf::Lines, states);
}

void ClockObject::initializeElements()
{
	GUIElement clock_rect(GUIElement::RECTANGLE, 72, 72);
	clock_rect.setPosition(sf::Vector2f(0, 0));
	clock_rect.setTexture(&gui.clock_72x72_tex);
	GUIElement zero_s_rect(GUIElement::RECTANGLE, 24, 24);
	zero_s_rect.setPosition(sf::Vector2f(0, 72));
	GUIElement play_pause_rect(GUIElement::RECTANGLE, 24, 24);
	play_pause_rect.setPosition(sf::Vector2f(24, 72));
	GUIElement s_ms_rect(GUIElement::RECTANGLE, 24, 24);
	s_ms_rect.setPosition(sf::Vector2f(48, 72));
	elements.push_back(clock_rect);
	elements.push_back(zero_s_rect);
	elements.push_back(play_pause_rect);
	elements.push_back(s_ms_rect);
	GUIElement output_circle(GUIElement::CIRCLE, gui.obj_circle_radius);
	outputs.push_back(output_circle);
	updateGUIElements();
}

void ClockObject::updateGUIElements()
{
	int zero_s_rect = 1;
	int play_pause_rect = 2;
	int s_ms_rect = 3;
	elements.at(zero_s_rect).setTexture((curr_time == sf::seconds(0)) ? &gui.zero_s_true_24x24_tex : &gui.zero_s_false_24x24_tex);
	elements.at(play_pause_rect).setTexture((playing) ? &gui.pause_24x24_tex : &gui.play_24x24_tex);
	elements.at(s_ms_rect).setTexture((seconds) ? &gui.seconds_24x24_tex : &gui.millisecoonds_24x24_tex);
	outputs.at(0).setPosition(sf::Vector2f(72,36) + 
		sf::Vector2f(gui.outline_thickness / 2.0, 0.0) +
		sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius));
	Object::updateGUIElements();
}

void ClockObject::update()
{
	sf::Time new_time = clock.getElapsedTime();
	sf::Time elapsed_time = new_time - last_time;
	last_time = new_time;
	if (playing) curr_time = curr_time + elapsed_time;
	
}

bool ClockObject::checkOverlap(sf::RectangleShape select_box)
{
	for (std::vector<GUIElement>::iterator it = elements.begin(); it != elements.end(); ++it) {
		if (checkIntersection(select_box.getGlobalBounds(), (*it).getGlobalBounds()))
			return true;
	}
	return false;
}

void ClockObject::setPlaying(bool new_playing) {
  playing = new_playing;
}

ClickResponse ClockObject::processLeftClick(sf::Vector2i mouse_pos)
{
	int zero_s_rect = 1;
	int play_pause_rect = 2;
	int s_ms_rect = 3;
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (checkIntersection(outputs.at(0).getGlobalBounds(), mouse_pos)){
		response.type = CLICK_RESPONSE::GOT_RIGHT;
		response.clicked = true;
	}
	else if (checkIntersection(elements.at(0).getGlobalBounds(), mouse_pos)) {
		response.type = CLICK_RESPONSE::SELECTED;
		response.clicked = true;
	}
	else if (checkIntersection(elements.at(zero_s_rect).getGlobalBounds(), mouse_pos)) {
		curr_time = sf::seconds(0);
		response.type = CLICK_RESPONSE::PROCESSED;
		response.clicked = true;
	}
	else if (checkIntersection(elements.at(play_pause_rect).getGlobalBounds(), mouse_pos)) {
		playing = !playing;
		response.type = CLICK_RESPONSE::PROCESSED;
		response.clicked = true;
	}
	else if (checkIntersection(elements.at(s_ms_rect).getGlobalBounds(), mouse_pos)) {
		seconds = !seconds;
		response.type = CLICK_RESPONSE::PROCESSED;
		response.clicked = true;
	}
	return response;
}

Parameter ClockObject::getParameter()
{
	Parameter return_parameter;
	param return_param;
	return_param.float_val = (seconds) ? curr_time.asSeconds() : curr_time.asMilliseconds();
	return_parameter.setType(PARAM_TYPE::FLOAT);
	return_parameter.setValue(return_param);
	return return_parameter;
}

ClockObject::ClockObject()
{
	playing = false;
	curr_time = sf::seconds(0);
	initializeElements();
}


ClockObject::~ClockObject()
{
}
