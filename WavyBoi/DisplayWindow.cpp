#include <pch.h>
#include <DisplayWindow.h>

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
		window->setActive(true);
		window->clear();
		window->setTitle(animation_manager->getName());
		ObjectNode * channels = animation_manager->getChannels();
		for (int i = 0; i < 4; i++) {
			if (channels[i].inputs.size() == 1) { //ye olde channel is active if there is a single input link
				Parameter * input_param = channels[i].inputs.at(0)->getParameterFromLink();
				if (input_param->getType() == PARAM_TYPE::TEXTURE) {
					sf::RectangleShape rect_to_draw;
					const sf::Texture * frame_to_draw = input_param->getValue().texture;
					sf::Vector2f video_size = sf::Vector2f(frame_to_draw->getSize());
					sf::Vector2f screen_size = sf::Vector2f(window->getSize());
					float video_ratio = video_size.x / video_size.y;
					float display_ratio = window->getSize().x / window->getSize().y;
					if (video_ratio > display_ratio) {
						rect_to_draw.setSize(sf::Vector2f(screen_size.x,screen_size.x/video_ratio));
					}
					else {
						rect_to_draw.setSize(sf::Vector2f(screen_size.y*video_ratio,screen_size.y));
					}
					rect_to_draw.setPosition(screen_size / 2.0f - rect_to_draw.getSize() / 2.0f);
					rect_to_draw.setTexture(frame_to_draw);
					window->draw(rect_to_draw);
				}
				else {
					std::cerr << "input to display channel " << i << " is somehow not a texture! wtf dude" << std::endl;
				}
			}
		}
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
	window->setPosition(sf::Vector2i(1680,-30));
}

void DisplayWindow::close(){
	if (window != NULL){
		window->close();
		window = NULL;
	}
}