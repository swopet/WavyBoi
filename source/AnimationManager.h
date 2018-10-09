#pragma once
/* AnimationManager controls all of the everything!
it is passed by reference into ControlWindow to
get inputs and display previews. It is then passed
(also by reference) into DisplayWindow for the final
rendering. */
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Object/Object.h"
#include "Object/Video.h"
#include "Common.h"
 
#define WAVYBOITMP "C:\Users\Trevor\Documents\WavyBoi\tmp"


struct AnimationManagerState {
	bool edited;
	bool display_open = false;
	std::string project_name;
	std::string project_path;
	sf::Vector2u out_res;
};

class AnimationManager {
private:
	AnimationManagerState state;
	std::vector<Object *> objects;
public:
	AnimationManager();
	std::string getName();
	std::vector<Object *> getObjects();
	bool isEdited();
	bool isDisplayOpen();
	bool verifyClose();
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