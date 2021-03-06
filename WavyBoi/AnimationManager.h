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
#include <ImageSequence.h>
#include <SceneObject.h>
#include <Shader.h>
#include <Multiplexer.h>
#include <AudioHandler.h>
#include <FreqBandBlock.h>
#include <Operator.h>
#include <Channel.h>
#include <Common.h>
#include <IntObject.h>
#include <FloatObject.h>
#include <ClockObject.h>
#include <MathFunction.h>
#include <Register.h>
#include <math.h>
#include <map>
#include <vector>
#include <unordered_set>
#include <Preferences.h>

struct AnimationManagerState {
	bool edited = false;
	bool display_open = false;
	bool delete_selected = false;
    bool resource_cache_updated = false;
	std::string project_name;
	std::string project_path;
	double fps;
};

struct ResourceCache {
  std::vector<std::string> videos;
  std::vector<std::string> plugins;
  std::vector<std::string> shaders;
};

struct ObjectNode {
	std::unordered_set<Link *> inputs;
	Object * obj = NULL;
	std::unordered_set<Link *> outputs;
	bool updated;
};

class AnimationManager {
  enum RESOURCE_TYPE {
  VIDEO,
  PLUGIN,
  SHADER
};
private:
	Preferences preferences;
	AnimationManagerState state;
    ResourceCache resource_cache;
	std::vector<Object *> objects;
	std::vector<Link *> links;
	std::vector<Channel *> channels;
	std::map<Link *,bool> updated_links;
	std::map<Object *, ObjectNode> obj_graph;
	std::vector<Object *> root_objects;
	std::vector<Object *> freq_band_objects;
    std::vector<Object *> register_objects;
	AudioHandler * audio_handler;
public:
	AnimationManager();
	~AnimationManager();
	Preferences getPreferences();
	AudioHandler * getAudioHandler();
	std::string getName();
	std::vector<Channel *> getChannels();
	std::vector<Object *> getObjects();
	std::vector<Link *> getLinks();
	bool processCommand(std::vector<std::string> args);
    void loadResourceCache();
    void writeResourceCache();
    void updateResourceCache(std::string, RESOURCE_TYPE);
    ResourceCache * getResourceCache();
	void decrementLinkOutIndsGreaterThan(int, Object *);
	void pushToTop(Object *);
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
	void clickNewLT();
	void clickNewLTE();
	void clickNewGTE();
	void clickNewGT();
	void clickNewEQ();
	void clickNewNEQ();
	void clickNewPlus();
	void clickNewMinus();
	void clickNewTimes();
	void clickNewDividedby();
	void clickNewModulo();
	void clickNewExponent();
	void clickNewInt();
	void clickNewFloat();
	void clickNewPI();
	void clickNewE();
	void clickNewCos();
	void clickNewSin();
	void clickNewTan();
	void clickNewLog();
	void clickNewMux();
	void clickNewClock();
    void clickNewRegister();
};