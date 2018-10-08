#include "DisplayWindow.h"

DisplayWindow::DisplayWindow(AnimationManager * animation_manager){
	window = NULL;
}

DisplayWindow::DisplayWindow(){
	window = NULL;
}

bool DisplayWindow::update(AnimationManager * animation_manager){
	if (window == NULL && animation_manager->isDisplayOpen() == true){
		open(animation_manager);
	}
	else if (window != NULL && animation_manager->isDisplayOpen() == false){
		close();
	}
	if (window != NULL){
		window->setTitle(animation_manager->getName());
		
		window->display();
		return true;
	}
	else {
		return false;
	}
}

void DisplayWindow::open(AnimationManager * animation_manager){
	window = new sf::RenderWindow(sf::VideoMode(1600,900), "WavyBoi - " + animation_manager->getName());
	window->setPosition(sf::Vector2i(0,0));
}

void DisplayWindow::close(){
	if (window != NULL){
		window->close();
		window = NULL;
	}
}