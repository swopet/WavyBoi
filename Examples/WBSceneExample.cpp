#include "pch.h"
#include "WBSceneExample.h"



void WBSceneExample::draw()
{
	std::cout << "drawing example" << std::endl;
	render_target->setActive();
	glClearColor(0.0, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, render_target->getSize().x, render_target->getSize().y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double ratio = (double)render_target->getSize().x / (double)render_target->getSize().y;
	glOrtho(-ratio, ratio, -1.0, 1.0, -1000.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//PUT YOUR OPENGL rendering code here my friend
	
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(-0.5, 0.0, 0.0);
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(0.0, 0.8, 0.0);
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(0.5, 0.0, 0.0);
	glEnd();
	render_target->display();
}

void WBSceneExample::update()
{
	draw();
}

void WBSceneExample::setParamsToDefault()
{
}

WBSceneExample::WBSceneExample()
{
	//render_target is initialized in the default constructor; use this constructor for any animation-specific initialization
	std::cout << "initializing example" << std::endl;
}


WBSceneExample::~WBSceneExample()
{
}

extern "C" {

	__declspec(dllexport) WBScene* GetWBScene() {
		return new WBSceneExample();
	}

}