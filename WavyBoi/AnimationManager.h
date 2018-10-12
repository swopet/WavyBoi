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
#include <Channel.h>
#include <Common.h>
#include <map>
#include <vector>
#include <unordered_set>

struct AnimationManagerState {
	bool edited;
	bool display_open = false;
	bool delete_selected = false;
	std::string project_name;
	std::string project_path;
	sf::Vector2u out_res;
	double fps;
};

struct ObjectNode {
	std::unordered_set<Link *> inputs;
	Object * obj;
	std::unordered_set<Link *> outputs;
	bool updated;
};

class AnimationManager {
private:
	AnimationManagerState state;
	std::vector<Object *> objects;
	std::vector<Link *> links;
	std::vector<Channel *> channels;
	std::map<Link *,bool> updated_links;
	std::map<Object *, ObjectNode> obj_graph;
	std::vector<Object *> root_objects;
public:
	AnimationManager();
	std::string getName();
	std::vector<Channel *> getChannels();
	std::vector<Object *> getObjects();
	std::vector<Link *> getLinks();
	void deleteObject(Object *);
	void deleteLink(Link *);
	void addLink(Link *);
	void addObject(Object *);
	void addChannel(Channel *);
	bool isDeleteRequested();
	bool isEdited();
	bool isDisplayOpen();
	bool verifyClose();
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