#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <math.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#define NDEBUG
#include <ffft/FFTReal.h>

extern double freqAtKey(double key);

class WavyBoiRecorder : public sf::SoundRecorder
{
	sf::Clock clock;
	sf::Time last_time;
	ffft::FFTReal<double> * fft_object;
	long size = 1024; //should be a power of 2
	double * x;
	double * f;
	double * last_f;
	sf::Mutex last_f_mutex;
	
	virtual bool onStart() // optional
	{
		f = (double *)malloc(size * sizeof(double));
		x = (double *)malloc(size * sizeof(double));
		last_f = (double *)malloc(1000 * sizeof(double));
		setProcessingInterval(sf::milliseconds(size * 1000./44100. / 0.9)); //we want 30 FPS fidelity on audio processing. to get a sample size of 16384, we need 30*16384 = 1000000 samples per second
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
		for (int i = 0; i < 1000; i++) {
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
	WavyBoiRecorder audio_recorder;
	double curr_max = 0;
	bool normalize_request = false;
	std::vector<std::pair<int, int>> ranges;
	std::unordered_map<long unsigned int, double> range_maxes;
	std::unordered_map<long unsigned int, double> range_avgs;
public:
	AudioHandler();
	~AudioHandler();
	double getMaxAtRange(std::pair<int, int>);
	double getAvgAtRange(std::pair<int, int>);
	void addRange(std::pair<int, int>);
	void resetLevels();
	void draw(sf::RenderTarget &, sf::RenderStates);
	void update();
	void normalize();
};

extern AudioHandler * audio_handler;

