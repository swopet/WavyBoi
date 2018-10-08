#pragma once
/* AnimationManager controls all of the everything!
it is passed by reference into ControlWindow to
get inputs and display previews. It is then passed
(also by reference) into DisplayWindow for the final
rendering. */
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
 
#define WAVYBOITMP "C:\Users\Trevor\Documents\WavyBoi\tmp"


struct AnimationManagerState {
	bool edited;
	std::string project_name;
	std::string project_path;
	sf::Vector2u out_res;
};

class AnimationManager {
private:
	AnimationManagerState state;
public:
	AnimationManager();
	std::string get_name();
	bool is_edited();
	bool verify_close();
	void clickFileNew();
	void clickFileOpen();
	void clickFileSave();
	void clickFileSaveAs();
	void clickEditCut();
	void clickEditCopy();
	void clickEditPaste();
	void clickEditDelete();
};