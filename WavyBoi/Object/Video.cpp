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

void Video::init(){
	main_box = sf::RectangleShape(size);
	main_box.setOutlineThickness(-VIDEO_OUTLINE_THICKNESS);
	main_box.setOutlineColor(sf::Color(191,191,191));
	main_box.setFillColor(sf::Color(0,0,0));
	video_box = sf::RectangleShape(sf::Vector2f(0,0));
	movie = NULL;
	loadFromFile("C:/Users/Trevor/Stuff/WavyBoi/test_files/mandelbrot.mp4");
}

void Video::draw(sf::RenderTarget& target, sf::RenderStates states){
	main_box.setPosition(center - size/2.0f);
	video_box.setPosition(center - video_box.getSize()/2.0f);
	target.draw(main_box);
	target.draw(video_box);
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
	}
}

ClickResponse Video::processLeftClick(sf::Vector2i mouse_pos){
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (mouse_pos.x >= center.x - size.x/2.0f &&
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