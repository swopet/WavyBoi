#include "pch.h"
#include "SceneObject.h"


void SceneObject::init()
{
	ready_mutex.lock();
	size = sf::Vector2f(60,60);
	if (ready && scene != NULL) {
		sf::Vector2f scene_size = sf::Vector2f(scene->getTexture()->getSize());
		if (scene_size.y != 0) {
			float ratio = scene_size.x / scene_size.y;
			size = sf::Vector2f(60 * ratio, 60);
		}
	}
	ready_mutex.unlock();
	initializeElements();
}

void SceneObject::initializeElements()
{
	elements.clear();
	inputs.clear();
	outputs.clear();
	ready_mutex.lock();
	if (ready) {
		GUIElement main_box(GUIElement::RECTANGLE, size.x - gui.outline_thickness * 2, size.y - gui.outline_thickness * 2);
		main_box.setPosition(sf::Vector2f(0.0f, 0.0f));
		GUIElement tex_box(GUIElement::RECTANGLE, size.x, size.y);
		tex_box.setPosition(sf::Vector2f(-gui.outline_thickness, -gui.outline_thickness));
        GUIElement loop_rect(GUIElement::RECTANGLE, 24, 24);
        loop_rect.setPosition(sf::Vector2f(0, 0));
		elements.push_back(main_box);
		elements.push_back(tex_box);
        elements.push_back(loop_rect);
		GUIElement input_circle(GUIElement::CIRCLE, gui.obj_circle_radius);
		input_circle.setPosition(sf::Vector2f(0.0f, size.y / 2.0f) +
			sf::Vector2f(-gui.outline_thickness / 2.0f, -gui.outline_thickness / 2.0f) +
			sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius)
		);
		inputs.push_back(input_circle);
		GUIElement output_circle(GUIElement::CIRCLE, gui.obj_circle_radius);
		output_circle.setPosition(sf::Vector2f(size.x, size.y / 2.0f) +
			sf::Vector2f(-1.5f * gui.outline_thickness, -gui.outline_thickness / 2.0f) +
			sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius)
		);
		outputs.push_back(output_circle);
        
	}
	ready_mutex.unlock();
	updateGUIElements();
}

void SceneObject::updateGUIElements() {
	ready_mutex.lock();
	if (ready && scene != NULL)
	{
		if (elements.size() == 3) {
			elements.at(1).setTexture(scene->getTexture());
		}
	}
	else {
		if (elements.size() == 3) {
			elements.at(1).setTexture(NULL);
		}
	}
	ready_mutex.unlock();
    size_t loop_rect = 2;
    if (elements.size() >= 3) elements.at(loop_rect).setTexture(&gui.loop_false_24x24_tex);
	Object::updateGUIElements();
}

SceneObject::SceneObject()
{
	scene = NULL;
	ready = true;
	init();
}

SceneObject::SceneObject(Plugin * new_scene)
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
		std::cout << "loaded " << filename << " successfully!" << std::endl;
        valid = true;
	}
	else {
		scene = NULL;
		ready_mutex.lock();
		ready = true;
		ready_mutex.unlock();
		std::cout << "load failed on " << filename << std::endl;
        valid = false;
		return;
	}
	auto ptr = reinterpret_cast<Plugin* (*)()>(GetProcAddress(hinst, "GetWBScene"));
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

bool SceneObject::checkOverlap(sf::RectangleShape select_box)
{
	return checkIntersection(select_box.getGlobalBounds(), elements.at(0).getGlobalBounds());
}

bool SceneObject::checkValid()
{
  return valid;
}

void SceneObject::reload()
{
  ready_mutex.lock();
  if (ready) {
    if (scene != NULL) delete scene;
    scene = NULL;
    ready = false;
    valid = false;
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
  loadScene();
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

void SceneObject::update()
{
	ready_mutex.lock();
	if (ready && scene != NULL) {
		scene->update();
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

ClickResponse SceneObject::processLeftClick(sf::Vector2i mouse_pos) {
  int loop_rect = 2;
  ClickResponse response;
  response.clicked = false;
  response.type = CLICK_RESPONSE::NONE;
  if (checkIntersection(elements.at(loop_rect).getGlobalBounds(), sf::Vector2f(mouse_pos))) {
    reload();
    response.clicked = true;
    response.type = CLICK_RESPONSE::PROCESSED;
  }
  if (response.clicked) {
    return response;
  }
  else {
    response = Object::processLeftClick(mouse_pos);
  }
  return response;
}