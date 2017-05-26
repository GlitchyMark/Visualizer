#pragma once
#include <SFML/Audio.hpp>

class MicSoundRecorder : public sf::SoundRecorder
{
public:
	MicSoundRecorder();
	~MicSoundRecorder();
	virtual bool OnStart()
	{

	}

	virtual bool OnProcessSamples(const sf::Int16* Samples, std::size_t SamplesCount)
	{
	}

	virtual void OnStop()
	{
	}
};