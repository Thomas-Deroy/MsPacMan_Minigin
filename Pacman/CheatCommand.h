#pragma once
#include "Command.h"
#include "Soundsystem.h"
#include "LevelManager.h"
#include "Scene.h"
#include <memory>

namespace dae
{
    class CheatCommand : public Command
    {
    public:
        CheatCommand(Scene* scene, LevelManager* levelManager, SoundSystem* soundSystem, int function)
            : m_Scene(scene), m_LevelManager(levelManager), m_SoundSystem(soundSystem), m_Function(function) {
        }

        void Execute() override
        {
            switch (m_Function)
            {
            case 1: 
				m_Scene->Pause();
			    m_LevelManager->NextLevel();
				m_Scene->Resume();
				break;
			case 2:
                m_SoundSystem->MuteAll();
                break;
			case 3:
                m_SoundSystem->UnmuteAll();
				break;
            default:
                break;
            }
        }

    private:
		Scene* m_Scene;
        LevelManager* m_LevelManager;
        SoundSystem* m_SoundSystem;
        int m_Function;
    };
}
