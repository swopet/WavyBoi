#include <pch.h>
#include <Object\Video.h>
//using namespace sfe;

Video::Video(){
	name = "unnamed video";
	type = OBJECT_TYPE::VIDEO;
	size = sf::Vector2f(80+VIDEO_OUTLINE_THICKNESS*2,60+VIDEO_OUTLINE_THICKNESS*2);
	center = sf::Vector2f(100,100);
	init();
}


void Video::update() {
	if (movie != NULL) {
		movie->update();
		video_box.setTexture(&movie->getCurrentImage());
	}
}

void Video::init(){
	main_box = sf::RectangleShape(size);
	main_box.setOutlineThickness(-VIDEO_OUTLINE_THICKNESS);
	main_box.setOutlineColor(sf::Color(192,192,192));
	main_box.setFillColor(sf::Color(0,0,0));
	left_circle = sf::CircleShape(VIDEO_CIRCLE_RADIUS + VIDEO_OUTLINE_THICKNESS);
	left_circle.setOutlineThickness(-VIDEO_OUTLINE_THICKNESS);
	left_circle.setOutlineColor(sf::Color(192, 192, 192));
	left_circle.setFillColor(sf::Color(128, 128, 128));
	right_circle = left_circle;
	video_box = sf::RectangleShape(sf::Vector2f(0,0));
	video_box.setFillColor(sf::Color(128, 128, 128));
	movie = NULL;
	loadFromFile("C:/Users/Trevor/Stuff/WavyBoi/test_files/mandelbrot.mp4");
}

void Video::draw(sf::RenderTarget& target, sf::RenderStates states){
	main_box.setPosition(center - size/2.0f);
	video_box.setPosition(center - video_box.getSize()/2.0f);
	left_pos = center - sf::Vector2f((size.x - VIDEO_OUTLINE_THICKNESS) / 2.0f, 0);
	left_circle.setPosition(left_pos - sf::Vector2f(left_circle.getRadius(), left_circle.getRadius()));
	right_pos = center + sf::Vector2f((size.x - VIDEO_OUTLINE_THICKNESS) / 2.0f, 0);
	right_circle.setPosition(right_pos - sf::Vector2f(right_circle.getRadius(), right_circle.getRadius()));
	target.draw(main_box);
	target.draw(video_box);
	target.draw(left_circle);
	target.draw(right_circle);
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
		sf::Vector2f movie_size = movie->getSize();
		if (movie_size.y != 0) { //don't know why it would but just to be sure
			sf::Vector2f main_box_size = main_box.getSize() - 2.0f*sf::Vector2f(VIDEO_OUTLINE_THICKNESS,VIDEO_OUTLINE_THICKNESS);
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

param Video::getVal() {
	param return_param;
	if (movie != NULL) {
		return_param.texture = &movie->getCurrentImage();
	}
	else {
		return_param.texture = NULL;
	}
	return return_param;
}

Parameter * Video::getNewParameter()
{
	return new Parameter(PARAM_TYPE::TEXTURE, getVal(), name);
}

sf::Vector2f Video::getLeftPos()
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
	else if (mouse_pos.x >= center.x - size.x/2.0f &&
		mouse_pos.y >= center.y - size.y/2.0f &&
		mouse_pos.x < center.x + size.x/2.0f &&
		mouse_pos.y < center.y + size.y/2.0f){
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
	}
	else {
		
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
	else if (mouse_pos.x >= center.x - size.x / 2.0f &&
		mouse_pos.y >= center.y - size.y / 2.0f &&
		mouse_pos.x < center.x + size.x / 2.0f &&
		mouse_pos.y < center.y + size.y / 2.0f) {
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
	else if (mouse_pos.x >= center.x - size.x / 2.0f &&
		mouse_pos.y >= center.y - size.y / 2.0f &&
		mouse_pos.x < center.x + size.x / 2.0f &&
		mouse_pos.y < center.y + size.y / 2.0f) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
	}
	else {

	}
	return response;
}