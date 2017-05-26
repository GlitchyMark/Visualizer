#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <complex>
#include <valarray>
#include <cmath>
#include "MicRec.cpp"
const double PI = 3.141592653589793238460;

using namespace std;
using namespace sf;

typedef complex<double> Complex;
typedef valarray<Complex> CArray;


class FFT
{
public:
	FFT(string const& _path, int const& _bufferSize);

	void hammingWindow();
	void fft(CArray &x);
	string dPrint();
	string dbgp;
	void update();

	void bars(float const& max);
	void lines(float const& max);

	int rangeMax(int start, int end, CArray ary);

	float rangeAverage(int start, int end, CArray ary);

	vector<int> getBarValues(int bars);

	void draw(RenderWindow &window);


private:
	string Test = "Not working";

	SoundBuffer tempbuffer;
	SoundBuffer buffer;
	Sound sound;
	MicRec recorder;

	vector<Complex> sample;
	vector<float> window;
	CArray bin;

	VertexArray VA1;
	VertexArray VA2;
	VertexArray VA3;
	vector<Vertex> cascade;

	int sampleRate;
	int sampleCount;
	int bufferSize;
	int mark;
};

