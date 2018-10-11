#include <pch.h>
#include <AnimationManager.h>

AnimationManager::AnimationManager(){
	state.edited = false;
	state.project_name = "untitled";
	state.project_path = "";
	state.out_res = sf::Vector2u(600,400);
	Video * new_video = new Video();
	Video * new_video_2 = new Video();
	objects.push_back(new_video);
	objects.push_back(new_video_2);
	channels[0].inputs.push_back(new Link(new_video, NULL, new_video->getNewParameter()));
}

std::string AnimationManager::getName(){
	return state.project_name;
}

std::vector<Object *> AnimationManager::getObjects(){
	return objects;
}

std::vector<Link*> AnimationManager::getLinks()
{
	return links;
}

void AnimationManager::addLink(Link * new_link)
{
	//TODO: check for loops!!
	links.push_back(new_link);
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

ObjectNode * AnimationManager::getChannels()
{
	return channels;
}

float AnimationManager::getFPS()
{
	return state.fps;
}

void AnimationManager::updateFPS(sf::Time frame_time)
{
	float curr_fps = 1000000.0f / (float)frame_time.asMicroseconds();
	state.fps = curr_fps;
}



void AnimationManager::update() {
	for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		(*it)->update();
	}
	for (std::vector<Link *>::iterator it = links.begin(); it != links.end(); ++it) {
		(*it)->update();
	}
	for (int i = 0; i < 4; i++) {
		for (std::vector<Link *>::iterator it = channels[i].inputs.begin(); it != channels[i].inputs.end(); ++it) {
			(*it)->update();
		}
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