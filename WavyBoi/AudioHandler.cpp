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
	audio_recorder.start(192000);
}


AudioHandler::~AudioHandler()
{
	audio_recorder.stop();
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
		draw_vertices[i + 1] = sf::Vertex(base_pos + sf::Vector2f(i + 1, 50 * -freq_vals[i]/curr_max));
		if (freq_vals[i] > max) {
			
			max = freq_vals[i];
			freq = freqAtKey((double)i / 10.);
		}
		else if (freq_vals[i] == max) {
			freq = (freq + freqAtKey((double)i / 10.)) / 2.0;
		}
	}
	draw_vertices[1001] = sf::Vertex(base_pos + sf::Vector2f(1002, 0));
	target.draw(draw_vertices, 1002, sf::LineStrip);
}

void AudioHandler::resetLevels() {
	sub_bass_max = 1;
	bass_max = 1;
	lower_mid_max = 1;
	mid_max = 1;
	upper_mid_max = 1;
	presence_max = 1;
	brilliance_max = 1;
}

void AudioHandler::update()
{
	int i = 0;
	const double * freq_vals = audio_recorder.getFrequencies();
	double curr_total = 0;
	double ctr = 0;
	while (freqAtKey((double)i / 10.) < 60) {
		curr_total += 1 - (1-freq_vals[i])*(1-freq_vals[i]);
		ctr++;
		if (freq_vals[i] > sub_bass_max) sub_bass_max = freq_vals[i];
		i++;
	}
	sub_bass_avg = curr_total / ctr / sub_bass_max;
	curr_total = 0;
	ctr = 0;
	while (freqAtKey((double)i / 10.) < 250) {
		if (freq_vals[i] > bass_max) bass_max = freq_vals[i];
		if (freq_vals[i] / bass_max > 0.1) {
			curr_total += freq_vals[i];
			ctr++;
		}
		i++;
	}
	if (ctr == 0) bass_avg = 0;
	else bass_avg = curr_total / ctr / bass_max;
	curr_total = 0;
	ctr = 0;
	while (freqAtKey((double)i / 10.) < 500) {
		curr_total += freq_vals[i];
		ctr++;
		if (freq_vals[i] > lower_mid_max) lower_mid_max = freq_vals[i];
		i++;
	}
	lower_mid_avg = curr_total / ctr / lower_mid_max;
	curr_total = 0;
	ctr = 0;
	while (freqAtKey((double)i / 10.) < 2000) {
		curr_total += freq_vals[i];
		ctr++;
		if (freq_vals[i] > mid_max) mid_max = freq_vals[i];
		i++;
	}
	mid_avg = curr_total / ctr / mid_max;
	curr_total = 0;
	ctr = 0;
	while (freqAtKey((double)i / 10.) < 4000) {
		curr_total += freq_vals[i];
		ctr++;
		if (freq_vals[i] > upper_mid_max) upper_mid_max = freq_vals[i];
		i++;
	}
	upper_mid_avg = curr_total / ctr / upper_mid_max;
	curr_total = 0;
	ctr = 0;
	while (freqAtKey((double)i / 10.) < 6000) {
		curr_total += freq_vals[i];
		ctr++;
		if (freq_vals[i] > presence_max) presence_max = freq_vals[i];
		i++;
	}
	presence_avg = curr_total / ctr / presence_max;
	curr_total = 0;
	ctr = 0;
	while (i < 1000) {
		curr_total += freq_vals[i];
		ctr++;
		if (freq_vals[i] > brilliance_max) brilliance_max = freq_vals[i];
		i++;
	}
	brilliance_avg = curr_total / ctr / brilliance_max;
	if (sub_bass_max > curr_max) curr_max = sub_bass_max;
	if (bass_max > curr_max) curr_max = bass_max;
	if (lower_mid_max > curr_max) curr_max = lower_mid_max;
	if (mid_max > curr_max) curr_max = mid_max;
	if (upper_mid_max > curr_max) curr_max = upper_mid_max;
	if (presence_max > curr_max) curr_max = presence_max;
	if (brilliance_max > curr_max) curr_max = brilliance_max;
}

AudioHandler * audio_handler;

double freqAtKey(double key)
{ //A440Hz is considered to be Key 53 (note that this isn't quite accurate for MIDI, A is 49 there)
	return 440 * pow(2, (key - 53) / 12.0);
}
