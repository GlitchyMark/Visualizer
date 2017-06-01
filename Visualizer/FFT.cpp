#include "FFT.h"

//Credits for the origional version of this class to Muzkaw - https://www.youtube.com/watch?v=LqUuMqfW1PE
//TODO: Optimize the fuck out of everything

int sensivity = 10000 * 0.85;
//int sensivity = 10;
FFT::FFT(string const& _path, int const& _bufferSize)
{
	if (!SoundRecorder::isAvailable()) cout << "Unable to detect sound device" << endl;
	recorder.setChannelCount(2);
	recorder.setInfo(tempbuffer, Test);
	recorder.start();
	cout << "Devices: ";
	for (int i = 0; i < recorder.getAvailableDevices().size(); i++)
	{
		cout << recorder.getAvailableDevices()[i] << " ";
	}
	cout << endl;
	while (Test != "worked!") { cout << Test << endl; };
	buffer = tempbuffer;

	sampleRate = recorder.getSampleRate()*recorder.getChannelCount();
	sampleCount = buffer.getSampleCount();
	//if (_bufferSize < sampleCount) bufferSize = _bufferSize;
	bufferSize = sampleCount;

	for (int i(0); i < bufferSize; i++) window.push_back(0.54 - 0.46*cos(2 * PI*i / (float)bufferSize));

	//sample.resize(bufferSize);
}

void FFT::Close()
{
	recorder.stop();
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

//This lies, it isn't the frequency value, but the converted value to our ears on a logrithmic scale
float FFT::getFreqValue(float i)
{
	return ((log(i) / log(min(bufferSize / 2.f, 20000.f)))) * .5f; //*.5f is dumb, but works - Don't remember why I did this...
}

vector<int> FFT::getBarValues(int bars)
{
	vector<int> brs;
	brs.resize(bars);
	//Curent bar being worked on
	int cb = 0;
	//End I value of pervious bar
	int preI = 3;

	//Low and high, Range doesn't correlate to frequency. - Both used differently.
	float low = 3.8;
	float high =3.75;//(2 - (min(bufferSize / 2.f, 20000.f) / (high));
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
			//TODO: setup vars for divisor and sensivity.
			int lmax = ((float)rangeMax(preI, i, bin)/ 10000000) * 500;
			if (lmax > maxHeight) lmax = maxHeight;
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
	}
	return brs;
}