#include "AnimationManager.h"

AnimationManager::AnimationManager(){
	state.edited = false;
	state.project_name = "untitled";
	state.project_path = "";
	state.out_res = sf::Vector2u(600,400);
}

std::string AnimationManager::get_name(){
	return state.project_name;
}

bool AnimationManager::is_edited(){
	return state.edited;
}

bool AnimationManager::verify_close(){
	if (state.edited){
		//TODO: add warning about closing without saving project
		return true;
	}
	else {
		return true;
	}
}

//TODO
void AnimationManager::clickFileNew(){
	std::cout << "Clicked File -> New" << std::endl;
}

//TODO
void AnimationManager::clickFileOpen(){
	std::cout << "Clicked File -> Open" << std::endl;
}

//TODO
void AnimationManager::clickFileSave(){
	std::cout << "Clicked File -> Save" << std::endl;
}

//TODO
void AnimationManager::clickFileSaveAs(){
	std::cout << "Clicked File -> Save As" << std::endl;
}

//TODO
void AnimationManager::clickEditCut(){
	std::cout << "Clicked Edit -> Cut" << std::endl;
}

//TODO
void AnimationManager::clickEditCopy(){
	std::cout << "Clicked Edit -> Cut" << std::endl;
}

//TODO
void AnimationManager::clickEditPaste(){
	std::cout << "Clicked Edit -> Cut" << std::endl;
}

//TODO
void AnimationManager::clickEditDelete(){
	std::cout << "Clicked Edit -> Cut" << std::endl;
}