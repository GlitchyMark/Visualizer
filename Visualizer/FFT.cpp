#include "FFT.h"

//Credits for most of this to Muzkaw - https://www.youtube.com/watch?v=LqUuMqfW1PE

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

void FFT::hammingWindow()
{
	mark = 0.00;
		for (int i(mark); i < bufferSize + mark; i++)
		{
			if (window.size() < i || sample.size() < i || buffer.getSampleCount() < i)
				break;
			try {
				sample[i - mark] = Complex(buffer.getSamples()[i] * window[i], 0);
			}
			catch (exception) {}
			//VA1[i - mark] = Vertex(Vector2f(20, 250) + Vector2f((i - mark) / (float)bufferSize * 700, sample[i - mark].real()*0.005), Color::Color(255, 0, 0, 50));
		}
}
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

string FFT::dPrint()
{
	return to_string(1);
}

void FFT::update()
{
	buffer = tempbuffer;
	if (buffer.getSampleCount() == 0 || buffer.getDuration().asMilliseconds() < 5)
	{
		cout << "No samples!" << endl;
		return;
	}

	sampleCount = buffer.getSampleCount();
	sample.resize(bufferSize);

	hammingWindow();

	bin = CArray(sample.data(), bufferSize);
	fft(bin);
	

	float max = 100000;
}

RectangleShape cRect(int number, float r_height)
{
	sf::RectangleShape rectangle(sf::Vector2f(20, r_height));
	rectangle.setPosition((20+5) * number, 800 - r_height);
	rectangle.setFillColor(sf::Color::Green);
	return rectangle;
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
		//return 0;
		//throw invalid_argument("The fuck");
	}
	while (end > start)
	{
		if (start > ary.size())
			break;
		//cout << to_string(inc) << " : ";
		//cout << to_string(abs(ary[inc])) << endl;
		if (abs(ary[start]) > max)
		{
			max = abs(ary[(int)start]);
			//cout << to_string(max) << endl;
		}
		start++;
	}
	//if(max == 0)
	//cout << to_string(end) << endl;
	dbgp = to_string(max);
	return max;
}

float FFT::rangeAverage(int start, int end, CArray ary)
{
	float total = 0;
	//float div = end - start;
	if (start > end)
	{
		cout << "Start: " << to_string(start) << " End: " << to_string(end);
		//return 0;
		//throw invalid_argument("The fuck");
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
		//cout << to_string(inc) << " : ";
		//cout << to_string(abs(ary[inc])) << endl;
		total = abs(ary[(int)start]);
		start++;
	}
	//if(max == 0)
	//cout << to_string(end) << endl;
	dbgp = to_string((total * 2000) / (ticks));
	return (total*2000)/(ticks);
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
	float s = (logf(3.235f) / logf(min(bufferSize / 2.f, 20000.f)));
	//
	float e = 1;


	//Frequency Range
	float low = 3;
	float high = 10000;

	for (float i(3.235); i < bufferSize / 2.f; i *= 1.01)
	{
		Vector2f samplePosition((abs(logf(i) / logf(bufferSize / 2.f) - s*(1))* (1+s+.5)), abs(bin[(int)i]));


		//if (!(low < samplePosition.x < high))
		//+	continue;

		int cbar = floor((samplePosition.x)*bars);

		//cout << "Low: " << cbar << " : " << i << " : "<< to_string((samplePosition.x)) << endl;

		//cout << to_string(floor((samplePosition.x*(1+(s/e))-s)*bars)) << endl;

		if (cb != cbar)
		{
			if (brs.size() < cbar)
				break;
			int lmax = rangeMax(preI, i, bin) / 3000 *500;
			if (lmax > 70000) lmax = 70000;
			brs[cb] = lmax;
			preI = i;
			cb = cbar;
		}

	}
	return brs;
}
void FFT::draw(RenderWindow &window)
{
	int max = 100000;

	const int bars = 32;
	int brs[bars];
	//int divisor = floor(bin.size() / bars);

	int cb = 0;
	int preI = 0;
	for (float i(1); i < min(bufferSize / 2.f, 20000.f); i *= 1.005)
	{
		Vector2f samplePosition(log(i) / log(min(bufferSize / 2.f, 20000.f)), abs(bin[(int)i]));
		int cbar = floor((samplePosition.x)*bars);
		//cout << to_string(samplePosition.x) << endl;
		if(cb != cbar)
		{
		int lmax = rangeMax(preI, i , bin);
		window.draw(cRect(cbar, lmax / 30 + 30));
		preI = i;
		cb = cbar;
		}

	}
	/*for (int i = 0; i < bars; i++)
	{
		int lmax = 0;//rangeMax(i*divisor, (i + 1)*divisor, bin);
		//cout << to_string((log(i*divisor))*1000) << endl;
		lmax = rangeMax((log(i*divisor))*divisor, (log((i+1)*divisor))*divisor , bin);
		window.draw(cRect(i, lmax/10 + 30));
	}*/
	/*for (int i = 0; i < bars; i++)
	{
		window.draw(cRect(i, abs(bin[(int)i]) / max * 500));
	}*/
	//window.draw(VA1);
	//window.draw(VA3);
	//window.draw(VA2);
}