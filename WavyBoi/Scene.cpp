#include "pch.h"
#include "Scene.h"


void Scene::update()
{
	draw();
}

void Scene::draw()
{
	//DRAW EVERYTHING
	render_target->setActive();
	glClearColor(0.0, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, render_target->getSize().x, render_target->getSize().y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double ratio = (double)render_target->getSize().x / (double)render_target->getSize().y;
	glOrtho(-ratio, ratio, -1.0, 1.0, 0.001, 1000.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0.3, 0.0, 0.4);
	
	glBegin(GL_QUADS);
	glVertex3f(-0.5, -0.5, -1.0);
	glVertex3f(0.5, -0.5, -1.0);
	glVertex3f(0.5, 0.5, -1.0);
	glVertex3f(-0.5, 0.5, -1.0);
	glEnd();
	render_target->display();
}

Scene::Scene()
{
	context_settings.depthBits = 32;
	render_target = new sf::RenderTexture();
	render_target->create(1080, 720, context_settings);
}


Scene::~Scene()
{
}
