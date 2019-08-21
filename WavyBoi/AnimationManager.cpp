#include <pch.h>
#include <AnimationManager.h>

AnimationManager::AnimationManager(){

	audio_handler = new AudioHandler();
	//audio_handler = NULL;
	//audio_handler->start(AudioHandler::AUDIO_FILE, std::string("C:/Users/Trevor/Stuff/Rearranger_Videos/Rearranger.wav"));
	state.edited = false;
	state.project_name = "untitled";
	state.project_path = "";
	//processCommand(std::vector<std::string>({ "loadImageSequence","C:/Users/Trevor/Stuff/Rearranger_Videos/cape_lookout_1" }));
	Channel * new_channel = new Channel(0);
	addChannel(new_channel);
	std::cout << "Added new Channel" << std::endl; //debug7.9
}

AnimationManager::~AnimationManager()
{
	delete audio_handler;
}

AudioHandler * AnimationManager::getAudioHandler()
{
	return audio_handler;
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

bool AnimationManager::processCommand(std::vector<std::string> args) {
	if (args.size() == 0) return false;
	std::cout << args[0].length() << std::endl;
	if (args[0].compare("loadScene") == 0) {
		if (args.size() != 2) {
			std::cout << "usage: loadScene <PATH_TO_SCENE>" << std::endl;
			return false;
		}
		else {
			SceneObject * new_scene = new SceneObject(args[1]);
			new_scene->setPosition(sf::Vector2f(50, 50));
			addObject(new_scene);
			return true;
		}
	}
	else if (args[0].compare("loadVideo") == 0) {
		if (args.size() != 2) {
			std::cout << "usage: loadVideo <PATH_TO_VIDEO>" << std::endl;
			return false;
		}
		else {
			Video * new_video = new Video();
			new_video->loadFromFile(args[1]);
			addObject(new_video);
			return true;
		}
	}
	else if (args[0].compare("loadImageSequence") == 0) {
		if (args.size() != 2) {
			std::cout << "usage: loadImageSequence <PATH_TO_DIR>" << std::endl;
			return false;
		}
		ImageSequence * new_image_seq = new ImageSequence();
		new_image_seq->loadFromPath(args[1]);
		addObject(new_image_seq);
		return true;
	}
	else if (args[0].compare("addRange") == 0) {
		if (args.size() != 3) {
			std::cout << "usage: addRange <low_val> <high_val>" << std::endl;
			return false;
		}
		else {
			int low_val;
			int high_val;
			try {
				low_val = std::stoi(args[1]);
			}
			catch (std::invalid_argument ex) {
				std::cout << "Invalid argument: " << args[1] << std::endl;
				return false;
			}
			try {
				high_val = std::stoi(args[2]);
			}
			catch (std::invalid_argument ex) {
				std::cout << "Invalid argument: " << args[2] << std::endl;
				return false;
			}
			if (low_val > high_val || low_val < 0) {
				std::cout << "Low val must be less than high val and greater than 0" << std::endl;
				return false;
			}
			FreqBandBlock * new_band = new FreqBandBlock();
			new_band->setRange(std::pair<int, int>(low_val, high_val));
			addObject(new_band);
			return true;
		}
	}
	else if (args[0].compare("addFloat") == 0) {
		if (args.size() > 2) {
			std::cout << "usage: addFloat <val>" << std::endl;
			return false;
		}
			float val = 0.0;

			if (args.size() == 2) {
				if (args[1].compare("pi") == 0) {
					val = PI;
				}
				else {
					try {
						val = std::stof(args[1]);
					}
					catch (std::invalid_argument ex) {
						std::cout << "Invalid float: " << args[1] << std::endl;
						return false;
					}
			}
			FloatObject * new_float = new FloatObject(val);
			addObject(new_float);
		}
	}
	else if (args[0].compare("addInt") == 0) {
		if (args.size() > 2) {
			std::cout << "usage: addInt <val>" << std::endl;
			return false;
		}
		int val = 0;

		if (args.size() == 2) {
			try {
				val = std::stoi(args[1]);
			}
			catch (std::invalid_argument ex) {
				std::cout << "Invalid int: " << args[1] << std::endl;
				return false;
			}
		}
		IntObject * new_int = new IntObject(val);
		addObject(new_int);
	}
	else {
		std::cout << "unrecognized command" << std::endl;
		return false;
	}
}

void AnimationManager::decrementLinkOutIndsGreaterThan(int ind, Object * obj)
{
	std::vector<Link *> links_to_remove;
	for (std::unordered_set<Link *>::iterator it = obj_graph[obj].inputs.begin(); it != obj_graph[obj].inputs.end(); ++it) {
		if ((*it)->getOutInd() > ind) (*it)->setOutInd((*it)->getOutInd() - 1);
		else if ((*it)->getOutInd() == ind) links_to_remove.push_back(*it);
	}
	for (std::vector<Link*>::iterator it = links_to_remove.begin(); it != links_to_remove.end(); ++it){
		deleteLink(*it);
	}
}

void AnimationManager::pushToTop(Object * obj)
{
	std::vector<Object *>::iterator it = objects.begin();
	while (*it != obj) ++it;
	if (it != objects.end()) {
		objects.erase(it);
		objects.push_back(obj);
	}
}

void AnimationManager::deleteObject(Object * object_to_delete) {
	while (obj_graph[object_to_delete].inputs.size() > 0) {
		deleteLink(*obj_graph[object_to_delete].inputs.begin());
	}
	while (obj_graph[object_to_delete].outputs.size() > 0) {
		deleteLink(*obj_graph[object_to_delete].outputs.begin());
	}
	obj_graph.erase(object_to_delete);
	if (object_to_delete->getObjectType() == OBJECT_TYPE::FREQBANDBLOCK) {
		std::vector<FreqBandBlock *>::iterator position = freq_band_objects.begin();
		while ((Object *)(*position) != object_to_delete) {
			++position;
		}
		freq_band_objects.erase(position);
		std::cout << "special deleted freq band block" << std::endl;
	}
	std::vector<Object *>::iterator position = objects.begin();
	while ((*position) != object_to_delete) {
		++position;
	}
	objects.erase(position);
	position = root_objects.begin();
	while ((*position) != object_to_delete) {
		++position;
	}
	if (position != root_objects.end()) {
		root_objects.erase(position);
	}
	delete object_to_delete;
}

void AnimationManager::deleteLink(Link * link_to_delete) {
	//remove link from all objects
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
	//TODO: check for loops!!
	//We expect new_link->getOutObj() to not be null, otherwise we should have never entered this routine
	if (new_link->getOutObj()->getMultipleInputsAllowed(new_link->getOutInd()) == false) {
		Link * link_to_delete = NULL;
		for (std::unordered_set<Link *>::iterator it = obj_graph[new_link->getOutObj()].inputs.begin(); it != obj_graph[new_link->getOutObj()].inputs.end(); ++it) {
			if ((*it)->getOutInd() == new_link->getOutInd()) {
				link_to_delete = *it;
				break;
			}
		}
		if (link_to_delete != NULL) {
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
	if (new_obj->getObjectType() == OBJECT_TYPE::FREQBANDBLOCK) {
		freq_band_objects.push_back((FreqBandBlock *)new_obj);
	}
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
	for (std::vector<FreqBandBlock *>::iterator it = freq_band_objects.begin(); it != freq_band_objects.end(); ++it) {
		(*it)->sendRangeToHandler(audio_handler);
	}
	audio_handler->update();
	for (std::vector<FreqBandBlock *>::iterator it = freq_band_objects.begin(); it != freq_band_objects.end(); ++it) {
		(*it)->updateValsFromHandler(audio_handler);
	}
	state.delete_selected = false;
	for (std::map<Link *, bool>::iterator it = updated_links.begin(); it != updated_links.end(); ++it) {
		it->second = false;
	}
	for (std::map<Object *, ObjectNode>::iterator it = obj_graph.begin(); it != obj_graph.end(); ++it) {
		it->first->setParamsToDefault();
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
						new_obj->setParameter((*out_link_it)->getParameterFromLink(),(*out_link_it)->getOutInd());
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

void AnimationManager::clickNewLT() {
	Operator * new_LT = new Operator(COMPARATOR::LT);
	new_LT->setPosition(sf::Vector2f(32, 32));
	addObject(new_LT);
}

void AnimationManager::clickNewLTE() {
	Operator * new_LTE = new Operator(COMPARATOR::LTE);
	new_LTE->setPosition(sf::Vector2f(32, 32));
	addObject(new_LTE);
}

void AnimationManager::clickNewGTE() {
	Operator * new_GTE = new Operator(COMPARATOR::GTE);
	new_GTE->setPosition(sf::Vector2f(32, 32));
	addObject(new_GTE);
}

void AnimationManager::clickNewGT() {
	Operator * new_GT = new Operator(COMPARATOR::GT);
	new_GT->setPosition(sf::Vector2f(32, 32));
	addObject(new_GT);
}

void AnimationManager::clickNewEQ() {
	Operator * new_EQ = new Operator(COMPARATOR::EQ);
	new_EQ->setPosition(sf::Vector2f(32, 32));
	addObject(new_EQ);
}

void AnimationManager::clickNewNEQ() {
	Operator * new_NEQ = new Operator(COMPARATOR::NEQ);
	new_NEQ->setPosition(sf::Vector2f(32, 32));
	addObject(new_NEQ);
}

void AnimationManager::clickNewPlus() {
	Operator * new_plus = new Operator(ARITHMETIC::PLUS);
	new_plus->setPosition(sf::Vector2f(32, 32));
	addObject(new_plus);
}

void AnimationManager::clickNewMinus() {
	Operator * new_minus = new Operator(ARITHMETIC::MINUS);
	new_minus->setPosition(sf::Vector2f(32, 32));
	addObject(new_minus);
}

void AnimationManager::clickNewTimes() {
	Operator * new_times = new Operator(ARITHMETIC::TIMES);
	new_times->setPosition(sf::Vector2f(32, 32));
	addObject(new_times);
}

void AnimationManager::clickNewDividedby() {
	Operator * new_dividedby = new Operator(ARITHMETIC::DIVIDEDBY);
	new_dividedby->setPosition(sf::Vector2f(32, 32));
	addObject(new_dividedby);
}

void AnimationManager::clickNewModulo() {
	Operator * new_modulo = new Operator(ARITHMETIC::MODULO);
	new_modulo->setPosition(sf::Vector2f(32, 32));
	addObject(new_modulo);
}

void AnimationManager::clickNewExponent() {
	Operator * new_exponent = new Operator(ARITHMETIC::EXPONENT);
	new_exponent->setPosition(sf::Vector2f(32, 32));
	addObject(new_exponent);
}

void AnimationManager::clickNewInt() {
	IntObject * new_int = new IntObject();
	new_int->setPosition(sf::Vector2f(32, 32));
	addObject(new_int);
}

void AnimationManager::clickNewFloat()
{
	FloatObject * new_float = new FloatObject();
	new_float->setPosition(sf::Vector2f(32, 32));
	addObject(new_float);
}

void AnimationManager::clickNewPI()
{
	FloatObject * new_float = new FloatObject(3.14159265358979f);
	new_float->setPosition(sf::Vector2f(32, 32));
	addObject(new_float);
}

void AnimationManager::clickNewE()
{
	FloatObject * new_float = new FloatObject(2.71828182846f);
	new_float->setPosition(sf::Vector2f(32, 32));
	addObject(new_float);
}

void AnimationManager::clickNewCos()
{
	MathFunction * new_func = new MathFunction(0);
	new_func->setPosition(sf::Vector2f(32, 32));
	addObject(new_func);
}

void AnimationManager::clickNewSin()
{
	MathFunction * new_func = new MathFunction(1);
	new_func->setPosition(sf::Vector2f(32, 32));
	addObject(new_func);
}
void AnimationManager::clickNewTan()
{
	MathFunction * new_func = new MathFunction(2);
	new_func->setPosition(sf::Vector2f(32, 32));
	addObject(new_func);
}
void AnimationManager::clickNewLog()
{
	MathFunction * new_func = new MathFunction(3);
	new_func->setPosition(sf::Vector2f(32, 32));
	addObject(new_func);
}

void AnimationManager::clickNewClock()
{
	ClockObject * new_clock = new ClockObject();
	new_clock->setPosition(sf::Vector2f(32, 32));
	addObject(new_clock);
}
