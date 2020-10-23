#include <pch.h>
#include <Object\Video.h>
//using namespace sfe;

Video::Video(){
	name = "unnamed video";
	type = OBJECT_TYPE::VIDEO;
	size = sf::Vector2f(80,60);
	position = sf::Vector2f(100,100);
    speed = 1.0;
	init();
}

Video::~Video()
{
	if (movie != NULL) {
		delete movie;
	}
}

void Video::initializeElements()
{
	elements.clear();
	inputs.clear();
	outputs.clear();
	GUIElement main_box(GUIElement::RECTANGLE, size.x - gui.outline_thickness * 2, size.y - gui.outline_thickness * 2);
	main_box.setPosition(sf::Vector2f(0.0f, 0.0f));
	GUIElement tex_box(GUIElement::RECTANGLE, size.x, size.y);
	tex_box.setPosition(sf::Vector2f(-gui.outline_thickness, -gui.outline_thickness));
	GUIElement play_pause_rect(GUIElement::RECTANGLE, 24, 24);
	play_pause_rect.setPosition(sf::Vector2f(0, main_box.getGlobalBounds().height));
	GUIElement stop_rect(GUIElement::RECTANGLE, 24, 24);
	stop_rect.setPosition(sf::Vector2f(24, main_box.getGlobalBounds().height));
	GUIElement loop_rect(GUIElement::RECTANGLE, 24, 24);
	loop_rect.setPosition(sf::Vector2f(48, main_box.getGlobalBounds().height));
	elements.push_back(main_box);
	elements.push_back(tex_box);
	elements.push_back(play_pause_rect);
	elements.push_back(stop_rect);
	elements.push_back(loop_rect);
	GUIElement input_circle(GUIElement::CIRCLE, gui.obj_circle_radius);
	input_circle.setPosition(sf::Vector2f(0.0f, size.y / 2.0f) +
		sf::Vector2f(-gui.outline_thickness / 2.0f, -gui.outline_thickness / 2.0f) +
		sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius)
	);
	inputs.push_back(input_circle);
	GUIElement output_circle(GUIElement::CIRCLE, gui.obj_circle_radius);
	output_circle.setPosition(sf::Vector2f(size.x, size.y / 2.0f) +
		sf::Vector2f(-1.5f * gui.outline_thickness, -gui.outline_thickness / 2.0f) +
		sf::Vector2f(-gui.obj_circle_radius, -gui.obj_circle_radius)
	);
	outputs.push_back(output_circle);
	updateGUIElements();
}

void Video::updateGUIElements() {
	if (movie != NULL) {
		elements.at(1).setTexture(&movie->getCurrentImage());
	}
	else {
		elements.at(1).setTexture(NULL);
	}
	int play_pause_rect = 2;
	int stop_rect = 3;
	int loop_rect = 4;
	elements.at(play_pause_rect).setTexture((playing) ? &gui.pause_24x24_tex : &gui.play_24x24_tex);
	elements.at(stop_rect).setTexture(&gui.stop_24x24_tex);
	elements.at(loop_rect).setTexture((loop) ? &gui.loop_true_24x24_tex : &gui.loop_false_24x24_tex);
	Object::updateGUIElements();
}

void Video::update() {
	if (movie != NULL) {
		sfe::Status status = movie->getStatus();
		if (playing) {
			if (status == sfe::Stopped) {
				if (loop) {
					std::cout << "Movie stopped, continuing" << std::endl;
					movie->play();
				}
				else {
					playing = false;
				}
                last_time = clock.getElapsedTime();
			}
            else if (speed != 1.0) {
              sf::Time elapsed = clock.getElapsedTime() - last_time;
              last_time = clock.getElapsedTime();
              sf::Time warped = elapsed * speed;
              sf::Time diff = warped - elapsed;
              if (!movie->setPlayingOffset(movie->getPlayingOffset() + diff)) {
                std::cout << "failed to warp playback speed" << std::endl;
              }
            }
		}
		movie->update();
	}

}

void Video::setSpeed(float new_speed) {
	speed = new_speed;
}

void Video::init(){
	movie = NULL;
	initializeElements();
}

void Video::togglePlay()
{
	if (playing) {
		movie->pause();
		playing = false;
	}
	else {
		movie->play();
		playing = true;
	}
}

void Video::stopAndReset()
{
	if (playing) {
		movie->stop();
	}
	playing = false;
	movie->update();
}

bool Video::loadFromFile(std::string file_name){ //load from full path
	movie = new sfe::Movie();
	if (!movie->openFromFile(file_name)) {
		std::cerr << "Could not open " << file_name << std::endl;
		delete movie;
		movie = NULL;
        return false;
	}
	else {
		std::cout << "Loaded " << file_name << std::endl;
		movie->setVolume(0.0);
		movie->play();
		movie->update();
		playing = true;
		sf::Vector2f movie_size = movie->getSize();
		if (movie_size.y != 0) { //don't know why it would but just to be sure
			sf::Vector2f main_box_size = sf::Vector2f(80,60);
			//fit the preview video to the inside of the video object
			float movie_ratio = movie_size.x / movie_size.y;
			float main_box_ratio = main_box_size.x / main_box_size.y;
			sf::Vector2f video_box_size;
			if (movie_ratio > main_box_ratio) size = sf::Vector2f(main_box_size.x, main_box_size.x / movie_ratio);
			else size = sf::Vector2f(main_box_size.y * movie_ratio, main_box_size.y);
			initializeElements();
		}
        return true;
	}
}

Parameter * Video::getNewParameter()
{
	param return_param;
	if (movie != NULL) {
		return_param.texture = &movie->getCurrentImage();
	}
	else {
		return_param.texture = NULL;
	}
	return new Parameter(PARAM_TYPE::TEXTURE, return_param, name);
}

Parameter Video::getParameter()
{

	param return_param;
	if (movie != NULL) {
		return_param.texture = &movie->getCurrentImage();
	}
	else {
		return_param.texture = NULL;
	}
	return Parameter(PARAM_TYPE::TEXTURE,return_param,name);
}

bool Video::checkOverlap(sf::RectangleShape select_box)
{
	return checkIntersection(select_box.getGlobalBounds(), elements.at(0).getGlobalBounds());
}

ClickResponse Video::processLeftClick(sf::Vector2i mouse_pos){
	int play_pause_rect = 2;
	int stop_rect = 3;
	int loop_rect = 4;
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (checkIntersection(elements.at(play_pause_rect).getGlobalBounds(), sf::Vector2f(mouse_pos))){
		togglePlay();
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
	}
	else if (checkIntersection(elements.at(stop_rect).getGlobalBounds(), sf::Vector2f(mouse_pos))) {
		stopAndReset();
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
	}
	else if (checkIntersection(elements.at(loop_rect).getGlobalBounds(), sf::Vector2f(mouse_pos))) {
		loop = !loop;
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
	}
	if (response.clicked) {
		return response;
	}
	else {
		response = Object::processLeftClick(mouse_pos);
	}
	return response;
}