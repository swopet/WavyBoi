#include <pch.h>
#include <Common.h>
#include <SFML/Graphics.hpp>
//#include <sfeMovie/Movie.hpp>
#include <AnimationManager.h>
#include <ControlWindow.h>
#include <DisplayWindow.h>
#include <AudioHandler.h>
#define OPEN_DISPLAY false


extern AudioHandler * audio_handler;

void program_loop(AnimationManager * animation_manager, ControlWindow * control_window, DisplayWindow * display_window) {
	bool close_clicked = false;
	bool close_verified = false;
	loadApplicationTextures();
	audio_handler = new AudioHandler();
	sf::Clock clock;
	sf::Time last_frame = clock.getElapsedTime();
	while (!close_verified) {
		while (!close_clicked) {
			audio_handler->update();
			close_clicked = control_window->update(animation_manager);
			display_window->update(animation_manager);
			sf::Time frame_time = clock.getElapsedTime() - last_frame;
			animation_manager->updateFPS(frame_time);
			last_frame = clock.getElapsedTime();
		}
		close_verified = animation_manager->verifyClose();
	}
	delete audio_handler;
}

int main()
{
	AnimationManager * animation_manager;
	ControlWindow * control_window;
	DisplayWindow * display_window;
	animation_manager = new AnimationManager();
	control_window = new ControlWindow(animation_manager);
	display_window = new DisplayWindow();
	program_loop(animation_manager, control_window, display_window);
	control_window->close();
	delete control_window;
	delete display_window;
	delete animation_manager;
	return 0;
}