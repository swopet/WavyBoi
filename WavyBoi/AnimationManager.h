#pragma once
/* AnimationManager controls all of the everything!
it is passed by reference into ControlWindow to
get inputs and display previews. It is then passed
(also by reference) into DisplayWindow for the final
rendering. */

#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Object/Object.h>
#include <Object/Video.h>
#include <Object/Link.h>
#include <Common.h>

struct AnimationManagerState {
	bool edited;
	bool display_open = false;
	std::string project_name;
	std::string project_path;
	sf::Vector2u out_res;
	double fps;
};

struct ObjectNode {
	std::vector<Link *> inputs;
	Object * obj;
	std::vector<Link *> outputs;
};

class AnimationManager {
private:
	AnimationManagerState state;
	std::vector<Object *> objects;
	ObjectNode channels[4];
public:
	AnimationManager();
	std::string getName();
	std::vector<Object *> getObjects();
	bool isEdited();
	bool isDisplayOpen();
	bool verifyClose();
	ObjectNode * getChannels();
	float getFPS();
	void updateFPS(sf::Time);
	void update();
	void clickDisplayOpen();
	void clickDisplayClose();
	void clickFileNew();
	void clickFileOpen();
	void clickFileSave();
	void clickFileSaveAs();
	void clickEditCut();
	void clickEditCopy();
	void clickEditPaste();
	void clickEditDelete();
};