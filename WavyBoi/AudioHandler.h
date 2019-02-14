#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <math.h>
#include <Common.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <GUISettings.h>
#define NDEBUG
#include <ffft/FFTReal.h>

extern double freqAtKey(double key);

class WavyBoiRecorder : public sf::SoundRecorder
{
	sf::Clock clock;
	sf::Time last_time;
	ffft::FFTReal<double> * fft_object;
	long size = 1024; //should be a power of 2
	double sample_rate = 44100;
	double * x;
	double * f;
	double * last_f;
	sf::Mutex last_f_mutex;
	
	void setSampleRate(double new_sample_rate) {
		sample_rate = new_sample_rate;
	}

	virtual bool onStart() // optional
	{
		f = (double *)malloc(size * sizeof(double));
		x = (double *)malloc(size * sizeof(double));
		last_f = (double *)malloc(1000 * sizeof(double));
		setProcessingInterval(sf::milliseconds(size * 1000./sample_rate / 0.9)); //we want 30 FPS fidelity on audio processing. to get a sample size of 16384, we need 30*16384 = 1000000 samples per second
		// initialize whatever has to be done before the capture starts
		last_time = clock.getElapsedTime();
		fft_object = new ffft::FFTReal<double>(size);
		std::cout << "FFT length: " << fft_object->get_length() << std::endl;
		// return true to start the capture, or false to cancel it
		return true;
	}

	virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
	{
		sf::Time elapsed_time = clock.getElapsedTime() - last_time;
		for (int i = 0; i < size; i++) {
			x[i] = (double)samples[i];
		}
		fft_object->do_fft(f, x);
		double total = 0;
		last_f_mutex.lock();
		last_f[0] = 0;
		for (int i = 1; i < 1000; i++) {
			double freq = freqAtKey(((double)i) / 10.);
			int ind = (int)(freq * size / 44100.);
			if (ind < size / 2)
				last_f[i] = sqrt(f[ind] * f[ind] + f[size / 2 + ind] * f[size / 2 + ind]);
			else
				last_f[i] = 0;
			total += last_f[i];
		}
		last_f_mutex.unlock();
		double avg = total / 1000.;
		last_time = clock.getElapsedTime();
		return true;
	}

	virtual void onStop() // optional
	{
		delete fft_object;
		delete f;
		delete x;
		delete last_f;
		// clean up whatever has to be done after the capture is finished
	}
public:
	const double * getFrequencies() {
		last_f_mutex.lock();
		return last_f;
	}
	void unlockLastF() {
		last_f_mutex.unlock();
	}
	void setSize(long newSize) {
		//stop and reset if it's playing
		size = newSize;
	}
};

class AudioHandler
{
private:
	long size = 1024;
	WavyBoiRecorder * audio_recorder = NULL;
	double curr_max = 0;
	bool running = false;
	bool normalize_request = false;
	std::vector<std::pair<int, int>> ranges;
	std::unordered_map<long unsigned int, double> range_maxes;
	std::unordered_map<long unsigned int, double> range_avgs;
	int curr_device = 0;
	std::vector<std::string> available_devices;
	std::string name;
	sf::Vector2f position;
	sf::SoundBuffer sound_buffer;
	sf::Sound sound;
	sf::RectangleShape main_box;
	sf::RectangleShape text_box;
	sf::RectangleShape pause_play_box;
	sf::RectangleShape stop_box;
	int frame_counter;
	double fps;
public:
	enum AUDIO_MODE {
		AUDIO_DEVICE,
		AUDIO_FILE,
		AUDIO_FILE_RECORD
	} mode;
	AudioHandler();
	~AudioHandler();
	double getMaxAtRange(std::pair<int, int>);
	double getAvgAtRange(std::pair<int, int>);
	void setPosition(sf::Vector2f);
	void refreshDevices();
	void start(AUDIO_MODE, std::string);
	void stop();
	void addRange(std::pair<int, int>);
	void resetLevels();
	void draw(sf::RenderTarget &, sf::RenderStates);
	void setFPS(double);
	ClickResponse processLeftClick(sf::Vector2i mouse_pos);
	ClickResponse processMouseWheel(sf::Vector2i mouse_pos, int delta);
	bool update();
	void normalize();
};

extern AudioHandler * audio_handler;

