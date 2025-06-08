#pragma once
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <string>

using sound_id = unsigned short;

namespace dae
{
	// Base sound system class
    class SoundSystem
    {
    public:
        virtual ~SoundSystem() = default;

        virtual void LoadSound(sound_id id, const std::string& filePath) = 0;
        virtual void SetVolume(sound_id id, float volume) = 0;
        virtual void Play(sound_id id, float volume, bool loop) = 0;
        virtual void StopAll() = 0;
        virtual void PauseAll() = 0;
        virtual void ResumeAll() = 0;
		virtual void Mute(sound_id id) = 0;
        virtual void MuteAll() = 0;
		virtual void Unmute(sound_id id) = 0;
        virtual void UnmuteAll() = 0;
    };
}
