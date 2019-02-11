#pragma once
#include "WBScene.h"
#include "Common.h"
class Corcle {
private:
	sf::Vector2f position;
	float radius;
	bool expanding;
	rgb color;
public:
	Corcle() {
		position = sf::Vector2f(0.0, 0.0);
		radius = 0.0;
		expanding = true;
	}
	void draw() {
		glColor3f(color.r, color.g, color.b);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(6.0f);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 24; i++) {
			double radians = (double)i * PI / 12.0;
			glVertex2f(radius * cos(radians), radius * sin(radians));
		}
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	void setPosition(sf::Vector2f new_pos) {
		position = new_pos;
	}
	sf::Vector2f getPosition() {
		return position;
	}
	void setColor(rgb new_color) {
		color = new_color;
	}
	void setRadius(float new_radius) {
		radius = new_radius;
	}
	float getRadius() {
		return radius;
	}
	bool update(double speed) {
		radius = radius + (expanding ? speed : -speed);
		if (expanding) {
			return radius > 4.0;
		}
		else {
			return radius < 0.0;
		}
	}
};

class FallingLeaves :
	public WBScene
{
private:
	int hue;
	float last_bass = 0.0;
	float rot_val;
	std::vector<Corcle *> corcles;
public:
	void update();
	void draw();
	FallingLeaves();
	~FallingLeaves();
};

