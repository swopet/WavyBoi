#pragma once
#include <vector>
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
	long size = 4096; //should be a power of 2
	double * x;
	double * f;
	double * last_f;
	
	virtual bool onStart() // optional
	{
		f = (double *)malloc(size * sizeof(double));
		x = (double *)malloc(size * sizeof(double));
		last_f = (double *)malloc(1000 * sizeof(double));
		setProcessingInterval(sf::milliseconds(size * 1000./48000. / 0.9)); //we want 30 FPS fidelity on audio processing. to get a sample size of 16384, we need 30*16384 = 1000000 samples per second
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
		//fft_object->rescale(f);
		double total = 0;
		for (int i = 0; i < 1000; i++) {
			double freq = freqAtKey(((double)i) / 10.);
			unsigned int ind = (unsigned int)(freq * size/ 48000.);
			if (i == 0) ind = 0;
			if (ind < sampleCount / 2 && ind < size / 2)
				last_f[i] = sqrt(f[ind] * f[ind] + f[size / 2 + ind] * f[size / 2 + ind]);
			else
				last_f[i] = 0;
			total += last_f[i];
		}
		double avg = total / 1000.;
		last_time = clock.getElapsedTime();
		return true;
	}

	virtual void onStop() // optional
	{
		delete fft_object;
		free(f);
		free(x);
		free(last_f);
		// clean up whatever has to be done after the capture is finished
	}
public:
	const double * getFrequencies() {
		return last_f;
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
	double curr_max = 0.1;
	double sub_bass_max = 0.1; //20 to 60 Hz
	double sub_bass_avg;
	
	double bass_max = 0.1; //60 to 250 Hz
	double bass_avg;
	
	double lower_mid_max = 0.1; //250 to 500 Hz
	
	double lower_mid_avg;
	double mid_max = 0.1; //500 Hz to 2 kHz
	double mid_avg;
	double upper_mid_max = 0.1; //2 to 4 kHz
	double upper_mid_avg;
	double presence_max = 0.1; //4 to 6 kHz
	double presence_avg;
	double brilliance_max = 0.1; //>6 kHz
	double brilliance_avg;
public:
	AudioHandler();
	~AudioHandler();
	void resetLevels();
	void draw(sf::RenderTarget &, sf::RenderStates);
	void update();
	double getSubBass() {
		return sub_bass_avg;
	}
	double getBass() {
		return bass_avg;
	}
	double getLowerMid() {
		return lower_mid_avg;
	}
	double getMid() {
		return mid_avg;
	}
	double getUpperMid() {
		return upper_mid_avg;
	}
	double getPresence() {
		return presence_avg;
	}
	double getBrilliance () {
		return brilliance_avg;
	}
};

extern AudioHandler * audio_handler;

