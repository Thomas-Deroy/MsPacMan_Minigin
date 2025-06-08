#include "LevelManager.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include <iostream>

namespace dae
{
	void LevelManager::LoadLevelManager(Scene* scene, int level)
	{
		m_Scene = scene;
		m_CurrentLevel = level;

		m_LevelTexture = std::make_unique<GameObject>();
		m_LevelTexturePtr = m_LevelTexture.get();
		m_LevelTexture->AddComponent<RenderComponent>("MSPC_Level1.tga", 2.f);
		m_LevelTexture->SetPosition(0.f, 40.f);
		m_LevelTexture->SetRenderLayer(0);
		m_Scene->Add(m_LevelTexture);

		LoadLevel();
	}

	void LevelManager::NextLevel()
	{
		m_Scene->Pause();
		++m_CurrentLevel;
		LoadLevel();
		for (GameObject* obj : m_LevelObjects)
		{
			obj->GetComponent<MovementComponent>()->FindAndSetStartNode(&m_Builder);
		}
		m_Scene->Resume();
	}

	void LevelManager::RestartLevel()
	{
		m_CurrentLevel = 1;
		LoadLevel();
	}

	void LevelManager::LoadLevel()
	{
		switch (m_CurrentLevel)
		{
		case 1:
			m_LevelTexturePtr->GetComponent<RenderComponent>()->SetTexture("MSPC_Level1.tga");
			m_Builder.UnloadLevel();
			m_Builder.SetPelletColor(1.f, 1.f, 1.f); // WHITE
			m_Builder.LoadLevelFromFile("../Data/Level1.txt", m_Scene, -2.f, 0.f);
			break;
		case 3:
			m_LevelTexturePtr->GetComponent<RenderComponent>()->SetTexture("MSPC_Level2.tga");
			m_Builder.UnloadLevel();
			m_Builder.SetPelletColor(1.f, 1.f, 0.f); // YELLOW
			m_Builder.LoadLevelFromFile("../Data/Level2.txt", m_Scene, -2.f, 0.f);
			break;
		case 5:
			m_LevelTexturePtr->GetComponent<RenderComponent>()->SetTexture("MSPC_Level3.tga");
			m_Builder.UnloadLevel();
			m_Builder.SetPelletColor(1.f, 0.f, 0.f); // RED
			m_Builder.LoadLevelFromFile("../Data/Level3.txt", m_Scene, -2.f, 0.f);
			break;
		case 7:
			m_LevelTexturePtr->GetComponent<RenderComponent>()->SetTexture("MSPC_Level4.tga");
			m_Builder.UnloadLevel();
			m_Builder.SetPelletColor(1.f, 1.f, 1.f); // WHITE
			m_Builder.LoadLevelFromFile("../Data/Level4.txt", m_Scene, -2.f, 0.f);
			break;
		case 10:
			m_LevelTexturePtr->GetComponent<RenderComponent>()->SetTexture("MSPC_Level5.tga");
			m_Builder.UnloadLevel();
			m_Builder.SetPelletColor(0.f, 1.f, 1.f); // CYAN
			m_Builder.LoadLevelFromFile("../Data/Level3.txt", m_Scene, -2.f, 0.f);
			break;
		case 12:
			m_LevelTexturePtr->GetComponent<RenderComponent>()->SetTexture("MSPC_Level6.tga");
			m_Builder.UnloadLevel();
			m_Builder.SetPelletColor(1.f, 1.f, 1.f); // WHITE
			m_Builder.LoadLevelFromFile("../Data/Level4.txt", m_Scene, -2.f, 0.f);
			break;
		default:
			m_Builder.ResetPellets();
			break;
		}
		
	}

	void LevelManager::AddLevelObjects(GameObject* object)
	{
		m_LevelObjects.emplace_back(object);
	}

	LevelBuilder* LevelManager::GetLevelBuilder()
	{
		return &m_Builder;
	}
}
