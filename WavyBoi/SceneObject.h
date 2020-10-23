#pragma once
#include "Object/Object.h"
#include "WBScene.h"
#include <Windows.h>
class SceneObject :
	public Object
{
private:
	bool ready;
    bool valid;
	std::string filename;
	sf::Mutex ready_mutex;
	WBScene * scene;
	sf::Vector2f size;
	HINSTANCE hinst;
	void init();
	void initializeElements();
	void updateGUIElements();
public:
	SceneObject();
	SceneObject(WBScene *);
	void loadScene();
	SceneObject(std::string);
	Parameter * getNewParameter();
	Parameter getParameter();
	bool getMultipleInputsAllowed(int ind) { return true; }
	bool checkOverlap(sf::RectangleShape);
    bool checkValid();
	void setParameter(Parameter * parameter, int ind);
	void setParamsToDefault();
	void update();
	~SceneObject();
};

