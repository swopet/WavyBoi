#include "pch.h"
#include "WBScene.h"


void Plugin::update()
{
	draw();
}

void Plugin::draw()
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
	param val;
	val.float_val = 0.0f;
	Parameter bass_param(PARAM_TYPE::FLOAT,val,std::string("bass"));
	std::map<std::string, Parameter>::iterator bass_pos = params.find(std::string("bass"));
	if (bass_pos != params.end()) {
		bass_param = params["bass"];
	}
	float bass_val = bass_param.getValue().float_val;
	double size = 0.5 + bass_val / 2.0f;
	glBegin(GL_QUADS);
	glVertex3f(-size, -size, -1.0);
	glVertex3f(size, -size, -1.0);
	glVertex3f(size, size, -1.0);
	glVertex3f(-size, size, -1.0);
	glEnd();
	render_target->display();
}

void Plugin::setParamsToDefault()
{
}

void Plugin::setParameter(Parameter * param)
{
	const std::string key = param->getName();
	Parameter new_param;
	new_param.setName(key);
	new_param.setType(param->getType());
	new_param.setValue(param->getValue());
	params[key] = new_param;
}

Plugin::Plugin()
{
  render_target = new sf::RenderTexture();
  render_target->create(1920, 1080);
}


Plugin::~Plugin()
{
	if (render_target != NULL) {
		delete render_target;
	}
}
