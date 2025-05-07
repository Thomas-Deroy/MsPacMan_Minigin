#pragma once
#include "SoundSystem.h"
#include "NullSoundSystem.h"
#include <memory>

namespace dae
{
    class ServiceLocator
    {
    public:
        static SoundSystem& GetSoundSystem()
        {
            return *m_SoundSystemInstance;
        }

        static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem)
        {
            if (soundSystem) // Register a custom sound system
                m_SoundSystemInstance = std::move(soundSystem);
            else // Fall back to NullSoundSystem
                m_SoundSystemInstance = std::make_unique<NullSoundSystem>();
        }

    private:
        // Default to a NullSoundSystem to avoid null checks everywhere
        inline static std::unique_ptr<SoundSystem> m_SoundSystemInstance = std::make_unique<NullSoundSystem>();
    };
}
