
#include <SFML/Audio.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class MicRec : public sf::SoundRecorder
{
public:
	SoundBuffer* buf;
	string* t;
	int updates = 0;
	MicRec()
	{

	}
	void setInfo(SoundBuffer &buff, string &tt)
	{
		buf = &buff;
		t = &tt;
		updates++;
	}
	virtual bool OnStart()
	{

		return true;
	}

	virtual bool onProcessSamples(const Int16 *samples, std::size_t sampleCount)
	{
		if (updates > 0)
			cout << "Updated times in a row: " << to_string(updates) << endl;
		updates = 0;
		buf->loadFromSamples(samples, sampleCount, 2, 92000);
		
		*t = "worked!";
		return true;
	}

	virtual void OnStop()
	{
	}

};