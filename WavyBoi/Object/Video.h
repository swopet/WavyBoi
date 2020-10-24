#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Object/Object.h>
#include <sfeMovie/Movie.hpp>
#include <Object\Parameter.h>
#include <Common.h>

class Video : public Object {
private:
	sf::Vector2f size;
    sf::Time last_time;
    sf::Clock clock;
	float speed = 1.0;
    sf::Time loop_time;
    sf::Time loop_start;
	void init();
	void togglePlay();
	void stopAndReset();
	sfe::Movie * movie;
	bool playing = false;
	bool loop = false;
public:
	Video();
	~Video();
	void initializeElements();
	void updateGUIElements();
	Parameter * getNewParameter();
	Parameter getParameter();
    bool getMultipleInputsAllowed(int);
    void setParamsToDefault();
    void setParameter(Parameter *, int);
	bool checkOverlap(sf::RectangleShape);
	void update();
	void setSpeed(float new_speed);
	bool loadFromFile(std::string);
	ClickResponse processLeftClick(sf::Vector2i);
};