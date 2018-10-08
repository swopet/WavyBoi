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
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	for (std::size_t i = 0; i < modes.size(); ++i)
	{
		sf::VideoMode mode = modes[i];
		std::cout << "Mode #" << i << ": "
				  << mode.width << "x" << mode.height << " - "
				  << mode.bitsPerPixel << " bpp" << std::endl;
	}
	window = new sf::RenderWindow(sf::VideoMode(1920,1080), "WavyBoi - " + animation_manager->getName(), sf::Style::None);
	window->setPosition(sf::Vector2i(1680,0));
}

void DisplayWindow::close(){
	if (window != NULL){
		window->close();
		window = NULL;
	}
}