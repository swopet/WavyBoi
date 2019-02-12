#include "pch.h"
#include "AudioHandler.h"

AudioHandler::AudioHandler()
{
	std::cout << "Initializing audio handler" << std::endl;
	std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();
	int ctr = 0;
	for (std::vector<std::string>::iterator it = availableDevices.begin(); it != availableDevices.end(); ++it) {
		std::cout << "Audio Device " << ctr << ": " << *it << std::endl;
		ctr++;
	}
	//we want at least ~15 FPS fidelity on audio signal processing. to get a sample size of <size> we need a sample rate of size*30 Hz, plus a little extra to be safe
	audio_recorder.setSize(size);
	audio_recorder.start(44100);
}


AudioHandler::~AudioHandler()
{
	audio_recorder.stop();
}

double AudioHandler::getMaxAtRange(std::pair<int, int> range)
{
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
	long unsigned int key = ((long unsigned int)range.first << 16 + (long unsigned int)range.second);
	std::unordered_map<long unsigned int, double>::iterator pos = range_avgs.find(key);
	if (pos != range_avgs.end()) {
		return range_avgs[key];
	}
	else {
		return 0.0;
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
	sf::Vertex draw_vertices[1002];
	sf::Vector2f base_pos(10, 800);
	const double * freq_vals = audio_recorder.getFrequencies();
	draw_vertices[0] = sf::Vertex(base_pos);
	double max = 0;
	double freq = 0;
	for (int i = 0; i < 1000; i++) { //freq_vals[0] is frequency at E_0, freq_vals[198] is frequency at G_8
		draw_vertices[i + 1] = sf::Vertex(base_pos + sf::Vector2f((i)/10., 50 * -freq_vals[i]/curr_max));
		if (freq_vals[i] > max) {
			
			max = freq_vals[i];
			freq = freqAtKey((double)i / 10.);
		}
		else if (freq_vals[i] == max) {
			freq = (freq + freqAtKey((double)i / 10.)) / 2.0;
		}
	}
	audio_recorder.unlockLastF();
	draw_vertices[1001] = sf::Vertex(base_pos + sf::Vector2f(100, 0));
	target.draw(draw_vertices, 1002, sf::LineStrip);
}

void AudioHandler::resetLevels() {
	curr_max = 1;
}

void AudioHandler::update()
{
	range_maxes.clear();
	range_avgs.clear();
	std::vector<std::pair<int, int>> active_ranges;
	std::vector<double> maxes;
	std::vector<double> sums;
	std::vector<int> counts;
	const double * freq_vals = audio_recorder.getFrequencies();
	if (normalize_request) {
		resetLevels();
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
		if (val > curr_max) curr_max = val;
	}
	audio_recorder.unlockLastF();
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
}

void AudioHandler::normalize()
{
	normalize_request = true;
}

double freqAtKey(double key)
{ //A440Hz is considered to be Key 53 (note that this isn't quite accurate for MIDI, A is 49 there)
	return 440 * pow(2, (key - 53) / 12.0);
}
