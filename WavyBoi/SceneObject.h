#pragma once
#include "Object/Object.h"
#include "WBScene.h"
#include <Windows.h>
class SceneObject :
	public Object
{
private:
	bool ready;
	std::string filename;
	sf::Mutex ready_mutex;
	WBScene * scene;
	sf::Vector2f size;
	sf::RectangleShape main_box;
	sf::RectangleShape scene_box;
	sf::CircleShape left_circle;
	sf::Vector2f left_pos;
	sf::CircleShape right_circle;
	sf::Vector2f right_pos;
	HINSTANCE hinst;
	void init();
public:
	SceneObject();
	SceneObject(WBScene *);
	void loadScene();
	SceneObject(std::string);
	Parameter * getNewParameter();
	Parameter getParameter();
	sf::Vector2f getLeftPos(int id);
	sf::Vector2f getRightPos();
	bool getMultipleInputsAllowed(int ind) { return true; }
	bool checkOverlap(sf::RectangleShape);
	void setParameter(Parameter * parameter, int ind);
	void setParamsToDefault();
	void draw(sf::RenderTarget&, sf::RenderStates);
	void update();
	~SceneObject();
	ClickResponse processLeftClick(sf::Vector2i);
	ClickResponse processLeftClickHeld(sf::Vector2i);
	ClickResponse processLeftClickRelease(sf::Vector2i mouse_pos);
};

