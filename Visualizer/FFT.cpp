#include "FFT.h"

//Credits for the origional version of this class to Muzkaw - https://www.youtube.com/watch?v=LqUuMqfW1PE
///TODO: Optimize the fuck out of everything

int sensivity = 50;

FFT::FFT(string const& _path, int const& _bufferSize)
{
	if (!SoundRecorder::isAvailable()) cout << "Unable to detect sound device" << endl;
	recorder.setChannelCount(1);
	recorder.setInfo(tempbuffer, Test);
	recorder.start();
	while (Test != "worked!") { cout << Test << endl; };
	buffer = tempbuffer;

	sampleRate = recorder.getSampleRate()*recorder.getChannelCount();
	sampleCount = buffer.getSampleCount();
	if (_bufferSize < sampleCount) bufferSize = _bufferSize;
	else bufferSize = sampleCount;

	for (int i(0); i < bufferSize; i++) window.push_back(0.54 - 0.46*cos(2 * PI*i / (float)bufferSize));

	//sample.resize(bufferSize);
}
//Does.. Something
void FFT::hammingWindow()
{
		for (int i(0); i < bufferSize; i++)
		{
			if (window.size() < i || sample.size() < i || buffer.getSampleCount() < i)
				break;
			try {
				sample[i] = Complex(buffer.getSamples()[i] * window[i], 0);
			}
			catch (exception) {}
		}
}
//Sets up frequencys... or something
void FFT::fft(CArray &x)
{
	const int N = x.size();
	if (N <= 1) return;

	CArray even = x[slice(0, N / 2, 2)];
	CArray  odd = x[slice(1, N / 2, 2)];

	fft(even);
	fft(odd);

	for (int k = 0; k < N / 2; k++)
	{
		Complex t = polar(1.0, -2 * PI * k / N) * odd[k];
		x[k] = even[k] + t;
		x[k + N / 2] = even[k] - t;
	}
}


//Not used
string FFT::dPrint()
{
	return to_string(1);
}
//Called every tick
void FFT::update()
{
	//Grab the current buffer to ignore threading issues
	buffer = tempbuffer;
	if (buffer.getSampleCount() == 0 || buffer.getDuration().asMilliseconds() < 5)
	{
		cout << "No samples!" << endl;
		return;
	}

	sampleCount = buffer.getSampleCount();
	sample.resize(bufferSize);

	//Do Muzwak's magic
	hammingWindow();

	bin = CArray(sample.data(), bufferSize);
	fft(bin);
	

	float max = 100000;
}



/*int FFT::rangeMax(int start, int end, CArray ary)
{
	float max = 0;
	//float div = end - start;
	while (end > start)
	{
		if (abs(ary[start]) > max)
			max = abs(ary[start]);
		start++;
	}
	return max;
}*/

int FFT::rangeMax(int start, int end, CArray ary)
{
	float max = 0;
	//float div = end - start;
	if (start > end)
	{
		cout << "Start: " << to_string(start) << " End: " << to_string(end);
	}
	while (end > start)
	{
		if (start > ary.size())
			break;
		if (abs(ary[start]) > max)
		{
			max = abs(ary[(int)start]);
		}
		start++;
	}
	dbgp = to_string(max);
	return max;
}

//Broken, I think
float FFT::rangeAverage(int start, int end, CArray ary)
{
	float total = 0;
	//float div = end - start;
	if (start > end)
	{
		cout << "Start: " << to_string(start) << " End: " << to_string(end);
	}
	int ticks = 0;
	while (end > start)
	{
		ticks++;
		if (start > ary.size())
		{
			end = start;
			break;
		}
		total = abs(ary[(int)start]);
		start++;
	}
	dbgp = to_string((total * 2000) / (ticks));
	return (total*2000)/(ticks);
}
//This lies, it isn't the frequency value, but the converted value to our ears on a logrithmic scale
float FFT::getFreqValue(float i)
{
	return ((log(i) / log(min(bufferSize / 2.f, 20000.f)))) * .5f; //*.9f is dumb, but works
}

vector<int> FFT::getBarValues(int bars)
{
	vector<int> brs;
	brs.resize(bars);
	//int divisor = floor(bin.size() / bars);

	int cb = 0;
	int preI = 3;
	//cout << to_string(preI) << endl;

	//starting value of grid
	//float s = (logf(3.235) / logf(min(bufferSize / 2.f, 20000.f)));
	//
	float e = 1;


	//Low and high, Range doesn't correlate to frequency.
	float low = 3;
	float high =3;//(2 - (min(bufferSize / 2.f, 20000.f) / (high));
	preI = getFreqValue(low);
	bool started = false;
	for (float i(3); i < min(bufferSize / 2.f, 20000.f); i+=1.01)
	{
		//Get fewVal of bin?
		Vector2f samplePosition(getFreqValue(i), (abs(bin[(int)i])));

		float xVal = (samplePosition.x - getFreqValue(low)) * high;

		//cout << to_string(2 - getFreqValue(high) / getFreqValue(min(bufferSize / 2.f, 20000.f))) << endl;
		int cbar = floor((xVal)*bars);



		if (xVal <= 0 && !started)
		{
			started = true;
			preI = i;
		}
		//cout << to_string(floor((samplePosition.x*(1+(s/e))-s)*bars)) << endl;

		if (brs.size() < cbar)
			continue;

		if (cb != cbar && cbar >= 0)
		{
			int lmax = rangeMax(preI, i, bin) / 500 *sensivity;
			if (lmax > 100000) lmax = 100000;
			brs[cb] = lmax;

			//cout << "Low: " << cbar << " Frequency: " << i << " xValue: " << to_string((xVal)) << " bin: " << to_string(lmax)<< endl;

			if (cb + 1 != cbar&& cbar != 0)
			{
				for (int inc = cb + 1; inc < cbar; inc++)
				{
					brs[inc] = (brs[cb] + lmax) / 2;
				}
			}
			preI = i;
			cb = cbar;
		}
		//brs[bars-1] = 100000;
	}
	return brs;
}