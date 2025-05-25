#include "Command.h"
#include "SceneManager.h"
#include "UIMovementComponent.h"
#include "InputManager.h"
#include "LevelManager.h"
#include "MenuScene.h"
#include "GameScene.h"

namespace dae
{
    class GameModeCommand final : public Command
    {
    public:
        GameModeCommand(UIMovementComponent* uiMovement, SceneManager& sceneManager, LevelManager& levelManager, InputManager& input)
            : m_pUIMovement(uiMovement), m_SceneManager(sceneManager), m_LevelManager(levelManager), m_InputManager(input) {
        }

        void Execute() override
        {
            if (!m_pUIMovement) return;

            int index = m_pUIMovement->GetCurrentIndex();
            dae::GameMode selectedMode;

            switch (index)
            {
            case 0: selectedMode = dae::GameMode::OnePlayer; break;
            case 1: selectedMode = dae::GameMode::TwoPlayer; break;
            case 2: selectedMode = dae::GameMode::Versus; break;
            default: return;
            }

            m_SceneManager.SetActiveScene("Game");

			LoadGameScene(m_SceneManager.GetScene("Game"), m_SceneManager, m_LevelManager, selectedMode);
        }

    private:
        UIMovementComponent* m_pUIMovement;
        SceneManager& m_SceneManager;
		InputManager& m_InputManager;
        LevelManager& m_LevelManager;
    };
}
