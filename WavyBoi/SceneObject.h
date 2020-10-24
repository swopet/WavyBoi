#pragma once
#include "Object/Object.h"
#include "WBScene.h"
#include <Windows.h>
class SceneObject :
	public Object
{
private:
    Plugin * scene;
    HINSTANCE hinst;
protected:
	bool ready;
    bool valid;
	sf::Mutex ready_mutex;
	sf::Vector2f size;
	void init();
	void initializeElements();
	void updateGUIElements();
    std::string filename;
public:
	SceneObject();
	SceneObject(Plugin *);
	virtual void loadScene();
	SceneObject(std::string);
	Parameter * getNewParameter();
	Parameter getParameter();
	bool getMultipleInputsAllowed(int ind) { return true; }
	bool checkOverlap(sf::RectangleShape);
    bool checkValid();
    void reload();
	void setParameter(Parameter * parameter, int ind);
	void setParamsToDefault();
	void update();
	~SceneObject();
    ClickResponse processLeftClick(sf::Vector2i mouse_pos);
};

