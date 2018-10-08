#include <SFML/Graphics.hpp>
#include <sfeMovie/Movie.hpp>
#include "AnimationManager.h"
#include "ControlWindow.h"
#include "DisplayWindow.h"
#define OPEN_DISPLAY false

void program_loop(AnimationManager * animation_manager, ControlWindow * control_window, DisplayWindow * display_window){
	bool close_clicked = false;
	bool close_verified = false;
	while(!close_verified){
		while(!close_clicked){
			close_clicked = control_window->update(animation_manager);
			display_window->update(animation_manager);
		}
		close_verified = animation_manager->verifyClose();
	}	
}

int main()
{
	AnimationManager * animation_manager;
	ControlWindow * control_window;
	DisplayWindow * display_window;
	animation_manager = new AnimationManager();
	control_window = new ControlWindow(animation_manager);
	display_window = new DisplayWindow();
	program_loop(animation_manager,control_window,display_window);
	control_window->close();
	delete control_window;
	delete display_window;
	delete animation_manager;
    return 0;
}