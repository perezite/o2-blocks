#include "Sound.h"
#include "Logger.h"
#include "Asset.h"
#include <SDL2/SDL_mixer.h>

namespace sb
{
	bool Sound::m_isInit = false;
	bool Sound::m_isValid = true;

	void Sound::init() {
		int result = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
		SB_WARNING_IF(result == -1, "unable to initialize SDL mixer");

		m_isValid = (result == 0);
		m_isInit = true;
	}

	Sound::~Sound()
	{
		if (m_sound)
			Mix_FreeChunk(m_sound);
	}

	void Sound::loadFromAsset(std::string assetPath)
	{
		if (!m_isInit) init();
		if (!m_isValid) return;

		m_assetPath = assetPath;
		std::string filePath = Asset::getFilePath(m_assetPath);
		validateFileEnding(filePath);

		// note: the Mix_LoadWAV method can actually load file types other than WAV as well..
		m_sound = Mix_LoadWAV(filePath.c_str());
		SB_WARNING_IF(m_sound == NULL, "unable to load sound " << filePath);
	}

	void Sound::play()
	{
		if (!m_isValid) return;

		int result = Mix_PlayChannel(-1, m_sound, 0);
		SB_WARNING_IF(result == -1, "unable to play sound " << m_assetPath << std::endl);
	}

	void Sound::validateFileEnding(std::string assetPath)
	{
		std::size_t pos = assetPath.rfind(".");
		std::string ending = assetPath.substr(pos, std::string::npos);

		SB_ERROR_IF(ending != ".ogg" && ending != ".wav", "Sound files must be in .ogg or .wav format" << std::endl);
	}
}
