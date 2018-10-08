#include <SFML/Graphics.hpp>
#include "AnimationManager.h"
#include "ControlWindow.h"
#include "DisplayWindow.h"
#define OPEN_DISPLAY false

int main()
{
	AnimationManager * animation_manager;
	ControlWindow * control_window;
	DisplayWindow * display_window;
	animation_manager = new AnimationManager();
	control_window = new ControlWindow(animation_manager);
	#if OPEN_DISPLAY == true
	display_window = new DisplayWindow(animation_manager);
	#endif
	bool close_clicked = false;
	bool close_verified = false;
	while(!close_verified){
		while(!close_clicked){
			close_clicked = control_window->update(animation_manager);
			#if OPEN_DISPLAY == true
			display_window->update(animation_manager);
			#endif
		}
		close_verified = animation_manager->verify_close();
	}
	control_window->close();
	#if OPEN_DISPLAY == true
	display_window->close();
	#endif
	delete control_window;
	#if OPEN_DISPLAY == true
	delete display_window;
	#endif
	delete animation_manager;
    return 0;
}