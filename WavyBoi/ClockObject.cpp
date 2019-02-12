#include "pch.h"
#include "ClockObject.h"

sf::Vector2f ClockObject::getRightPos()
{
	return sf::Vector2f(position + sf::Vector2f(71,36));
}

void ClockObject::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	target.draw(clock_rect);
	target.draw(s_ms_rect);
	target.draw(play_pause_rect);
	target.draw(zero_s_rect);
	float time = curr_time.asSeconds();
	sf::Vector2f second_hand(16.f*cos(time*PI / 30.f - PI/2.f), 16.f*sin(time*PI / 30.f - PI / 2.f));
	sf::Vertex line[] = {
		sf::Vertex(position + sf::Vector2f(36,36)),
		sf::Vertex(position + sf::Vector2f(36,36) + second_hand)
	};
	glLineWidth(gui.outline_thickness);
	glColor3f(0,0,0);
	target.draw(line, 2, sf::LineStrip);
	sf::CircleShape circle(gui.obj_circle_radius + gui.outline_thickness);
	circle.setOutlineThickness(-gui.outline_thickness);
	circle.setOutlineColor(gui.obj_outline_color);
	circle.setFillColor(gui.obj_fill_color);
	circle.setPosition(getRightPos() - sf::Vector2f(circle.getRadius(), circle.getRadius()));
	target.draw(circle);
}

void ClockObject::update()
{
	sf::Time new_time = clock.getElapsedTime();
	sf::Time elapsed_time = new_time - last_time;
	last_time = new_time;
	if (playing) curr_time = curr_time + elapsed_time;
	clock_rect.setSize(sf::Vector2f(72, 72));
	clock_rect.setPosition(position);
	clock_rect.setTexture(&gui.clock_72x72_tex);
	zero_s_rect.setSize(sf::Vector2f(24, 24));
	zero_s_rect.setPosition(position + sf::Vector2f(0, 72));
	zero_s_rect.setTexture((curr_time == sf::seconds(0)) ? &gui.zero_s_true_24x24_tex : &gui.zero_s_false_24x24_tex);
	play_pause_rect.setSize(sf::Vector2f(24, 24));
	play_pause_rect.setPosition(position + sf::Vector2f(24, 72));
	play_pause_rect.setTexture((playing) ? &gui.pause_24x24_tex : &gui.play_24x24_tex);
	s_ms_rect.setSize(sf::Vector2f(24, 24));
	s_ms_rect.setPosition(position + sf::Vector2f(48, 72));
	s_ms_rect.setTexture((seconds) ? &gui.seconds_24x24_tex : &gui.millisecoonds_24x24_tex);
	
}

bool ClockObject::checkOverlap(sf::RectangleShape select_box)
{
	return (checkIntersection(select_box, clock_rect)
		|| checkIntersection(select_box, zero_s_rect)
		|| checkIntersection(select_box, play_pause_rect)
		|| checkIntersection(select_box, s_ms_rect));
}

ClickResponse ClockObject::processLeftClick(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - getRightPos()) <= gui.obj_circle_radius + gui.outline_thickness) {
		response.type = CLICK_RESPONSE::GOT_RIGHT;
		response.clicked = true;
	}
	else if (checkIntersection(clock_rect, sf::Vector2f(mouse_pos))) {
		response.type = CLICK_RESPONSE::SELECTED;
		response.clicked = true;
	}
	else if (checkIntersection(zero_s_rect, sf::Vector2f(mouse_pos))) {
		curr_time = sf::seconds(0);
		response.type = CLICK_RESPONSE::PROCESSED;
		response.clicked = true;
	}
	else if (checkIntersection(play_pause_rect, sf::Vector2f(mouse_pos))) {
		playing = !playing;
		response.type = CLICK_RESPONSE::PROCESSED;
		response.clicked = true;
	}
	else if (checkIntersection(s_ms_rect, sf::Vector2f(mouse_pos))) {
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
}


ClockObject::~ClockObject()
{
}
