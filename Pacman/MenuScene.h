#pragma once

namespace dae {
    class Scene;
	class SceneManager;
	class LevelManager;

	enum class GameMode
	{
		OnePlayer,
		TwoPlayer,
		Versus
	};
}

void LoadMenuScene(dae::Scene& scene, dae::SceneManager& sceneManager, dae::LevelManager& levelManager);

