#pragma once

#include <iostream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <Object/Object.h>
#include <sfeMovie/Movie.hpp>
#include <Object\Parameter.h>
#include <Common.h>

class ImageSequence : public Object {
private:
	sf::Vector2f size;
	sf::RectangleShape main_box;
	sf::RectangleShape video_box;
	sf::RectangleShape play_pause_rect;
	sf::RectangleShape stop_rect;
	sf::RectangleShape loop_rect;
	sf::CircleShape left_circle;
	sf::Vector2f left_pos;
	sf::CircleShape right_circle;
	sf::Vector2f right_pos;
	sf::Time offset;
	sf::Time last_time;
	sf::Clock clock;
	float speed = 1.0;
	float desired_FPS = 6;
	void init();
	void togglePlay();
	void stopAndReset();
	std::string path;
	std::vector<std::string> files;
	int current_file = 0;
	sf::Vector2u tex_size;
	sf::Vector2u num_tiles;
	sf::Mutex buffer_mutex;
	sf::RenderTexture * loaded_texture_tiles;
	sf::RenderTexture * loaded_texture_tiles_buffer;
	sf::RenderTexture * out_texture;
	bool playing = false;
	bool loop = false;
public:
	ImageSequence();
	Parameter * getNewParameter();
	Parameter getParameter();
	sf::Vector2f getLeftPos(int);
	sf::Vector2f getRightPos();
	void loadCurrentTiles();
	bool checkOverlap(sf::RectangleShape);
	void update();
	void setSpeed(float new_speed);
	void draw(sf::RenderTarget&, sf::RenderStates);
	void loadFromPath(std::string);
	ClickResponse processLeftClick(sf::Vector2i);
	ClickResponse processLeftClickHeld(sf::Vector2i);
	ClickResponse processLeftClickRelease(sf::Vector2i mouse_pos);
};