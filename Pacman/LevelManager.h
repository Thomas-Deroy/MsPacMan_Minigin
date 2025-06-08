#pragma once
#include "Singleton.h"
#include "LevelBuilder.h"
#include "GameObject.h"
#include "Scene.h"

namespace dae
{
	class LevelManager final : public Singleton<LevelManager>
	{
	public:
		void LoadLevelManager(Scene* scene, int level = 1);

		void NextLevel();
		void RestartLevel();

		void AddLevelObjects(GameObject* object);


		LevelBuilder* GetLevelBuilder();
	private:
		friend class Singleton<LevelManager>; 

		LevelManager() = default;
		~LevelManager() = default;

		void LoadLevel();

		Scene* m_Scene{ nullptr };
		int m_CurrentLevel{ 1 };
		LevelBuilder m_Builder;

		std::unique_ptr<GameObject> m_LevelTexture{ nullptr };
		std::vector<GameObject*> m_LevelObjects;
		GameObject* m_LevelTexturePtr{ nullptr };
	};
}
