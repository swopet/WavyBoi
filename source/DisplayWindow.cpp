#include "DisplayWindow.h"

DisplayWindow::DisplayWindow(AnimationManager * animation_manager){
	window = new sf::RenderWindow(sf::VideoMode(1600,900), "WavyBoi - " + animation_manager->get_name());
}

DisplayWindow::DisplayWindow(){
	
}

bool DisplayWindow::update(AnimationManager * animation_manager){
	window->setTitle(animation_manager->get_name());
	return false;
}

void DisplayWindow::close(){
	window->close();
}