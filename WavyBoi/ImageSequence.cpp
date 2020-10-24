#include <pch.h>
#include <ImageSequence.h>
//using namespace sfe;

ImageSequence::ImageSequence() {
	name = "unnamed image sequence";
	type = OBJECT_TYPE::IMAGE_SEQUENCE;
	size = sf::Vector2f(80 + gui.outline_thickness * 2, 60 + gui.outline_thickness * 2);
	position = sf::Vector2f(100, 100);
	init();
	std::cout << sf::Texture::getMaximumSize() << std::endl;
}


void ImageSequence::update() {
	if (files.size() != 0) {
		if (playing) {
			/*sf::Time elapsed_time = clock.getElapsedTime() - last_time;
			offset = offset + elapsed_time;
			last_time = clock.getElapsedTime();
			std::cout << offset.asSeconds() << std::endl;
			int desired_frame = (int)(offset.asSeconds() * desired_FPS);
			std::cout << "desired frame: " << desired_frame << std::endl;*/
			current_file++;
			if (current_file == files.size()) {
				if (loop) {
					current_file = 0;
					offset = sf::seconds(0);
				}
				else {
					current_file = 0;
					offset = sf::seconds(0);
					playing = false;
				}
			}
			if (current_file % (num_tiles.x * num_tiles.y) == 0) {
				buffer_mutex.lock();
				std::swap(loaded_texture_tiles, loaded_texture_tiles_buffer);
				buffer_mutex.unlock();
				sf::Thread load_thread(&ImageSequence::loadCurrentTiles, this);
				load_thread.launch();
			}
			out_texture->setActive();
			int i = current_file % (num_tiles.x * num_tiles.y);
			int x = i % num_tiles.x;
			int y = i / num_tiles.x;
			out_texture->draw(sf::Sprite(loaded_texture_tiles->getTexture(), sf::IntRect(x*tex_size.x, y*tex_size.y, tex_size.x, tex_size.y)));
			out_texture->display();
		}
		if (out_texture != NULL) {
			sf::Vector2f main_box_size = main_box.getSize() - 2.0f*sf::Vector2f(gui.outline_thickness, gui.outline_thickness);
			//fit the preview video to the inside of the video object
			
			float movie_ratio = (float)tex_size.x / (float)tex_size.y;
			float main_box_ratio = main_box_size.x / main_box_size.y;
			sf::Vector2f video_box_size;
			if (movie_ratio > main_box_ratio) video_box_size = sf::Vector2f(main_box_size.x, main_box_size.x / movie_ratio);
			else video_box_size = sf::Vector2f(main_box_size.y * movie_ratio, main_box_size.y);
			video_box = sf::RectangleShape(video_box_size);
			video_box.setTexture(&out_texture->getTexture());
		}
	}
}

void ImageSequence::setSpeed(float new_speed) {
	speed = new_speed;
}

void ImageSequence::init() {
	main_box = sf::RectangleShape(size);
	main_box.setOutlineThickness(gui.outline_thickness);
	main_box.setOutlineColor(gui.obj_outline_color);
	main_box.setFillColor(sf::Color(0, 0, 0));
	left_circle = sf::CircleShape(gui.obj_circle_radius + gui.outline_thickness);
	left_circle.setOutlineThickness(gui.outline_thickness);
	left_circle.setOutlineColor(gui.obj_outline_color);
	left_circle.setFillColor(gui.obj_fill_color);
	right_circle = left_circle;
	video_box = sf::RectangleShape(sf::Vector2f(0, 0));
	video_box.setFillColor(gui.obj_fill_color);
	path = "";
	out_texture = NULL;
	loaded_texture_tiles = NULL;
}

void ImageSequence::togglePlay()
{
	if (playing) {
		playing = false;
	}
	else {
		playing = true;
		clock = sf::Clock();
		offset = sf::seconds(0);
		last_time = sf::seconds(0);
	}
}

void ImageSequence::stopAndReset()
{
	if (playing) {
		current_file = 0;
	}
	playing = false;
}

void ImageSequence::draw(sf::RenderTarget& target, sf::RenderStates states) {
	main_box.setPosition(position - size / 2.0f);
	video_box.setPosition(position - video_box.getSize() / 2.0f);
	left_pos = position - sf::Vector2f((size.x - gui.outline_thickness) / 2.0f, 0);
	left_circle.setPosition(left_pos - sf::Vector2f(left_circle.getRadius(), left_circle.getRadius()));
	right_pos = position + sf::Vector2f((size.x - gui.outline_thickness) / 2.0f, 0);
	right_circle.setPosition(right_pos - sf::Vector2f(right_circle.getRadius(), right_circle.getRadius()));
	play_pause_rect.setSize(sf::Vector2f(24, 24));
	play_pause_rect.setPosition(main_box.getPosition() + sf::Vector2f(0, main_box.getSize().y));
	play_pause_rect.setTexture((playing) ? &gui.pause_24x24_tex : &gui.play_24x24_tex);
	stop_rect.setSize(sf::Vector2f(24, 24));
	stop_rect.setPosition(main_box.getPosition() + sf::Vector2f(24, main_box.getSize().y));
	stop_rect.setTexture(&gui.stop_24x24_tex);
	loop_rect.setSize(sf::Vector2f(24, 24));
	loop_rect.setPosition(main_box.getPosition() + sf::Vector2f(48, main_box.getSize().y));
	loop_rect.setTexture((loop) ? &gui.loop_true_24x24_tex : &gui.loop_false_24x24_tex);
	target.draw(main_box, states);
	target.draw(video_box, states);
	target.draw(left_circle, states);
	target.draw(right_circle, states);
	target.draw(play_pause_rect, states);
	target.draw(stop_rect, states);
	target.draw(loop_rect, states);
}

void ImageSequence::loadFromPath(std::string path) { //load from path
	files = std::vector<std::string>();
	for (const auto & entry : std::experimental::filesystem::directory_iterator(path)) {
		std::cout << entry.path() << std::endl;
		files.push_back(entry.path().generic_string());
	}
	current_file = 0;
	if (files.size() != 0) {
		sf::Texture temp_tex;
		temp_tex.loadFromFile(files[0]);
		tex_size = temp_tex.getSize();
		int max_size = sf::Texture::getMaximumSize();
		if (max_size > 4096) max_size = 4096;
		num_tiles = sf::Vector2u(max_size / tex_size.x, max_size / tex_size.y);
		
		loaded_texture_tiles = new sf::RenderTexture();
		while (!loaded_texture_tiles->create(tex_size.x * num_tiles.x, tex_size.y * num_tiles.y)) {
			std::cout << num_tiles.x << ", " << num_tiles.y << std::endl;
			num_tiles = sf::Vector2u(num_tiles.x/2, num_tiles.y/2);
		}
		loaded_texture_tiles_buffer = new sf::RenderTexture();
		if (!loaded_texture_tiles_buffer->create(loaded_texture_tiles->getSize().x, loaded_texture_tiles->getSize().y)) {
			std::cout << "not enough memory to hold an image sequence efficiently... terminating sadly " << std::endl;
		}
		std::cout << num_tiles.x << ", " << num_tiles.y << std::endl;
		current_file = -(int)(num_tiles.x * num_tiles.y);
		loadCurrentTiles();
		buffer_mutex.lock();
		std::swap(loaded_texture_tiles, loaded_texture_tiles_buffer);
		buffer_mutex.unlock();
		current_file = 0;
		sf::Thread load_thread(&ImageSequence::loadCurrentTiles, this);
		load_thread.launch();
		out_texture = new sf::RenderTexture();
		out_texture->create(tex_size.x, tex_size.y);
		out_texture->setActive();
		int i = current_file % (num_tiles.x * num_tiles.y);
		int x = i % num_tiles.x;
		int y = i / num_tiles.x;
		out_texture->draw(sf::Sprite(loaded_texture_tiles->getTexture(),sf::IntRect(x*tex_size.x,y*tex_size.y,tex_size.x,tex_size.y)));
		out_texture->display();
	}
}

Parameter * ImageSequence::getNewParameter()
{
	param return_param;
	if (out_texture != NULL) {
		return_param.texture = &out_texture->getTexture();
	}
	else {
		return_param.texture = NULL;
	}
	return new Parameter(PARAM_TYPE::VIDEO_TEXTURE, return_param, name);
}

Parameter ImageSequence::getParameter()
{

	param return_param;
	if (out_texture != NULL) {
		return_param.texture = &out_texture->getTexture();
	}
	else {
		return_param.texture = NULL;
	}
	return Parameter(PARAM_TYPE::VIDEO_TEXTURE, return_param, name);
}

sf::Vector2f ImageSequence::getLeftPos(unsigned int id = 0)
{
	return left_pos;
}

sf::Vector2f ImageSequence::getRightPos()
{
	return right_pos;
}

void ImageSequence::loadCurrentTiles()
{
	buffer_mutex.lock();
	loaded_texture_tiles_buffer->setActive();
	int max_file = 0;
	int next_current_file = current_file + (int)(num_tiles.x * num_tiles.y);
	if (next_current_file > files.size()) next_current_file = 0;
	for (int i = next_current_file; i < next_current_file + (int)(num_tiles.x * num_tiles.y) && i < files.size(); i++) {
		int x = (i - next_current_file) % num_tiles.x;
		int y = (i - next_current_file) / num_tiles.x;
		sf::Transform transform;
		sf::Texture temp_tex;
		temp_tex.loadFromFile(files[i]);
		transform.translate(sf::Vector2f(x*tex_size.x, y*tex_size.y));
		sf::RenderStates states(transform);
		loaded_texture_tiles_buffer->draw(sf::Sprite(temp_tex), states);
		max_file = i;
	}
	loaded_texture_tiles_buffer->display();
	std::cout << "loaded tiles " << next_current_file << "-" << max_file << std::endl;
	buffer_mutex.unlock();
}

bool ImageSequence::checkOverlap(sf::RectangleShape select_box)
{
	return checkIntersection(select_box.getGlobalBounds(), main_box.getGlobalBounds());
}

ClickResponse ImageSequence::processLeftClick(sf::Vector2i mouse_pos) {
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - left_pos) <= left_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	else if (length(sf::Vector2f(mouse_pos) - right_pos) <= right_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
	}
	else if (mouse_pos.x >= position.x - size.x / 2.0f &&
		mouse_pos.y >= position.y - size.y / 2.0f &&
		mouse_pos.x < position.x + size.x / 2.0f &&
		mouse_pos.y < position.y + size.y / 2.0f) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
	}
	else if (checkIntersection(play_pause_rect.getGlobalBounds(), sf::Vector2f(mouse_pos))) {
		togglePlay();
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
	}
	else if (checkIntersection(stop_rect.getGlobalBounds(), sf::Vector2f(mouse_pos))) {
		stopAndReset();
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
	}
	else if (checkIntersection(loop_rect.getGlobalBounds(), sf::Vector2f(mouse_pos))) {
		loop = !loop;
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
	}
	return response;
}

ClickResponse ImageSequence::processLeftClickHeld(sf::Vector2i mouse_pos) {
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - left_pos) <= left_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	else if (length(sf::Vector2f(mouse_pos) - right_pos) <= right_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
	}
	else if (mouse_pos.x >= position.x - size.x / 2.0f &&
		mouse_pos.y >= position.y - size.y / 2.0f &&
		mouse_pos.x < position.x + size.x / 2.0f &&
		mouse_pos.y < position.y + size.y / 2.0f) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
	}
	else {

	}
	return response;
}

ClickResponse ImageSequence::processLeftClickRelease(sf::Vector2i mouse_pos) {
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (length(sf::Vector2f(mouse_pos) - left_pos) <= left_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_LEFT;
	}
	else if (length(sf::Vector2f(mouse_pos) - right_pos) <= right_circle.getRadius()) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::GOT_RIGHT;
	}
	else if (mouse_pos.x >= position.x - size.x / 2.0f &&
		mouse_pos.y >= position.y - size.y / 2.0f &&
		mouse_pos.x < position.x + size.x / 2.0f &&
		mouse_pos.y < position.y + size.y / 2.0f) {
		response.clicked = true;
		response.type = CLICK_RESPONSE::SELECTED;
	}
	else {

	}
	return response;
}