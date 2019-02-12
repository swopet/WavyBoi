#include <pch.h>
#include <Object\Video.h>
//using namespace sfe;

Video::Video(){
	name = "unnamed video";
	type = OBJECT_TYPE::VIDEO;
	size = sf::Vector2f(80+gui.outline_thickness*2,60+gui.outline_thickness*2);
	position = sf::Vector2f(100,100);
	init();
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
			}
		}
		movie->update();
		video_box.setTexture(&movie->getCurrentImage());
	}
}

void Video::setSpeed(float new_speed) {
	speed = new_speed;
}

void Video::init(){
	main_box = sf::RectangleShape(size);
	main_box.setOutlineThickness(-gui.outline_thickness);
	main_box.setOutlineColor(gui.obj_outline_color);
	main_box.setFillColor(sf::Color(0,0,0));
	left_circle = sf::CircleShape(gui.obj_circle_radius + gui.outline_thickness);
	left_circle.setOutlineThickness(-gui.outline_thickness);
	left_circle.setOutlineColor(gui.obj_outline_color);
	left_circle.setFillColor(gui.obj_fill_color);
	right_circle = left_circle;
	video_box = sf::RectangleShape(sf::Vector2f(0,0));
	video_box.setFillColor(gui.obj_fill_color);
	movie = NULL;
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

void Video::draw(sf::RenderTarget& target, sf::RenderStates states){
	main_box.setPosition(position - size/2.0f);
	video_box.setPosition(position - video_box.getSize()/2.0f);
	left_pos = position - sf::Vector2f((size.x - gui.outline_thickness) / 2.0f, 0);
	left_circle.setPosition(left_pos - sf::Vector2f(left_circle.getRadius(), left_circle.getRadius()));
	right_pos = position + sf::Vector2f((size.x - gui.outline_thickness) / 2.0f, 0);
	right_circle.setPosition(right_pos - sf::Vector2f(right_circle.getRadius(), right_circle.getRadius()));
	play_pause_rect.setSize(sf::Vector2f(24, 24));
	play_pause_rect.setPosition(main_box.getPosition() + sf::Vector2f(0,main_box.getSize().y));
	play_pause_rect.setTexture((playing) ? &gui.pause_24x24_tex : &gui.play_24x24_tex);
	stop_rect.setSize(sf::Vector2f(24, 24));
	stop_rect.setPosition(main_box.getPosition() + sf::Vector2f(24, main_box.getSize().y));
	stop_rect.setTexture(&gui.stop_24x24_tex);
	loop_rect.setSize(sf::Vector2f(24, 24));
	loop_rect.setPosition(main_box.getPosition() + sf::Vector2f(48, main_box.getSize().y));
	loop_rect.setTexture((loop) ? &gui.loop_true_24x24_tex : &gui.loop_false_24x24_tex);
	target.draw(main_box);
	target.draw(video_box);
	target.draw(left_circle);
	target.draw(right_circle);
	target.draw(play_pause_rect);
	target.draw(stop_rect);
	target.draw(loop_rect);
}

void Video::loadFromFile(std::string file_name){ //load from full path
	movie = new sfe::Movie();
	if (!movie->openFromFile(file_name)) {
		std::cerr << "Could not open " << file_name << std::endl;
		delete movie;
		movie = NULL;
	}
	else {
		std::cout << "Loaded" << file_name << std::endl;
		movie->play();
		movie->update();
		playing = true;
		sf::Vector2f movie_size = movie->getSize();
		if (movie_size.y != 0) { //don't know why it would but just to be sure
			sf::Vector2f main_box_size = main_box.getSize() - 2.0f*sf::Vector2f(gui.outline_thickness,gui.outline_thickness);
			//fit the preview video to the inside of the video object
			float movie_ratio = movie_size.x / movie_size.y;
			float main_box_ratio = main_box_size.x / main_box_size.y;
			sf::Vector2f video_box_size;
			if (movie_ratio > main_box_ratio) video_box_size = sf::Vector2f(main_box_size.x, main_box_size.x / movie_ratio);
			else video_box_size = sf::Vector2f(main_box_size.y * movie_ratio, main_box_size.y);
			video_box = sf::RectangleShape(video_box_size);
		}
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

sf::Vector2f Video::getLeftPos(int id = 0)
{
	return left_pos;
}

sf::Vector2f Video::getRightPos()
{
	return right_pos;
}

bool Video::checkOverlap(sf::RectangleShape select_box)
{
	return checkIntersection(select_box, main_box);
}

ClickResponse Video::processLeftClick(sf::Vector2i mouse_pos){
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - left_pos) <= left_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	else if (length(sf::Vector2f(mouse_pos) - right_pos) <= right_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
	}
	else if (mouse_pos.x >= position.x - size.x/2.0f &&
		mouse_pos.y >= position.y - size.y/2.0f &&
		mouse_pos.x < position.x + size.x/2.0f &&
		mouse_pos.y < position.y + size.y/2.0f){
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
	}
	else if (checkIntersection(play_pause_rect, sf::Vector2f(mouse_pos))){
		togglePlay();
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
	}
	else if (checkIntersection(stop_rect, sf::Vector2f(mouse_pos))) {
		stopAndReset();
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
	}
	else if (checkIntersection(loop_rect, sf::Vector2f(mouse_pos))) {
		loop = !loop;
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
	}
	return response;
}

ClickResponse Video::processLeftClickHeld(sf::Vector2i mouse_pos) {
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - left_pos) <= left_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	else if (length(sf::Vector2f(mouse_pos) - right_pos) <= right_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
	}
	else if (mouse_pos.x >= position.x - size.x / 2.0f &&
		mouse_pos.y >= position.y - size.y / 2.0f &&
		mouse_pos.x < position.x + size.x / 2.0f &&
		mouse_pos.y < position.y + size.y / 2.0f) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
	}
	else {

	}
	return response;
}

ClickResponse Video::processLeftClickRelease(sf::Vector2i mouse_pos) {
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - left_pos) <= left_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	else if (length(sf::Vector2f(mouse_pos) - right_pos) <= right_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
	}
	else if (mouse_pos.x >= position.x - size.x / 2.0f &&
		mouse_pos.y >= position.y - size.y / 2.0f &&
		mouse_pos.x < position.x + size.x / 2.0f &&
		mouse_pos.y < position.y + size.y / 2.0f) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
	}
	else {

	}
	return response;
}