#include "pch.h"
#include "SceneObject.h"


void SceneObject::init()
{
	if (scene == NULL) {
		size = sf::Vector2f(60 + gui.outline_thickness * 2, 60 + gui.outline_thickness * 2);
	}
	else {
		sf::Vector2f scene_size = sf::Vector2f(scene->getTexture()->getSize());
		if (scene_size.y != 0) {
			float ratio = scene_size.x / scene_size.y;
			size = sf::Vector2f(60 * ratio + gui.outline_thickness * 2, 60 + gui.outline_thickness * 2);
		}
		else {
			size = sf::Vector2f(60 + gui.outline_thickness * 2, 60 + gui.outline_thickness * 2);
		}
	}
	main_box = sf::RectangleShape(size);
	main_box.setOutlineThickness(-gui.outline_thickness);
	main_box.setOutlineColor(gui.obj_outline_color);
	main_box.setFillColor(sf::Color(0, 0, 0));
	left_circle = sf::CircleShape(gui.obj_circle_radius + gui.outline_thickness);
	left_circle.setOutlineThickness(-gui.outline_thickness);
	left_circle.setOutlineColor(gui.obj_outline_color);
	left_circle.setFillColor(gui.obj_fill_color);
	right_circle = left_circle;
	scene_box = sf::RectangleShape(sf::Vector2f(0, 0));
	scene_box.setFillColor(gui.obj_fill_color);
}

SceneObject::SceneObject()
{
	scene = NULL;
	init();
}

SceneObject::SceneObject(WBScene * new_scene)
{
	scene = new_scene;
	init();
}

SceneObject::SceneObject(std::string filename)
{
	std::wstring stemp = std::wstring(filename.begin(), filename.end());
	LPCWSTR sw = stemp.c_str();
	hinst = LoadLibrary(sw);
	auto ptr = reinterpret_cast<WBScene* (*)()>(GetProcAddress(hinst, "GetWBScene"));
	scene = ptr();
	init();
}

Parameter * SceneObject::getNewParameter()
{
	param return_param;
	if (scene != NULL) {
		return_param.texture = scene->getTexture();
	}
	else {
		return_param.texture = NULL;
	}
	return new Parameter(PARAM_TYPE::TEXTURE, return_param, name);
}

Parameter SceneObject::getParameter()
{
	param return_param;
	if (scene != NULL) {
		return_param.texture = scene->getTexture();
	}
	else {
		return_param.texture = NULL;
	}
	return Parameter(PARAM_TYPE::TEXTURE, return_param, name);
}

sf::Vector2f SceneObject::getLeftPos(int id = 0)
{
	return left_pos;
}

sf::Vector2f SceneObject::getRightPos()
{
	return right_pos;
}

bool SceneObject::checkOverlap(sf::RectangleShape select_box)
{
	return checkIntersection(select_box, main_box);
}

void SceneObject::setParameter(Parameter * parameter, int ind)
{
	scene->setParameter(parameter);
}

void SceneObject::setParamsToDefault()
{
	scene->setParamsToDefault();
}

void SceneObject::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	main_box.setPosition(position - size / 2.0f);
	scene_box.setPosition(position - scene_box.getSize() / 2.0f);
	left_pos = position - sf::Vector2f((size.x - gui.outline_thickness) / 2.0f, 0);
	left_circle.setPosition(left_pos - sf::Vector2f(left_circle.getRadius(), left_circle.getRadius()));
	right_pos = position + sf::Vector2f((size.x - gui.outline_thickness) / 2.0f, 0);
	right_circle.setPosition(right_pos - sf::Vector2f(right_circle.getRadius(), right_circle.getRadius()));
	target.draw(main_box);
	target.draw(scene_box);
	target.draw(left_circle);
	target.draw(right_circle);
}

void SceneObject::update()
{
	if (scene != NULL) {
		scene->update();
		scene_box.setTexture(scene->getTexture());
		sf::Vector2f scene_size = sf::Vector2f(scene->getTexture()->getSize());
		if (scene_size.y != 0) {
			float ratio = scene_size.x / scene_size.y;
			size = sf::Vector2f(60 * ratio + gui.outline_thickness * 2, 60 + gui.outline_thickness * 2);
		}
		else {
			size = sf::Vector2f(60 + gui.outline_thickness * 2, 60 + gui.outline_thickness * 2);
		}
		scene_box.setSize(size - sf::Vector2f(gui.outline_thickness * 2, gui.outline_thickness * 2));
		main_box.setSize(size);
	}
	else {
		scene_box.setTexture(NULL);
	}
}


SceneObject::~SceneObject()
{
	if (scene != NULL) {
		delete scene;
		scene = NULL;
	}
}

ClickResponse SceneObject::processLeftClick(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - left_pos) <= left_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	else if (length(sf::Vector2f(mouse_pos) - right_pos) <= right_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
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

ClickResponse SceneObject::processLeftClickHeld(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - left_pos) <= left_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	else if (length(sf::Vector2f(mouse_pos) - right_pos) <= right_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
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

ClickResponse SceneObject::processLeftClickRelease(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - left_pos) <= left_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	else if (length(sf::Vector2f(mouse_pos) - right_pos) <= right_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
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
