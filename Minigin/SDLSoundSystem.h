#pragma once
#include "SoundSystem.h"
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace dae
{
    class SDLSoundSystem final : public SoundSystem
    {
    public:
        SDLSoundSystem();
        ~SDLSoundSystem() override;

        void LoadSound(sound_id id, const std::string& path) override;
        void Play(sound_id id, float volume, bool loop = false);
        void SetVolume(sound_id id, float volume) override;
        void StopAll() override;
        void PauseAll() override;
        void ResumeAll() override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_pImpl;
    };
}