#pragma once
#include "Singleton.h"
#include "LevelBuilder.h"
#include "GameObject.h"
#include "GameEvents.h"
#include "EventSystem.h"
#include "SoundSystem.h"
#include "Scene.h"

namespace dae
{
	enum class CharacterType
	{
		MsPacman,
		Pacman,
		GhostInky,
		GhostPinky,
		GhostBlinky,
		GhostSue,
		Fruit
	};

	class LevelManager final : public Singleton<LevelManager>
	{
	public:
		void LoadLevelManager(Scene* scene, EventSystem* event, SoundSystem* sound, int level = 1);

		void NextLevel();

		void AddLevelObjects(GameObject* object, CharacterType type);

		int GetCurrentLevel() const { return m_CurrentLevel; }
		LevelBuilder* GetLevelBuilder();
	private:
		friend class Singleton<LevelManager>; 

		LevelManager() = default;
		~LevelManager() = default;

		void LoadLevel();

		const glm::vec2 POSITION_MSPACMAN {194.f, 395.f};
		const glm::vec2 POSITION_PACMAN	{200.f, 304.f};
		const glm::vec2 POSITION_BLINKY	{190.f, 200.f};
		const glm::vec2 POSITION_PINKY {190.f, 240.f};
		const glm::vec2 POSITION_INKY {150.f, 240.f};
		const glm::vec2 POSITION_Sue {240.f, 240.f};

		Scene* m_Scene{ nullptr };
		EventSystem* m_EventSystem{ nullptr };
		SoundSystem* m_SoundSystem{ nullptr };
		LevelBuilder m_Builder;

		int m_CurrentLevel{ 1 };

		std::unique_ptr<GameObject> m_LevelTexture{ nullptr };
		std::unique_ptr<GameObject> m_TransitionScreen{ nullptr };

		std::vector<GameObject*> m_LevelObjects;
		GameObject* m_MsPacmanPtr{ nullptr };
		GameObject* m_PacmanPtr{ nullptr };
		GameObject* m_GhostInkyPtr{ nullptr };
		GameObject* m_GhostPinkyPtr{ nullptr };
		GameObject* m_GhostBlinkyPtr{ nullptr };
		GameObject* m_GhostSuePtr{ nullptr };
		GameObject* m_FruitPtr{ nullptr };

		GameObject* m_LevelTexturePtr{ nullptr };
		GameObject* m_TransitionScreenPtr{ nullptr };

	};
}
