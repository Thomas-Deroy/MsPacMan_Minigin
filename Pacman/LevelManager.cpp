#include "LevelManager.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "GhostAIComponent.h"
#include "FruitComponent.h"
#include "FruitHiddenState.h"
#include <iostream>

namespace dae
{
	void LevelManager::LoadLevelManager(Scene* scene, EventSystem* event, SoundSystem* sound, int level)
	{
		m_Scene = scene;
		m_EventSystem = event;
		m_SoundSystem = sound;
		m_CurrentLevel = level;

		// GameObject that will be managed in the LevelManager
		m_LevelTexture = std::make_unique<GameObject>();
		m_LevelTexturePtr = m_LevelTexture.get();
		m_LevelTexture->AddComponent<RenderComponent>("MSPC_Level1.tga", 2.f);
		m_LevelTexture->SetPosition(0.f, 40.f);
		m_LevelTexture->SetRenderLayer(0);
		m_Scene->Add(m_LevelTexture);

		m_TransitionScreen = std::make_unique<dae::GameObject>();
		m_TransitionScreenPtr = m_TransitionScreen.get();
		m_TransitionScreen->AddComponent<dae::RenderComponent>("MSPC_BlackScreen.tga", 2.0f);
		m_TransitionScreen->SetPosition(0.f, 40.f);
		m_TransitionScreen->GetComponent<dae::RenderComponent>()->SetColor(0.f, 0.f, 0.f, 0.f);
		m_TransitionScreen->SetRenderLayer(3);
		m_Scene->Add(m_TransitionScreen);

		LoadLevel();

		m_EventSystem->Subscribe(NextLevelTimer, [this](const Event&) {
			LoadLevel();
			m_MsPacmanPtr->SetPosition(POSITION_MSPACMAN);
			if(m_PacmanPtr) m_PacmanPtr->SetPosition(POSITION_PACMAN);

			m_GhostBlinkyPtr->SetPosition(POSITION_BLINKY);

			m_GhostPinkyPtr->SetPosition(POSITION_PINKY);
			m_GhostPinkyPtr->GetComponent<dae::GhostAIComponent>()->SetStartDelay(2.f);

			m_GhostInkyPtr->SetPosition(POSITION_INKY);
			m_GhostInkyPtr->GetComponent<dae::GhostAIComponent>()->SetStartDelay(4.f);

			m_GhostSuePtr->SetPosition(POSITION_Sue);
			m_GhostSuePtr->GetComponent<dae::GhostAIComponent>()->SetStartDelay(6.f);

			m_FruitPtr->GetComponent<dae::FruitComponent>()->ChangeState(std::make_unique<FruitHiddenState>());

			for (GameObject* obj : m_LevelObjects)
			{
				auto movementComponent = obj->GetComponent<MovementComponent>();
				if (movementComponent)
				{
					movementComponent->Reset(&m_Builder);
				}
			}
			m_Scene->Resume();
			m_SoundSystem->Unmute(1);
		});

		m_EventSystem->Subscribe(RandomFruitSpawn, [this](const Event&) {
			m_FruitPtr->SetActive(true);
		});
	}

	void LevelManager::NextLevel()
	{
		m_SoundSystem->Mute(1);

		m_Scene->Pause();
		m_Scene->ResumeObject(m_TransitionScreenPtr);

		m_TransitionScreenPtr->GetComponent<dae::RenderComponent>()->SetFadeEffect(dae::FadeEffect::FadeInAndOut, 4.f);
		++m_CurrentLevel;

		m_Scene->Timer(2.f, NextLevelTimer);
		m_Scene->Timer(static_cast<float>(4 + std::rand() % 16), RandomFruitSpawn);
	}

	void LevelManager::LoadLevel()
	{
		switch (m_CurrentLevel)
		{
		case 1:
			m_LevelTexturePtr->GetComponent<RenderComponent>()->SetTexture("MSPC_Level1.tga");
			m_Builder.UnloadLevel();
			m_Builder.SetPelletColor(1.f, 1.f, 1.f); // WHITE
			m_Builder.SetPowerPelletColor(1.f, 1.f, 1.f); 
			m_Builder.LoadLevelFromFile(1, m_Scene, -2.f, 0.f);
			break;
		case 2:
			m_FruitPtr->GetComponent<SpriteComponent>()->SetAnimationColumn(1);
			m_Builder.ResetPellets();
			break;
		case 3:
			m_LevelTexturePtr->GetComponent<RenderComponent>()->SetTexture("MSPC_Level2.tga");
			m_Builder.UnloadLevel();
			m_Builder.SetPelletColor(1.f, 1.f, 0.f); // YELLOW
			m_Builder.SetPowerPelletColor(1.f, 1.f, 0.f);
			m_Builder.LoadLevelFromFile(2, m_Scene, -2.f, 0.f);
			m_FruitPtr->GetComponent<SpriteComponent>()->SetAnimationColumn(2);
			break;
		case 5:
			m_LevelTexturePtr->GetComponent<RenderComponent>()->SetTexture("MSPC_Level3.tga");
			m_Builder.UnloadLevel();
			m_Builder.SetPelletColor(1.f, 0.f, 0.f); // RED
			m_Builder.SetPowerPelletColor(1.f, 0.f, 0.f);
			m_Builder.LoadLevelFromFile(3, m_Scene, -2.f, 0.f);
			m_FruitPtr->GetComponent<SpriteComponent>()->SetAnimationColumn(3);
			break;
			
		case 7:
			m_LevelTexturePtr->GetComponent<RenderComponent>()->SetTexture("MSPC_Level4.tga");
			m_Builder.UnloadLevel();
			m_Builder.SetPelletColor(1.f, 1.f, 1.f); // WHITE
			m_Builder.SetPowerPelletColor(1.f, 1.f, 1.f);
			m_Builder.LoadLevelFromFile(4, m_Scene, -2.f, 0.f);
			m_FruitPtr->GetComponent<SpriteComponent>()->SetAnimationColumn(4);
			break;
		case 10:
			m_LevelTexturePtr->GetComponent<RenderComponent>()->SetTexture("MSPC_Level5.tga");
			m_Builder.UnloadLevel();
			m_Builder.SetPelletColor(0.f, 1.f, 1.f); // CYAN
			m_Builder.SetPowerPelletColor(0.f, 1.f, 1.f);
			m_Builder.LoadLevelFromFile(3, m_Scene, -2.f, 0.f);
			m_FruitPtr->GetComponent<SpriteComponent>()->SetAnimationColumn(5);
			break;
		case 12:
			m_LevelTexturePtr->GetComponent<RenderComponent>()->SetTexture("MSPC_Level6.tga");
			m_Builder.UnloadLevel();
			m_Builder.SetPelletColor(1.f, 1.f, 1.f); // WHITE
			m_Builder.SetPowerPelletColor(1.f, 1.f, 1.f);
			m_Builder.LoadLevelFromFile(4, m_Scene, -2.f, 0.f);
			break;
		default:
			m_Builder.ResetPellets();
			break;
		}
		
	}

	void LevelManager::AddLevelObjects(GameObject* object, CharacterType type)
	{
		switch (type)
		{
		case dae::CharacterType::MsPacman:
			m_MsPacmanPtr = object;
			break;
		case dae::CharacterType::Pacman:
			m_PacmanPtr = object;
			break;
		case dae::CharacterType::GhostInky:
			m_GhostInkyPtr = object;
			break;
		case dae::CharacterType::GhostPinky:
			m_GhostPinkyPtr = object;
			break;
		case dae::CharacterType::GhostBlinky:
			m_GhostBlinkyPtr = object;
			break;
		case dae::CharacterType::GhostSue:
			m_GhostSuePtr = object;
			break;
		case dae::CharacterType::Fruit:
			m_FruitPtr = object;
			break;
		default:
			break;
		}
		m_LevelObjects.emplace_back(object);
	}

	LevelBuilder* LevelManager::GetLevelBuilder()
	{
		return &m_Builder;
	}

	// === EVENTS === //



}
