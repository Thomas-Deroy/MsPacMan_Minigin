#pragma once

namespace dae {
    class Scene;

	enum class GameMode
	{
		OnePlayer,
		TwoPlayer,
		Versus
	};
}

void LoadMenuScene(dae::Scene& scene);

