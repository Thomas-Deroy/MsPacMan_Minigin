#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	if (m_activeScene)
		m_activeScene->Update(deltaTime);
}

void dae::SceneManager::Render()
{
	if (m_activeScene)
		m_activeScene->Render();
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	for (const auto& scene : m_scenes)
	{
		if (scene->GetName() == name)
		{
			m_activeScene = scene.get();
			break;
		}
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::make_unique<Scene>(name);
	Scene& ref = *scene;
	m_scenes.push_back(std::move(scene));

	if (!m_activeScene) 
		m_activeScene = &ref;

	return ref;
}
