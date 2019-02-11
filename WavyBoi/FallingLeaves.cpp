#include "pch.h"
#include "FallingLeaves.h"

void FallingLeaves::update()
{
	hue = hue + 1;
	param val;
	val.float_val = 0.0f;
	Parameter bass_param(PARAM_TYPE::FLOAT, val, std::string("bass"));
	Parameter mids_param(PARAM_TYPE::FLOAT, val, std::string("mids"));
	Parameter high_mids_param(PARAM_TYPE::FLOAT, val, std::string("high_mids"));
	std::map<std::string, Parameter>::iterator bass_pos = params.find(std::string("bass"));
	if (bass_pos != params.end()) {
		bass_param = params["bass"];
	}
	std::map<std::string, Parameter>::iterator mids_pos = params.find(std::string("mids"));
	if (mids_pos != params.end()) {
		mids_param = params["mids"];
	}
	std::map<std::string, Parameter>::iterator high_mids_pos = params.find(std::string("high_mids"));
	if (high_mids_pos != params.end()) {
		high_mids_param = params["high_mids"];
	}
	if (bass_param.getValue().float_val > last_bass * 1.1) {
		Corcle * new_corcle = new Corcle();
		hsv hsv_color;
		hsv_color.h = ((double)(hue % 360));
		hsv_color.s = 1.0;
		hsv_color.v = 1.0;
		rgb rgb_color = hsv2rgb(hsv_color);
		new_corcle->setColor(rgb_color);
		corcles.push_back(new_corcle);
		last_bass = bass_param.getValue().float_val;
	}
	else {
		last_bass = last_bass * 0.99;
		if (bass_param.getValue().float_val > last_bass) last_bass = bass_param.getValue().float_val;
		if (last_bass < 0.02) last_bass = 0.02;
	}
	rot_val = rot_val + (1.0 - high_mids_param.getValue().float_val);
	std::vector<int> delete_corcles;
	for (int i = 0; i < corcles.size(); i++) {
		if (corcles[i]->update(0.01 - mids_param.getValue().float_val / 50.0)) {
			delete_corcles.insert(delete_corcles.begin(),i);
		}
	}
	for (std::vector<int>::iterator it = delete_corcles.begin(); it != delete_corcles.end(); ++it) {
		delete corcles.at(*it);
		corcles.erase(corcles.begin() + *it);
	}
	draw();
}

void FallingLeaves::draw()
{
	//DRAW EVERYTHING
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
	for (std::vector<Corcle *>::iterator it = corcles.begin(); it != corcles.end(); ++it) {
		glPushMatrix();
		glTranslatef((*it)->getPosition().x, (*it)->getPosition().y, 0.0);
		float radius = (*it)->getRadius();
		glRotatef(rot_val + radius * 60.0, cos(radius*PI), sin(radius*PI), 0.0);
		(*it)->draw();
		glPopMatrix();
	}
	render_target->display();
}

FallingLeaves::FallingLeaves()
{
	context_settings.depthBits = 32;
	render_target = new sf::RenderTexture();
	render_target->create(1080, 720, context_settings);

	hue = 0;
}


FallingLeaves::~FallingLeaves()
{
}
