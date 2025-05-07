#pragma once
#include "SoundSystem.h"

namespace dae
{
    // Dummy sound system (use as fallback)
    class NullSoundSystem : public SoundSystem
    {
    public:
        void LoadSound(sound_id, const std::string&) override {}
        void SetVolume(sound_id, float) override {}
        void Play(sound_id, float, bool) override {}
        void StopAll() override {}
        void PauseAll() override {}
        void ResumeAll() override {}
    };
}