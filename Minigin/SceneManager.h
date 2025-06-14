#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "Scene.h"

namespace dae
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		Scene& GetScene(const std::string& name);

		void SetActiveScene(const std::string& name);
		const std::string& GetActiveSceneName() { return m_activeScene->GetName(); }

		void Update(float);
		void Render();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::vector<std::unique_ptr<Scene>> m_scenes;
		Scene* m_activeScene{ nullptr };
	};
}
