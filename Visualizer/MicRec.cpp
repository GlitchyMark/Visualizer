
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

class MicRec : public sf::SoundRecorder
{
public:
	SoundBuffer* buf;
	string* t;
	MicRec()
	{

	}
	void setInfo(SoundBuffer &buff, string &tt)
	{
		buf = &buff;
		t = &tt;
	}
	virtual bool OnStart()
	{

		return true;
	}

	virtual bool onProcessSamples(const Int16 *samples, std::size_t sampleCount)
	{
		buf->loadFromSamples(samples, sampleCount, 2, 92000);
		
		*t = "worked!";
		return true;
	}

	virtual void OnStop()
	{
	}

};