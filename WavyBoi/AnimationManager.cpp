#include <pch.h>
#include <AnimationManager.h>

AnimationManager::AnimationManager(){
	state.edited = false;
	state.project_name = "untitled";
	state.project_path = "";
	state.out_res = sf::Vector2u(600,400);
	
	Video * new_video = new Video();
	objects.push_back(new_video);
}

std::string AnimationManager::getName(){
	return state.project_name;
}

std::vector<Object *> AnimationManager::getObjects(){
	return objects;
}

bool AnimationManager::isEdited(){
	return state.edited;
}

bool AnimationManager::isDisplayOpen(){
	return state.display_open;
}

bool AnimationManager::verifyClose(){
	if (state.edited){
		//TODO: add warning about closing without saving project
		return true;
	}
	else {
		return true;
	}
}

void AnimationManager::clickDisplayOpen(){
	std::cout << "Clicked Display -> Open" << std::endl;
	state.display_open = true;
}

void AnimationManager::clickDisplayClose(){
	std::cout << "Clicked Display -> Close" << std::endl;
	state.display_open = false;
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