#include <pch.h>
#include <AnimationManager.h>

AnimationManager::AnimationManager(){
	state.edited = false;
	state.project_name = "untitled";
	state.project_path = "";
	state.out_res = sf::Vector2u(600,400);
	Video * new_video = new Video();
	new_video->loadFromFile("C:/Users/Trevor/Stuff/WavyBoi/test_files/mandelbrot.mp4");
	Video * new_video_2 = new Video();
	new_video_2->loadFromFile("C:/Users/Trevor/Stuff/WavyBoi/test_files/dogs.mp4");
	addObject(new_video);
	addObject(new_video_2);
	Channel * new_channel = new Channel(0);
	addChannel(new_channel);
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

void AnimationManager::deleteObject(Object * object_to_delete) {
	std::cout << "deleting an object" << std::endl;
	while (obj_graph[object_to_delete].inputs.size() > 0) {
		deleteLink(*obj_graph[object_to_delete].inputs.begin());
	}
	while (obj_graph[object_to_delete].outputs.size() > 0) {
		deleteLink(*obj_graph[object_to_delete].outputs.begin());
	}
	std::cout << "deleted links" << std::endl;
	obj_graph.erase(object_to_delete);
	std::cout << "deleted object from object graph" << std::endl;
	std::vector<Object *>::iterator position = objects.begin();
	while ((*position) != object_to_delete) {
		++position;
	}
	std::cout << "found the position" << std::endl;
	objects.erase(position);
	position = root_objects.begin();
	while ((*position) != object_to_delete) {
		++position;
	}
	if (position != root_objects.end()) {
		root_objects.erase(position);
	}
	std::cout << "deleted the object" << std::endl;
	delete object_to_delete;
	std::cout << "profit???" << std::endl;
}

void AnimationManager::deleteLink(Link * link_to_delete) {
	//remove link from all objects
	std::cout << "deleting a link" << std::endl;
	for (std::map<Object *, ObjectNode>::iterator it = obj_graph.begin(); it != obj_graph.end(); ++it) {
		if (it->second.inputs.count(link_to_delete) == 1) {
			it->second.inputs.erase(link_to_delete);
			if (it->second.inputs.size() == 0) {
				root_objects.push_back(it->first);
			}
		}
		if (it->second.outputs.count(link_to_delete) == 1) {
			it->second.outputs.erase(link_to_delete);
		}
	}
	std::vector<Link *>::iterator position = links.begin();
	while (*position != link_to_delete) {
		position++;
	}
	links.erase(position);
	delete link_to_delete;
}

void AnimationManager::addLink(Link * new_link)
{
	std::cout << "added new link: " << new_link->getName() << std::endl;
	//TODO: check for loops!!
	//We expect new_link->getOutObj() to not be null, otherwise we should have never entered this routine
	if (new_link->getOutObj()->getType() == OBJECT_TYPE::CHANNEL) {
		if (obj_graph[new_link->getOutObj()].inputs.size() > 0) {
			Link * link_to_delete = *(obj_graph[new_link->getOutObj()].inputs.begin());
			deleteLink(link_to_delete);
		}
	}
	links.push_back(new_link);
	Object * in, * out;
	in = new_link->getInObj();
	out = new_link->getOutObj();
	if (in != NULL) {
		obj_graph[in].outputs.insert(new_link);
	}
	if (out != NULL) {
		obj_graph[out].inputs.insert(new_link);
		//if it was a root, remove it from the root list
		if (obj_graph[out].inputs.size() == 1) {
			std::vector<Object *>::iterator position = root_objects.begin();
			while ((*position) != out) {
				++position;
			}
			root_objects.erase(position);
		}
	}
	updated_links.insert(std::pair<Link *, bool>(new_link, false));
}

void AnimationManager::addObject(Object * new_obj)
{
	objects.push_back(new_obj);
	ObjectNode new_node;
	new_node.obj = new_obj;
	new_node.updated = false;
	obj_graph.insert(std::pair<Object *, ObjectNode>(new_obj,new_node));
	root_objects.push_back(new_obj);
}

void AnimationManager::addChannel(Channel * new_channel)
{
	channels.push_back(new_channel);
	ObjectNode new_node;
	new_node.obj = new_channel;
	new_node.updated = false;
	obj_graph.insert(std::pair<Object *, ObjectNode>(new_channel, new_node));
	root_objects.push_back(new_channel);
}

bool AnimationManager::isDeleteRequested()
{
	return state.delete_selected;
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

std::vector<Channel *> AnimationManager::getChannels()
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
	state.delete_selected = false;
	for (std::map<Link *, bool>::iterator it = updated_links.begin(); it != updated_links.end(); ++it) {
		it->second = false;
	}
	for (std::map<Object *, ObjectNode>::iterator it = obj_graph.begin(); it != obj_graph.end(); ++it) {
		it->second.updated = false;
	}
	for (std::vector<Object *>::iterator it = root_objects.begin(); it != root_objects.end(); ++it) {
		std::vector<Object *> objects_to_update;
		Object * curr_obj = (*it);
		while (curr_obj != NULL) {
			if (obj_graph[curr_obj].updated == false) {
				bool ready = true;
				for (std::unordered_set<Link *>::iterator in_link_it = obj_graph[curr_obj].inputs.begin(); in_link_it != obj_graph[curr_obj].inputs.end(); ++in_link_it) {
					if (updated_links[*in_link_it] == false) {
						ready = false;
						break;
					}
				}
				if (ready) {
					curr_obj->update();
					obj_graph[curr_obj].updated = true;
					for (std::unordered_set<Link *>::iterator out_link_it = obj_graph[curr_obj].outputs.begin(); out_link_it != obj_graph[curr_obj].outputs.end(); ++out_link_it) {
						(*out_link_it)->update();
						updated_links[*out_link_it] = true;
						Object * new_obj = (*out_link_it)->getOutObj();
						new_obj->setParameter((*out_link_it)->getParameterFromLink());
						if (new_obj != NULL) {
							objects_to_update.push_back(new_obj);
						}
					}
				}
			}
			if (!objects_to_update.empty()) {
				curr_obj = objects_to_update.back();
				objects_to_update.pop_back();
			}
			else {
				curr_obj = NULL;
			}
		}
	}
	/*for (std::vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		(*it)->update();
	}
	for (std::vector<Link *>::iterator it = links.begin(); it != links.end(); ++it) {
		(*it)->update();
	}*/
	/*for (int i = 0; i < 4; i++) {
		if (channels[i] != NULL) {
			channels[i]->update();
		}
	}*/
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
	std::cout << "Clicked Edit -> Copy" << std::endl;
}

//TODO
void AnimationManager::clickEditPaste(){
	std::cout << "Clicked Edit -> Caste" << std::endl;
}

//TODO
void AnimationManager::clickEditDelete(){
	state.delete_selected = true;
	std::cout << "Clicked Edit -> Delete" << std::endl;
}