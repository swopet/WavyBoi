#include "pch.h"
#include "AudioHandler.h"

AudioHandler::AudioHandler()
{
	refreshDevices();
	main_box.setSize(sf::Vector2f(gui.audio_recorder_width + gui.outline_thickness * 2, gui.audio_display_height + gui.play_24x24_tex.getSize().y + gui.outline_thickness * 2));
	main_box.setFillColor(sf::Color::Black);
	main_box.setOutlineColor(gui.obj_outline_color);
	main_box.setOutlineThickness(gui.outline_thickness);
	text_box.setFillColor(sf::Color::Black);
	text_box.setOutlineColor(gui.obj_outline_color);
	text_box.setOutlineThickness(gui.outline_thickness);
	mode = AUDIO_DEVICE;
	
}


AudioHandler::~AudioHandler()
{
	stop();
}

double AudioHandler::getMaxAtRange(std::pair<int, int> range)
{
	if (!running) {
		return 0.0;
	}
	long unsigned int key = ((long unsigned int)range.first << 16 + (long unsigned int)range.second);
	std::unordered_map<long unsigned int, double>::iterator pos = range_maxes.find(key);
	if (pos != range_maxes.end()) {
		return range_maxes[key];
	}
	else {
		return 0.0;
	}
}

double AudioHandler::getAvgAtRange(std::pair<int, int> range)
{
	if (!running) {
		return 0.0;
	}
	long unsigned int key = ((long unsigned int)range.first << 16 + (long unsigned int)range.second);
	std::unordered_map<long unsigned int, double>::iterator pos = range_avgs.find(key);
	if (pos != range_avgs.end()) {
		return range_avgs[key];
	}
	else {
		return 0.0;
	}
}

void AudioHandler::setPosition(sf::Vector2f new_position)
{
	position = new_position;
}

void AudioHandler::refreshDevices()
{
	available_devices = sf::SoundRecorder::getAvailableDevices();
    curr_device = 0;
    if (available_devices.size() != 0) {
      name = available_devices[0];
    }
    else {
      name = "<no device>";
    }
}

void AudioHandler::start(AUDIO_MODE new_mode, std::string file_or_device_name)
{
	name = file_or_device_name;
	mode = new_mode;
	switch (mode) {
	case AUDIO_DEVICE:
		audio_recorder = new WavyBoiRecorder();
		audio_recorder->setSize(size);
		audio_recorder->setDevice(name);
		audio_recorder->start(48000);
		running = true;
		normalize_request = true;
		break;
	case AUDIO_FILE:
		if (sound_buffer.loadFromFile(name)) {
			std::cout << "loaded " << name << std::endl;
			std::cout << "channels: " << sound_buffer.getChannelCount() << std::endl;
			std::cout << "sample rate: " << sound_buffer.getSampleRate() << std::endl;
		}
		else {
			std::cout << "could not load " << name << std::endl;
		}
		sound.setBuffer(sound_buffer);
		sound.play();
		running = false;
		break;
	case AUDIO_FILE_RECORD:
		sound_buffer.loadFromFile(name);
		running = false;
		break;
	}
}

void AudioHandler::stop()
{
	if (running) {
		switch (mode) {
		case AUDIO_DEVICE:
			if (audio_recorder != NULL) {
				audio_recorder->stop();
				delete audio_recorder;
				audio_recorder = NULL;
			}
			running = false;
			break;
		case AUDIO_FILE:
			running = false;
			break;
		case AUDIO_FILE_RECORD:
			running = false;
			break;
		}
	}
}

void AudioHandler::addRange(std::pair<int, int> new_range)
{
	int i = 0;
	while (i < ranges.size()) {
		if (ranges[i].first > new_range.first) {
			i++;
		}
		else {
			break;
		}
	}
	ranges.insert(ranges.begin() + i, new_range);
}

void AudioHandler::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	main_box.setPosition(position);
	text_box.setPosition(position + sf::Vector2f(0,main_box.getSize().y));
	
	sf::Text text;
	text.setString(name);
	text.setFont(gui.font);
	text.setCharacterSize(gui.input_text_height);
	text.setFillColor(sf::Color::White);
	text_box.setSize(sf::Vector2f(text.getLocalBounds().width + abs(gui.outline_thickness * 2), gui.audio_device_text_height + gui.menu_text_buffer));
	text.setPosition(text_box.getPosition() + sf::Vector2f(gui.outline_thickness, gui.outline_thickness));
	target.draw(main_box, states);
	pause_play_box.setTexture((running) ? &gui.pause_24x24_tex : &gui.play_24x24_tex);
	pause_play_box.setSize(sf::Vector2f(pause_play_box.getTexture()->getSize()));
	pause_play_box.setPosition(position + sf::Vector2f(gui.outline_thickness, gui.outline_thickness + gui.audio_display_height));
	stop_box.setTexture(&gui.stop_24x24_tex);
	stop_box.setSize(sf::Vector2f(stop_box.getTexture()->getSize()));
	stop_box.setPosition(position + sf::Vector2f(gui.outline_thickness + gui.play_24x24_tex.getSize().x, gui.outline_thickness + gui.audio_display_height));
	target.draw(pause_play_box, states);
	target.draw(stop_box, states);
	target.draw(text_box, states);
	target.draw(text, states);
	if (running) {
		sf::Vertex draw_vertices[1002];
		sf::Vector2f base_pos = position + sf::Vector2f(gui.outline_thickness,gui.outline_thickness + gui.audio_display_height);
		const double * freq_vals = audio_recorder->getFrequencies();
		draw_vertices[0] = sf::Vertex(base_pos);
		double max = 0;
		double freq = 0;
		for (int i = 0; i < 1000; i++) { //freq_vals[0] is frequency at E_0, freq_vals[198] is frequency at G_8
			double ratio = (i) / 1000.;
			ratio = ratio * ratio;
			draw_vertices[i + 1] = sf::Vertex(base_pos + sf::Vector2f(ratio * gui.audio_recorder_width, gui.audio_display_height * -freq_vals[i] / curr_max));
			if (freq_vals[i] > max) {

				max = freq_vals[i];
				freq = freqAtKey((double)i / 10.);
			}
			else if (freq_vals[i] == max) {
				freq = (freq + freqAtKey((double)i / 10.)) / 2.0;
			}
		}
		audio_recorder->unlockLastF();
		draw_vertices[1001] = sf::Vertex(base_pos + sf::Vector2f(gui.audio_recorder_width, 0));
		target.draw(draw_vertices, 1002, sf::LineStrip);
	}
	else {
		sf::Vertex draw_vertices[2];
		sf::Vector2f base_pos = position + sf::Vector2f(gui.outline_thickness, gui.outline_thickness + gui.audio_display_height);
		draw_vertices[0] = sf::Vertex(base_pos);
		draw_vertices[1] = sf::Vertex(base_pos + sf::Vector2f(gui.audio_recorder_width, 0));
		target.draw(draw_vertices, 2, sf::LineStrip);
	}

}

void AudioHandler::setFPS(double new_fps)
{
	fps = new_fps;
}

ClickResponse AudioHandler::processLeftClick(sf::Vector2i mouse_pos)
{
	ClickResponse response;
	response.clicked = false;
	if (checkIntersection(pause_play_box.getGlobalBounds(), sf::Vector2f(mouse_pos))) {
		if (running) {
			stop();
		}
		else {
            if (available_devices.size() > 0) {
              start(mode, name);
            }
            else {
              refreshDevices();
            }
		}
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
	}
	else if (checkIntersection(stop_box.getGlobalBounds(), sf::Vector2f(mouse_pos))) {
		stop();
		response.clicked = true;
		response.type = CLICK_RESPONSE::PROCESSED;
	}
	return response;
}

ClickResponse AudioHandler::processMouseWheel(sf::Vector2i mouse_pos, int delta)
{
	ClickResponse response;
	response.clicked = false;
	response.type = CLICK_RESPONSE::NONE;
	if (checkIntersection(text_box.getGlobalBounds(), sf::Vector2f(mouse_pos))) {
      if (available_devices.size() > 0) {
        curr_device = curr_device + delta;
        curr_device = (available_devices.size() + (curr_device % available_devices.size())) % available_devices.size();
        if (mode == AUDIO_DEVICE) {
          name = available_devices[curr_device];
          if (running) {
            stop();
            start(mode, name);
          }
        }
      }
	  response.clicked = true;
	  response.type = CLICK_RESPONSE::PROCESSED;
	  return response;
	}
	return response;
}

void AudioHandler::resetLevels() {
	curr_max = 1;
}

bool AudioHandler::update()
{
	if (!running) {
		ranges.clear();
		return false;
	}
	range_maxes.clear();
	range_avgs.clear();
	std::vector<std::pair<int, int>> active_ranges;
	std::vector<double> maxes;
	std::vector<double> sums;
	std::vector<int> counts;
	const double * freq_vals = audio_recorder->getFrequencies();
	if (normalize_request) {
		resetLevels();
		normalize_request = false;
	}
	for (int i = 0; i < 1000; i++) {
		double freq = freqAtKey((double)i / 10.);
		while (ranges.size() > 0) {
			//check if the queued range should be activated
			if ((double)ranges.back().first < freq) {
				std::pair<int, int> curr_range = ranges.back();
				ranges.pop_back();
				//find the index such that the lowest high val ends up on the back of the active ranges vector
				int ind = 0;
				while (ind < active_ranges.size()) {
					if (active_ranges[ind].second > curr_range.second) {
						ind++;
					}
					else {
						break;
					}
				}
				active_ranges.insert(active_ranges.begin() + ind, curr_range);
				maxes.insert(maxes.begin() + ind, 0);
				sums.insert(sums.begin() + ind, 0);
				counts.insert(counts.begin() + ind, 0);
			}
			else {
				break;
			}
		}
		double val = freq_vals[i];
		for (int i = 0; i < active_ranges.size(); i++) {
			if (val > maxes[i]) maxes[i] = val;
			sums[i] += val;
			counts[i]++;
		}
		while (active_ranges.size() > 0) {
			if ((double)active_ranges.back().second < freq) {
				//remove the range from the active range
				unsigned long int key = ((unsigned long int)active_ranges.back().first << 16 + (unsigned long int)active_ranges.back().second);
				range_maxes[key] = maxes.back();
				range_avgs[key] = sums.back() / (double)(counts.back());
				active_ranges.pop_back();
				maxes.pop_back();
				sums.pop_back();
				counts.pop_back();
			}
			else {
				break;
			}
		}
		if (val > curr_max) {
			std::cout << "new max detected: " << val << "at freq" << freq << " at ind " << i << std::endl;
			curr_max = val;
		}
	}
	audio_recorder->unlockLastF();
	//clear any non-calculated ranges
	while (active_ranges.size() > 0) {
		//remove the range from the active range
		unsigned long int key = ((unsigned long int)active_ranges.back().first << 16 + (unsigned long int)active_ranges.back().second);
		range_maxes[key] = maxes.back();
		range_avgs[key] = sums.back() / (double)(counts.back());
		active_ranges.pop_back();
		maxes.pop_back();
		sums.pop_back();
		counts.pop_back();
	}
	//normalize calculated values
	for (std::unordered_map<long unsigned int, double>::iterator iter = range_maxes.begin(); iter != range_maxes.end(); ++iter) {
		range_maxes[iter->first] = iter->second / curr_max;
	}
	for (std::unordered_map<long unsigned int, double>::iterator iter = range_avgs.begin(); iter != range_avgs.end(); ++iter) {
		range_avgs[iter->first] = iter->second / curr_max;
	}
	return true;
}

void AudioHandler::normalize()
{
	normalize_request = true;
}

double freqAtKey(double key)
{ //A440Hz is considered to be Key 53 (note that this isn't quite accurate for MIDI, A is 49 there)
	return 440 * pow(2, (key - 53) / 12.0);
}
