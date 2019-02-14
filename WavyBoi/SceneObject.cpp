#include "pch.h"
#include "SceneObject.h"


void SceneObject::init()
{
	ready_mutex.lock();
	size = sf::Vector2f(60 + gui.outline_thickness * 2, 60 + gui.outline_thickness * 2);
	if (ready && scene != NULL) {
		sf::Vector2f scene_size = sf::Vector2f(scene->getTexture()->getSize());
		if (scene_size.y != 0) {
			float ratio = scene_size.x / scene_size.y;
			size = sf::Vector2f(60 * ratio + gui.outline_thickness * 2, 60 + gui.outline_thickness * 2);
		}
	}
	ready_mutex.unlock();
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
	ready = true;
	init();
}

SceneObject::SceneObject(WBScene * new_scene)
{
	scene = new_scene;
	ready = true;
	init();
}

void SceneObject::loadScene()
{
	sf::Clock clock;
	sf::Time start_time = clock.getElapsedTime();
	std::wstring stemp = std::wstring(filename.begin(), filename.end());
	LPCWSTR sw = stemp.c_str();
	hinst = LoadLibrary(sw);
	if (hinst != NULL) {
		std::cout << "loaded " << filename << "successfully!" << std::endl;
	}
	else {
		scene = NULL;
		ready_mutex.lock();
		ready = true;
		ready_mutex.unlock();
		std::cout << "load failed on " << filename << std::endl;
		return;
	}
	auto ptr = reinterpret_cast<WBScene* (*)()>(GetProcAddress(hinst, "GetWBScene"));
	scene = ptr();
	ready_mutex.lock();
	ready = true;
	ready_mutex.unlock();
	sf::Time elapsed_time = clock.getElapsedTime() - start_time;
	std::cout << "loaded " << filename << " in " << elapsed_time.asSeconds() << "s, ready!" << std::endl;
	init();
}

SceneObject::SceneObject(std::string scene_name)
{
	ready = false;
	filename = scene_name;
	sf::Thread loadThread(&SceneObject::loadScene, this);
	loadThread.launch();
	init();
}

Parameter * SceneObject::getNewParameter()
{
	param return_param;
	ready_mutex.lock();
	if (ready && scene != NULL) {
		return_param.texture = scene->getTexture();
	}
	else {
		return_param.texture = NULL;
	}
	ready_mutex.unlock();
	return new Parameter(PARAM_TYPE::TEXTURE, return_param, name);
}

Parameter SceneObject::getParameter()
{
	param return_param;
	ready_mutex.lock();
	if (ready && scene != NULL) {
		return_param.texture = scene->getTexture();
	}
	else {
		return_param.texture = NULL;
	}
	ready_mutex.unlock();
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
	if (ready && scene != NULL)
		scene->setParameter(parameter);
}

void SceneObject::setParamsToDefault()
{
	if (ready && scene != NULL)
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
	target.draw(main_box, states);
	target.draw(scene_box, states);
	target.draw(left_circle, states);
	target.draw(right_circle, states);
}

void SceneObject::update()
{
	ready_mutex.lock();
	if (ready && scene != NULL) {
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
	ready_mutex.unlock();
}


SceneObject::~SceneObject()
{
	ready_mutex.lock();
	if (ready && scene != NULL) {
		delete scene;
		scene = NULL;
	}
	ready_mutex.unlock();
	if (hinst != NULL) {
		if (FreeLibrary(hinst)) {
			std::cout << "freed library at " << filename << std::endl;
		}
		else {
			std::cout << "no library to free for deleted scene" << std::endl;
		}
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
